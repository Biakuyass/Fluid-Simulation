#include"SPH_System.h"

SPHSystem::SPHSystem()
{
// 	m_unitScale         = 0.004f;	// 尺寸单位
// 	m_viscosity			= 1.0f;				// 粘度
// 	m_restDensity		= 1000.f;			// 密度
// 	m_pointMass			= 0.0004f;			// 粒子质量
// 	m_gasConstantK		= 1.0f;				// 理想气体方程常量
// 	m_smoothRadius		= 0.01f;			// 光滑核半径
// 	m_wallLength        = 0.048f;
// 	m_updateinteval     = 0.003f;
// 
// 	m_iParticleCount      = 0;
// 	m_iParticleMaxNumber = 500;
// 
// 	m_gravity = XMFLOAT3(0,-9.8f,0);
// 	//容器
// 	m_limitBox = SPHBox(-m_wallLength ,0 ,-m_wallLength ,m_wallLength ,2 * m_wallLength ,m_wallLength);
// 
// 	m_boundaryCoefficient = 10000.f;
// 	m_boundaryDampening = 256.f;

	SPH_Init();


	m_isBarrierActive = false;
	//Poly6 Kernel
	m_kernelPoly6 = 315.0f/(64.0f * PI * pow(m_smoothRadius, 9));
	//Spiky Kernel
	m_kernelSpiky = -45.0f/(PI * pow(m_smoothRadius, 6));
	//Viscosity Kernel
	m_kernelViscosity = 45.0f/(PI * pow(m_smoothRadius, 6));

	
	m_SPHRenderState = eBillPartilce;

}

void SPHSystem::SPH_Init()
{

	m_unitScale         = 0.004f;	// 尺寸单位
	m_viscosity			= 1.1f;				// 粘度
	m_restDensity		= 1000.f;			// 密度
	m_pointMass			= 0.0004f;			// 粒子质量
	m_gasConstantK		= 1.0f;				// 理想气体方程常量
	m_smoothRadius		= 0.01f;			// 光滑核半径
	m_wallLength        = 0.048f;
	m_updateinteval     = 0.003f;

	m_iParticleCount      = 0;
	m_iParticleMaxNumber = 500;

	m_gravity = XMFLOAT3(0,-9.8f,0);

	//容器
	m_limitBox = SPHBox(-m_wallLength ,0 ,-m_wallLength ,m_wallLength ,2 * m_wallLength ,m_wallLength);

	//	m_limitBox = Box(0,0,0,0 * m_unitScale,2 * 0 * m_unitScale,100 * m_unitScale);
	m_boundaryCoefficient = 10000.f;
	m_boundaryDampening = 256.f;
}

void SPHSystem::SPH_CreateParticlePool()
{


	float pointDistance	= pow(m_pointMass/m_restDensity, 1.f/3.f); //粒子间距
	
	for(float j = WallThickness / 2.0f + m_wallLength / 2.0f; m_iParticleCount < m_iParticleMaxNumber  ; j += pointDistance)
	  for(float i = -10  * m_unitScale; i <= 10 * m_unitScale && m_iParticleCount < m_iParticleMaxNumber; i += pointDistance)
		for(float p = -5 * m_unitScale; p <= 10 * m_unitScale && m_iParticleCount < m_iParticleMaxNumber; p += pointDistance)
			{


				Particle particle;

				particle.PosRender( XMFLOAT3(i / m_unitScale,j / m_unitScale,p / m_unitScale) );
				particle.PosRealScale( XMFLOAT3(i,j ,p) );
				particle.Viscosity(0);
				particle.Density(0);
				particle.Pressure(0);
				particle.Acceleration(XMFLOAT3(0,0,0));
				particle.Velocity(XMFLOAT3(0,0,0));

				m_ParticlePool.push_back(particle);

				m_iParticleCount++;
			}

}



