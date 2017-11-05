#ifndef SPH_MARCHING_CUBE_H_
#define SPH_MARCHING_CUBE_H_

#include "SPH_Math.h"
#include "SPH_System.h" 
#include<exception>
using namespace std;
//1.Figure out density of every vertex
//2.Compare with the threshold value p =  1050 this is only a try
//

class CubeCell;
class SPHSystem;

class MarchingCube{

	SPHBox   m_spacebox;

	int      m_iXAxisGridNumber;
	int      m_iYAxisGridNumber;
	int      m_iZAxisGridNumber;

	float    m_Cubelength;
	int      m_CubeCellsNumber;

	float    m_thresholdvalue;
	float    m_CubeLength;
	float    m_CubeRange;

	bool     isRender;
	vector<SPHCube> m_Cubes; //每个单元格对应的顶点

	


	void InterpolateEdgeAndNormal(int * edgeTable,XMFLOAT3 * edgepos,XMFLOAT3 * normal,float isovalue,int cubeindex,int shapeindex,int edgeposindex,int vertexindex_1,int vertexindex_2);

public:

	vector<SPHTriangle> m_trianglePos;

	void Polygonise(CubeCell & cubeCells,float isovalue,SPHSystem & sphSystem);


	bool GridEmptyTest(CubeCell & cubeCells,int Cellindex);//单元格是否为空的检测
	void GridValueFigure(CubeCell & cubeCells,SPHSystem & sphSystem);

	XMFLOAT3 Interpolation(float isovalue,XMFLOAT3 p1,XMFLOAT3 p2,float v1,float v2);

	float Thresholdvalue() const { return m_thresholdvalue; }
	void  Thresholdvalue(float val) { m_thresholdvalue = val; }

	float CubeLength() const { return m_CubeLength; }
	void  CubeLength(float val);

	float CubeRange() const { return m_CubeRange; }
	void  CubeRange(float val);

	bool IsRender() const { return isRender; }
	void IsRender(bool val) { isRender = val; }

	void Reset()
	{
		if(!m_trianglePos.empty())
			m_trianglePos.clear();

	};


	void     SetMarchingCube(SPHBox limitbox, float length);
	void     Init();
	void     SetDefaultValue();
	void     CreateCube();

	bool     ShapeSelect(int index,float isovalue,int & shapeindex);
	void     CreateTriangleVertex(int index,SPHSystem & sphSystem,XMFLOAT3 * edgepos,XMFLOAT3 * normal,int shapeindex);
	
	XMFLOAT3 figureCellPos(int index);

	void     FigureCellVertexPos(int index,XMFLOAT3 * VertexPos);

	void     InitAllVertexPos();

	int      CubeCellscount() const { return m_CubeCellsNumber; }

	void UpdateScene(CubeCell & cubeCells,float thresholdvalue,SPHSystem & sphSystem);
};



#endif