#include"CubeCell.h"

void CubeCell::InitCubeCells(SPHBox spacebox,float particleradius)
{
	m_spacebox = spacebox;

	m_iXAxisCubeNumber = (int)((spacebox.max_x - spacebox.min_x) / ( 2 * particleradius));
	m_iYAxisCubeNumber = (int)((spacebox.max_y - spacebox.min_y) / ( 2 * particleradius));
	m_iZAxisCubeNumber = (int)((spacebox.max_z - spacebox.min_z) / ( 2 * particleradius));

	m_cubeNumber =  m_iXAxisCubeNumber * m_iYAxisCubeNumber * m_iZAxisCubeNumber;

	m_cubeLength = 2 * particleradius; // 边长为粒子核半径 * 2


	//InitAllVertexPos();

	//m_girdunit.resize(m_gridscount);


}


void CubeCell::ClearPool()
{
	//更新单元格中的信息时先清空
	for(int i = 0; i < m_cubeNumber; i++ )
	{
		if(!m_cubeCellParticleList[i].empty())
			m_cubeCellParticleList[i].clear();
	}
}

bool CubeCell::InsertIntoCubeCell(SPHSystem & sphsystem)
{
	for(int i = 0; i < sphsystem.m_iParticleCount; i++)
	{
		float pos_x = sphsystem.m_ParticlePool[i].PosRealScale().x;
		float pos_y = sphsystem.m_ParticlePool[i].PosRealScale().y;
		float pos_z = sphsystem.m_ParticlePool[i].PosRealScale().z;

		int index = findCell(pos_x,pos_y,pos_z);

		//if(-1 == index)
		//	continue;

		if(index < 0 || index >= m_cubeNumber)
		{
			MessageBoxA(0,"Particle exceed the border. Return to Default state automatically",0,0);
			return false;
		}
		m_cubeCellParticleList[index].push_back(i); // 将粒子插入单元格对应的粒子链表

	}
	return true;
}

void CubeCell::ErrorRecover(bool insertresult)
{
	if(!insertresult)
	{
		SPH_UIControl::ReturnToDefault();
	}
}


void CubeCell::InsertParticles(SPHSystem & sphsystem)
{
	ClearPool();

	bool  insertresult = InsertIntoCubeCell(sphsystem);

	ErrorRecover(insertresult);
// 	for(int i = 0; i < sphsystem.m_iParticleCount; i++)
// 	{
// 		float pos_x = sphsystem.m_ParticlePool[i].m_vPosRealScale.x;
// 		float pos_y = sphsystem.m_ParticlePool[i].m_vPosRealScale.y;
// 		float pos_z = sphsystem.m_ParticlePool[i].m_vPosRealScale.z;
// 
// 		int index = findCell(pos_x,pos_y,pos_z);
// 
// 		//if(-1 == index)
// 		//	continue;
// 
// 		if(index < 0 || index >= m_gridscount)
// 		{
// 			MessageBoxA(0,"Particle exceed the border. Return to Default state automatically",0,0);
// 			 insertresult = false;
// 			break;
// 		}
// 		m_girdunit[index].push_back(i); // 将粒子插入grid对应的链表
// 
// 	}




}

int CubeCell::findCell(XMFLOAT3 & vPos)
{
    float x = vPos.x;
	float y = vPos.y;
	float z = vPos.z;
	 
	if(x < m_spacebox.min_x || x > m_spacebox.max_x || y < m_spacebox.min_y || y > m_spacebox.max_y || z < m_spacebox.min_z || z > m_spacebox.max_z)
	{
	//	MessageBoxA(0,"Over Grid",0,0);
		//return -1;
	}


	int index_x,index_y,index_z;

	index_x = (int)((x - m_spacebox.min_x) / m_cubeLength);
	index_y = (int)((y - m_spacebox.min_y) / m_cubeLength);
	index_z = (int)((z - m_spacebox.min_z) / m_cubeLength);

	int index = index_z * m_iYAxisCubeNumber * m_iXAxisCubeNumber + index_y * m_iXAxisCubeNumber + index_x;

	return index;

}

int CubeCell::findCell(float x, float y, float z)
{
	if(x < m_spacebox.min_x || x > m_spacebox.max_x || y < m_spacebox.min_y || y > m_spacebox.max_y || z < m_spacebox.min_z || z > m_spacebox.max_z)
	{
	//	MessageBoxA(0,"Over Grid",0,0);
	//	return -1;
	}

	int index_x,index_y,index_z;

	index_x = (int)((x - m_spacebox.min_x) / m_cubeLength);
	index_y = (int)((y - m_spacebox.min_y) / m_cubeLength);
	index_z = (int)((z - m_spacebox.min_z) / m_cubeLength);

	int index = index_z * m_iYAxisCubeNumber * m_iXAxisCubeNumber + index_y * m_iXAxisCubeNumber + index_x;

	return index;
}

void CubeCell::findNeighborCells(XMFLOAT3 & vPos,int * neighborcells)
{
	float x = vPos.x;
	float y = vPos.y;
	float z = vPos.z;

	int index = findCell(vPos);

	XMFLOAT3 neighborPos[8];
	neighborPos[0] = XMFLOAT3(x - m_cubeLength/2.0f,y - m_cubeLength/2.0f,z - m_cubeLength/2.0f);
	neighborPos[1] = XMFLOAT3(x - m_cubeLength/2.0f,y - m_cubeLength/2.0f,z + m_cubeLength/2.0f);
	neighborPos[2] = XMFLOAT3(x - m_cubeLength/2.0f,y + m_cubeLength/2.0f,z - m_cubeLength/2.0f);
	neighborPos[3] = XMFLOAT3(x - m_cubeLength/2.0f,y + m_cubeLength/2.0f,z + m_cubeLength/2.0f);
	neighborPos[4] = XMFLOAT3(x + m_cubeLength/2.0f,y - m_cubeLength/2.0f,z - m_cubeLength/2.0f);
	neighborPos[5] = XMFLOAT3(x + m_cubeLength/2.0f,y - m_cubeLength/2.0f,z + m_cubeLength/2.0f);
	neighborPos[6] = XMFLOAT3(x + m_cubeLength/2.0f,y + m_cubeLength/2.0f,z - m_cubeLength/2.0f);
	neighborPos[7] = XMFLOAT3(x + m_cubeLength/2.0f,y + m_cubeLength/2.0f,z + m_cubeLength/2.0f);


	neighborcells[0] = index;
	for(int i = 1;i < 9; i++)
	{
		neighborcells[i] = findCell(neighborPos[i - 1]);

		// 有一定概率重复
		for(int j = 0; j < i; j++)
			if(neighborcells[i] == neighborcells[j])
				neighborcells[i] = -1;
	}

}

CubeCell::~CubeCell()
{

}