void SPHSystem::SPH_DensityPressure(CubeCell & grids)
{
	float h2 = m_smoothRadius * m_smoothRadius;

	for(int i = 0;i < m_iParticleCount; i++)
	{
		float sum = 0;
		int neighborCellsIndex[9]; // 最多2*2*2 + ;

		grids.findNeighborCells(m_ParticlePool[i].PosRealScale(),neighborCellsIndex);
		//每次要清空邻接表重新计算
		if(!m_ParticlePool[i].m_neightbortable.empty())
		  m_ParticlePool[i].m_neightbortable.clear();

		for(int j = 0; j < 9; j++)
		{
			if(-1 == neighborCellsIndex[j])
				continue;

			
	
			for(int p = 0; p < grids.m_cubeCellParticleList[ neighborCellsIndex[j] ].size(); p++)
			{
				int fParticleIndex = grids.m_cubeCellParticleList[ neighborCellsIndex[j] ][p];
				float r2 = DistanceSquare(m_ParticlePool[i].PosRealScale(),m_ParticlePool[fParticleIndex].PosRealScale());

				if(r2 > h2)
					continue;

				m_ParticlePool[i].m_neightbortable.push_back(fParticleIndex);

				sum += pow((h2 - r2),3);

			}

		}
		m_ParticlePool[i].Density(m_kernelPoly6 * sum * m_pointMass); //因为所有粒子的重量都一样所以这里可以提出来乘
		m_ParticlePool[i].Pressure((m_ParticlePool[i].Density() - m_restDensity) * m_gasConstantK);

	}
	
}

void SPHSystem::SPH_AccelerationFigure()
{
	float h2 = m_smoothRadius * m_smoothRadius;

	for(int i = 0;i < m_iParticleCount; i++)
	{
		XMFLOAT3 accel_sum = XMFLOAT3(0,0,0);

 		for(int j = 0; j < m_ParticlePool[i].m_neightbortable.size(); j++)
 		{
			int index = m_ParticlePool[i].m_neightbortable[j];
			float r2 = DistanceSquare(m_ParticlePool[i].PosRealScale(),m_ParticlePool[index].PosRealScale());

			if(r2 > h2)
				continue;

			float r = Distance(m_ParticlePool[i].PosRealScale(),m_ParticlePool[index].PosRealScale());


			float h2_r2 = h2 - r2;
			float h_r = m_smoothRadius - r;
			XMFLOAT3 ri_rj = MinusF3(m_ParticlePool[i].PosRealScale(),m_ParticlePool[index].PosRealScale());

 			float pscalar = -m_pointMass * m_kernelSpiky * h_r * h_r * (m_ParticlePool[i].Pressure() + m_ParticlePool[index].Pressure()) / (2.0f * m_ParticlePool[i].Density() * m_ParticlePool[index].Density());

			if(r != 0)
				accel_sum = PlusF3(accel_sum,DivideF3(MultiplyF3(ri_rj,pscalar),r) ); //(ri_rj * pscalar / r )


			float vscalar = m_kernelViscosity * m_viscosity * h_r * m_pointMass / (m_ParticlePool[i].Density() * m_ParticlePool[index].Density());
			accel_sum = PlusF3(accel_sum,MultiplyF3(MinusF3(m_ParticlePool[index].Velocity(),m_ParticlePool[i].Velocity()),vscalar) );  //(vj - vi * vscalar)
 
		}
		m_ParticlePool[i].Acceleration(accel_sum);
	}
}

void SPHSystem::BounceFuction(float fbounceParameter,float boundaryCoefficient,float boundaryDampening,XMFLOAT3 vec,Particle & particle,float boundarylimit,bool isbarrier)
{

	if(fbounceParameter > 0) 
	{
		//所受到的墙的弹力减去阻力
		//XMFLOAT3 vec(1,0,0);
		if(isbarrier && fbounceParameter >= boundarylimit)
			return;

		float fbounceForce = boundaryCoefficient * fbounceParameter - boundaryDampening * DotF3(vec,particle.Velocity());

		XMFLOAT3 vBounceAcc = MultiplyF3(vec,fbounceForce);

		particle.Acceleration(PlusF3(particle.Acceleration(),vBounceAcc));

	}
}

