#ifndef VERTEXCONTROL_H_
#define VERTEXCONTROL_H_
#include"MyD3D11app.h"
#include"FXControl.h"
#include"SPH_System.h"
#include"SPH_MarchingCube.h"

#include<vector>
#include<fstream>
#include<string>

const float SkyBoxRange = 300;
struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Tex;
	XMFLOAT2 Size;

	Vertex(
		float px, float py, float pz, 
		float nx, float ny, float nz,
		float u, float v,
		float sx,float sy)
		: Pos(px,py,pz),
		Normal(nx,ny,nz),
		Tex(u,v),
	 Size(sx,sy){}
	Vertex(){}
};

namespace Colors{
	XMGLOBALCONST XMVECTORF32 White = {1.0,1.0,1.0,1.0};
	XMGLOBALCONST XMVECTORF32 Blue = {0,0,1.0,1.0};
	XMGLOBALCONST XMVECTORF32 Red = {1.0,0,0,1.0};
	XMGLOBALCONST XMVECTORF32 LightRed = {0.5f,0,0,1.0f};
	XMGLOBALCONST XMVECTORF32 LightBlue = {0.25f,0.25f,1.0f,1.0f};
}

class VertexIndexControl{

private:
		void Release();
public:
	VertexIndexControl(){}
	~VertexIndexControl(){Release();}


	static void VertexIndexInit(ID3D11Device * pd3dDevice);
	static void SPHWallInit(ID3D11Device * pd3dDevice,SPHSystem & sphsystem);
	static HRESULT SetLayout(ID3D11Device * pd3dDevice);

	//static ID3D11Buffer * m_IB;
	static ID3D11Buffer * m_VB;
	static ID3D11Buffer * m_VBLimitedBox;
	static ID3D11Buffer * m_IBLimitedBox;

	static ID3D11Buffer * m_VBBarrier;
	static ID3D11Buffer * m_IBBarrier;

	static ID3D11Buffer * m_VBSkyBox;
	static ID3D11Buffer * m_IBSkyBox;

	static ID3D11Buffer * m_VBTestBox;

	static UINT m_iIndexCount;
	static ID3D11InputLayout* m_InputLayout;
};
#endif