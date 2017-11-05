#ifndef SPH_SYSTEM_H_
#define SPH_SYSTEM_H_

#include "SPH_Math.h"
#include"Particle.h"
#include"CubeCell.h"
#include"VertexIndexControl.h"

//const int Particle_maxnumber = 5000;
const float PI = 3.141592f;
const float WallThickness = 0.008f;
//const float WallLength = 0.048;
//const float SPHUnitScale = 0.004f;
const float DefaultSmoothRadius = 0.01f;
const float spacerange = 0.16f; // 40 * unitscale
// const float cuberange = 0.05; // 20 * unitscale
// const float thresholdvalue = 750;
// const float DefalutCubeSize = DefaultSmoothRadius / 3.0f;

//互相包含，前置声明
class CubeCell;

enum RenderState
{
	eMarchingCube,
	eParticle,
	eBillPartilce,
	eRender,
};

class SPHSystem
{
private:
	float m_unitScale;
	float m_viscosity;
	float m_restDensity; 
	float m_pointMass;
	float m_smoothRadius;
	float m_gasConstantK;
	float m_wallLength;
	float m_updateinteval;

	XMFLOAT3 m_gravity;

	SPHBox m_limitBox;

	float m_boundaryCoefficient;
	float m_boundaryDampening ;

	float m_kernelPoly6;
	float m_kernelSpiky;
	float m_kernelViscosity;


	int m_iParticleMaxNumber;

	bool m_isBarrierActive;
	RenderState m_SPHRenderState;

public:
	//Particle m_ParticlePool[Particle_maxnumber];
	vector<Particle> m_ParticlePool;

	SPHSystem();
	~SPHSystem(){};

	int m_iParticleCount;
	void SPH_DensityPressure(CubeCell & grids);
	void SPH_AccelerationFigure();
	void SPH_ParticleUpdate(const float deltatime);
	void SPH_CreateParticlePool();
	void SPH_Reset();
	void SPH_Init();

	int ParticleMaxNumber() const { return m_iParticleMaxNumber; }
	float SmoothRadius() const { return m_smoothRadius; }
	float PointMass() const { return m_pointMass; }
	float UnitScale() const { return m_unitScale; }
	float Viscosity() const { return m_viscosity; }
	float RestDensity() const { return m_restDensity; }
	float GasConstantK() const { return m_gasConstantK; }
	float Gravity() const { return m_gravity.y; }
	float WallLength() const { return m_wallLength; }
	float BoundaryCoefficient() const { return m_boundaryCoefficient; }
	float BoundaryDampening() const { return m_boundaryDampening; }
	float KernelPoly6() const { return m_kernelPoly6; }
	float Updateinteval() const { return m_updateinteval; }
	bool BarrierActive() const { return m_isBarrierActive; }
	RenderState SPHRenderState() const { return m_SPHRenderState; }

	void SPHRenderState(RenderState val) { m_SPHRenderState = val; }
	void Updateinteval(float val) { m_updateinteval = val; }
	void ParticleMaxNumber(int val) { m_iParticleMaxNumber = val; }
	void UnitScale(float val) { m_unitScale = val; }
	void Viscosity(float val) { m_viscosity = val; }
	void RestDensity(float val) { m_restDensity = val; }
	void PointMass(float val) { m_pointMass = val; }
	void SmoothRadius(float val) { m_smoothRadius = val; }
	void Gravity(float val) { m_gravity = XMFLOAT3(0,val,0); }
	void BoundaryCoefficient(float val) { m_boundaryCoefficient = val; }
	void BoundaryDampening(float val) { m_boundaryDampening = val; }
	void GasConstantK(float val) { m_gasConstantK = val; }
	void WallLength(float val) { m_wallLength = val; m_limitBox = SPHBox(-m_wallLength ,0 ,-m_wallLength ,m_wallLength ,2 * m_wallLength ,m_wallLength); }
	void AddBarrier(){m_isBarrierActive = true;}
	void DeleteBarrier(){m_isBarrierActive = false;}

	void BounceFuction(float fbounceParameter,float boundaryCoefficient,float boundaryDampening,XMFLOAT3 vec,Particle & particle,float boundarylimit = 0,bool isbarrier = false);
	void LimitBoxBounce(Particle & particle);
	void BarrierBoxBounce(Particle & particle);
	void ParticlePosAndSpeedUpdate(Particle & particle,float deltatime);

	void UpdateScene(CubeCell & cubeCells,float deltatime);
};




#endif