void SPHSystem::LimitBoxBounce(Particle & particle)
{
	//X-Wall min 
	float fbounceParameterXmin = WallThickness - (particle.PosRealScale().x - m_limitBox.min_x);
	XMFLOAT3 XminVec(1,0,0);
    BounceFuction(fbounceParameterXmin,m_boundaryCoefficient,m_boundaryDampening,XminVec,particle);

	//X-Wall max 
	float fbounceParameterXmax = WallThickness - (m_limitBox.max_x - particle.PosRealScale().x);
	XMFLOAT3 XMaxVec(-1,0,0);
	BounceFuction(fbounceParameterXmax,m_boundaryCoefficient,m_boundaryDampening,XMaxVec,particle);

	//Y-Wall min 
	float fbounceParameterYmin =  WallThickness - (particle.PosRealScale().y - m_limitBox.min_y);
	XMFLOAT3 YMinVec(0,1,0);
	BounceFuction(fbounceParameterYmin,m_boundaryCoefficient,m_boundaryDampening,YMinVec,particle);

	//Z-Wall min 
	float fbounceParameterZmin = WallThickness - (particle.PosRealScale().z - m_limitBox.min_z);
	XMFLOAT3 ZMinVec(0,0,1);
	BounceFuction(fbounceParameterZmin,m_boundaryCoefficient,m_boundaryDampening,ZMinVec,particle);

	//Z-Wall max 
	float fbounceParameterZMax = WallThickness - (m_limitBox.max_z - particle.PosRealScale().z);
	XMFLOAT3 ZMaxVec(0,0,-1);
	BounceFuction(fbounceParameterZMax,m_boundaryCoefficient,m_boundaryDampening,ZMaxVec,particle);

}

