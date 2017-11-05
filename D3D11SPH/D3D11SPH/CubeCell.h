#ifndef GRID_H_
#define GRID_H_

#include "SPH_Math.h"
#include <vector>
#include"SPH_System.h"
#include "SPH_MarchingCube.h"
#include "SPH_UI.h"

using namespace std;

class SPH_UIControl;
//互相包含，前置声明
class SPHSystem;

class CubeCell{
private:
	float   m_cubeLength;
	int     m_cubeNumber;
	SPHBox  m_spacebox;
	int m_iXAxisCubeNumber;
	int m_iYAxisCubeNumber;
	int m_iZAxisCubeNumber;
	
public:

	vector<int> m_cubeCellParticleList[5000];

	void InitCubeCells(SPHBox spacebox,float particleradius);

	void InsertParticles(SPHSystem & sphsystem);
	int  findCell(XMFLOAT3 & vPos);
	int  findCell(float x, float y, float z);
	void findNeighborCells(XMFLOAT3 & vPos,int * neighborcells);

	void ClearPool();
	bool InsertIntoCubeCell(SPHSystem & sphsystem);

	void ErrorRecover(bool insertresult); //出现错误时回归初始状态

	friend class MarchingCube;

	~CubeCell();

};

#endif