void SPHSystem::BarrierBoxBounce(Particle & particle)
{
	if(m_isBarrierActive)
	{

		SPHBox Barrier = SPHBox(-m_wallLength / 3.0f ,0 ,-m_wallLength / 3.0f,m_wallLength / 3.0f ,2 * m_wallLength / 6.0f,m_wallLength / 3.0f);
		float barrierBoundaryDampening = 400;
		float barrierBoundaryCoefficient = 40000;
		float boundarylimit = 0.004f; 

		//这里加入limit的目的是为了尽力避免水与两个墙壁同时发生碰撞效果，因为采取的是弹性势能的解决方案，所以有可能发生。
		//这里的优先度是Y > Z > X，可能会发生撞到墙壁交界处受力方向不对的问题

		//Y_max侧墙壁
		if(particle.PosRealScale().x < Barrier.max_x && particle.PosRealScale().x  > Barrier.min_x
			&& particle.PosRealScale().z < Barrier.max_z && particle.PosRealScale().z  > Barrier.min_z)
		{
			float fbounceParameterYMax = Barrier.max_y - particle.PosRealScale().y;
			XMFLOAT3 YMaxVec(0,1,0);
			BounceFuction(fbounceParameterYMax,barrierBoundaryCoefficient,barrierBoundaryDampening,YMaxVec,particle,boundarylimit,true);

		}

		//X_MAX侧墙壁
		if(particle.PosRealScale().z < Barrier.max_z - boundarylimit && particle.PosRealScale().z  > Barrier.min_z + boundarylimit
			&& particle.PosRealScale().y < Barrier.max_y - boundarylimit && particle.PosRealScale().x > 0)
		{

			float fbounceParameterXMax = Barrier.max_x - particle.PosRealScale().x;
			XMFLOAT3 XMaxVec(1,0,0);
			BounceFuction(fbounceParameterXMax,barrierBoundaryCoefficient,barrierBoundaryDampening,XMaxVec,particle,boundarylimit,true);



		}

		//X_MIN侧墙壁
		if(particle.PosRealScale().z < Barrier.max_z - boundarylimit && particle.PosRealScale().z  > Barrier.min_z + boundarylimit
			&& particle.PosRealScale().y < Barrier.max_y - boundarylimit && particle.PosRealScale().x < 0)
		{

			float    fbounceParameterXMin =  particle.PosRealScale().x - Barrier.min_x;
			XMFLOAT3 XMinVec(-1,0,0);
			BounceFuction(fbounceParameterXMin,barrierBoundaryCoefficient,barrierBoundaryDampening,XMinVec,particle,boundarylimit,true);

		}

		//Z_Max侧墙壁
		if(particle.PosRealScale().x < Barrier.max_x && particle.PosRealScale().x  > Barrier.min_x
			&& particle.PosRealScale().y < Barrier.max_y - boundarylimit && particle.PosRealScale().z > 0)
		{
			float fbounceParameterZMax = Barrier.max_z - particle.PosRealScale().z;
			XMFLOAT3 ZMaxVec(0,0,1);
			BounceFuction(fbounceParameterZMax,barrierBoundaryCoefficient,barrierBoundaryDampening,ZMaxVec,particle,boundarylimit,true);

		}

		//Z_Min侧墙壁
		if(particle.PosRealScale().x < Barrier.max_x && particle.PosRealScale().x  > Barrier.min_x
			&& particle.PosRealScale().y < Barrier.max_y - boundarylimit && particle.PosRealScale().z < 0)
		{

			float fbounceParameterZMin =  particle.PosRealScale().z - Barrier.min_z;
			XMFLOAT3 ZMinVec(0,0,-1);
			BounceFuction(fbounceParameterZMin,barrierBoundaryCoefficient,barrierBoundaryDampening,ZMinVec,particle,boundarylimit,true);
		}





	}
}

void SPHSystem::ParticlePosAndSpeedUpdate(Particle & particle,float deltatime)
{
	XMFLOAT3 vAcceleration = MultiplyF3(particle.Acceleration(),deltatime); //at，增加的速度
	XMFLOAT3 vAccelerationAverage = DivideF3(vAcceleration,2.0f); // at/2.0f
	XMFLOAT3 vVelocityAverage = PlusF3(particle.Velocity(),vAccelerationAverage);// (v + at/2) 
	XMFLOAT3 vDisplacement = MultiplyF3(vVelocityAverage,deltatime); // vt + 1/2 * at^2

	particle.Velocity(PlusF3(particle.Velocity(),vAcceleration)); // v2 = v1 + at
	particle.PosRealScale(PlusF3(particle.PosRealScale(),vDisplacement));
	particle.PosRender(DivideF3(particle.PosRealScale(),m_unitScale));

}
void SPHSystem::SPH_ParticleUpdate(const float deltatime)
{
	for(int i = 0;i < m_iParticleCount;i++)
	{
		float deltatime = m_updateinteval;

		//加上重力和弹力更新加速度
		m_ParticlePool[i].Acceleration(PlusF3(m_ParticlePool[i].Acceleration(),m_gravity));


		LimitBoxBounce(m_ParticlePool[i]);

		BarrierBoxBounce(m_ParticlePool[i]);
		
		ParticlePosAndSpeedUpdate(m_ParticlePool[i],deltatime);

	}
}

void SPHSystem::SPH_Reset()
{
	m_iParticleCount      = 0;
	if(!m_ParticlePool.empty())
		m_ParticlePool.clear();
}

void SPHSystem::UpdateScene(CubeCell& cubeCells,float deltatime)
{
	SPH_DensityPressure(cubeCells);
	SPH_AccelerationFigure();
	SPH_ParticleUpdate(deltatime);
}















