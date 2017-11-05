#include"VertexIndexControl.h"


//ID3D11Buffer * VertexIndexControl::m_VBSkull = 0;

ID3D11Buffer * VertexIndexControl::m_VB = 0;

ID3D11Buffer * VertexIndexControl::m_IBLimitedBox = 0;

ID3D11Buffer * VertexIndexControl::m_VBLimitedBox = 0;

ID3D11Buffer * VertexIndexControl::m_IBBarrier = 0;

ID3D11Buffer * VertexIndexControl::m_VBBarrier = 0;

ID3D11Buffer * VertexIndexControl::m_IBSkyBox = 0;

ID3D11Buffer * VertexIndexControl::m_VBSkyBox = 0;

ID3D11Buffer * VertexIndexControl::m_VBTestBox = 0;
//ID3D11Buffer * VertexIndexControl::m_VBox = 0;

//ID3D11Buffer * VertexIndexControl::m_IB = 0;

UINT VertexIndexControl::m_iIndexCount = 0;

ID3D11InputLayout* VertexIndexControl::m_InputLayout = 0;


void VertexIndexControl::VertexIndexInit(ID3D11Device * pd3dDevice)
{

	XMFLOAT2 Size(0.75,0.75);
	Vertex v;
	v = Vertex(0, 0, 0,0,0,0,0,0, Size.x,Size.y);
//     for(int i = 0; i < Particle_number; i++)
// 	{
// 		v[i] = Vertex(0, 0, 0, 0,0, Size.x,Size.y);
// 	}

	D3D11_BUFFER_DESC mVertexBufferDesc;
	mVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	mVertexBufferDesc.MiscFlags = 0;
	mVertexBufferDesc.CPUAccessFlags = 0;
	mVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	mVertexBufferDesc.ByteWidth = sizeof(Vertex);
	mVertexBufferDesc.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA mVertexSubReData;
	mVertexSubReData.pSysMem = &v;

	pd3dDevice->CreateBuffer(&mVertexBufferDesc,&mVertexSubReData,&m_VB);

	
	float length = SkyBoxRange ;


	Vertex vBox[8] = {
		Vertex(-length, -length , -length,0,0,0,0,0,0,0),
		Vertex(-length, +length , -length,0,0,0,0,0,0,0),
		Vertex(+length, +length , -length,0,0,0,0,0,0,0),
		Vertex(+length, -length , -length,0,0,0,0,0,0,0),
		Vertex(-length,  -length, +length,0,0,0,0,0,0,0),
		Vertex(-length,  +length, +length,0,0,0,0,0,0,0),
		Vertex(+length,  +length, +length,0,0,0,0,0,0,0),
		Vertex(+length,  -length, +length,0,0,0,0,0,0,0),
	};  

	D3D11_BUFFER_DESC mVertexBoxBufferDesc;
	mVertexBoxBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	mVertexBoxBufferDesc.MiscFlags = 0;
	mVertexBoxBufferDesc.CPUAccessFlags = 0;
	mVertexBoxBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	mVertexBoxBufferDesc.ByteWidth = sizeof(Vertex) * 8;
	mVertexBoxBufferDesc.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA mVertexBoxSubReData;
	mVertexBoxSubReData.pSysMem = vBox;

	pd3dDevice->CreateBuffer(&mVertexBoxBufferDesc,&mVertexBoxSubReData,&m_VBSkyBox);

	length = 10 ;
	float dheight = 30;

	Vertex vTestBox[8] = {
		Vertex(-length, -length + dheight , -length,-1,-1,-1,0,0,0,0),
		Vertex(-length, +length + dheight, -length,-1,+1,-1,0,0,0,0),
		Vertex(+length, +length + dheight, -length,+1,+1,-1,0,0,0,0),
		Vertex(+length, -length + dheight, -length,+1,-1,-1,0,0,0,0),
		Vertex(-length,  -length + dheight, +length,-1,-1,+1,0,0,0,0),
		Vertex(-length,  +length + dheight, +length,-1,+1,+1,0,0,0,0),
		Vertex(+length,  +length + dheight, +length,+1,+1,+1,0,0,0,0),
		Vertex(+length,  -length + dheight, +length,+1,-1,+1,0,0,0,0),
	};  

	D3D11_BUFFER_DESC mVertexTestBoxBufferDesc;
	mVertexTestBoxBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	mVertexTestBoxBufferDesc.MiscFlags = 0;
	mVertexTestBoxBufferDesc.CPUAccessFlags = 0;
	mVertexTestBoxBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	mVertexTestBoxBufferDesc.ByteWidth = sizeof(Vertex) * 8;
	mVertexTestBoxBufferDesc.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA mVertexTestBoxSubReData;
	mVertexTestBoxSubReData.pSysMem = vTestBox;

	pd3dDevice->CreateBuffer(&mVertexTestBoxBufferDesc,&mVertexTestBoxSubReData,&m_VBTestBox);

	//Index
	UINT Indics[] = {

		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3, 
		4, 3, 7

	};
	// 
	m_iIndexCount = 36;

	D3D11_BUFFER_DESC mIndexBufferDesc;
	mIndexBufferDesc.Usage =  D3D11_USAGE_IMMUTABLE;
	mIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	mIndexBufferDesc.ByteWidth = sizeof(UINT) * m_iIndexCount;
	mIndexBufferDesc.CPUAccessFlags = 0;
	mIndexBufferDesc.MiscFlags = 0;
	mIndexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA mIndexSubReData;
	mIndexSubReData.pSysMem = &Indics[0];


	pd3dDevice->CreateBuffer(&mIndexBufferDesc,&mIndexSubReData,&m_IBSkyBox);


}

void VertexIndexControl::Release()
{
	RELEASE_COM(m_VB);
	//RELEASE_COM(m_IB);
	//RELEASE_COM(m_VBSkull);
	RELEASE_COM(m_InputLayout);
}

HRESULT VertexIndexControl::SetLayout(ID3D11Device * pd3dDevice)
{
	//ÉèÖÃLayout
	D3D11_INPUT_ELEMENT_DESC d3dInputDesc[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"SIZE",0,DXGI_FORMAT_R32G32_FLOAT,0,32,D3D11_INPUT_PER_VERTEX_DATA,0},


	};

	D3DX11_PASS_DESC passDesc;
	FXControl::m_fxTech->GetPassByIndex(0)->GetDesc(&passDesc);
	//FXControl::m_fxBillBoardTech->GetPassByIndex(0)->GetDesc(&passDesc);


	HRESULT hr = pd3dDevice->CreateInputLayout(d3dInputDesc,4,passDesc.pIAInputSignature,passDesc.IAInputSignatureSize,&m_InputLayout);

	if(FAILED(hr))
	{
		MessageBoxA(0,"Fail to Create Input Layout",0,0);
		return E_FAIL;
	}

	return S_OK;
}

void VertexIndexControl::SPHWallInit(ID3D11Device * pd3dDevice,SPHSystem & sphsystem)
{

	float WallLength = sphsystem.WallLength();
	float SPHUnitScale = sphsystem.UnitScale();

	float length = (WallLength - WallThickness / 2.0f) / ( SPHUnitScale) ;


	Vertex vBox[8] = {
		Vertex(-length, WallThickness / (2.0f * SPHUnitScale) , -length,0,0,0,0,0,0,0),
		Vertex(-length, WallThickness / (2.0f * SPHUnitScale) , +length,0,0,0,0,0,0,0),
		Vertex(+length, WallThickness / (2.0f * SPHUnitScale) , +length,0,0,0,0,0,0,0),
		Vertex(+length, WallThickness / (2.0f * SPHUnitScale) , -length,0,0,0,0,0,0,0),
		Vertex(-length, 2 * length, -length,0,0,0,0,0,0,0),
		Vertex(-length, 2 * length, +length,0,0,0,0,0,0,0),
		Vertex(+length, 2 * length, +length,0,0,0,0,0,0,0),
		Vertex(+length, 2 * length, -length,0,0,0,0,0,0,0),
	};  

	D3D11_BUFFER_DESC mVertexBoxBufferDesc;
	mVertexBoxBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	mVertexBoxBufferDesc.MiscFlags = 0;
	mVertexBoxBufferDesc.CPUAccessFlags = 0;
	mVertexBoxBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	mVertexBoxBufferDesc.ByteWidth = sizeof(Vertex) * 8;
	mVertexBoxBufferDesc.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA mVertexBoxSubReData;
	mVertexBoxSubReData.pSysMem = vBox;

	pd3dDevice->CreateBuffer(&mVertexBoxBufferDesc,&mVertexBoxSubReData,&m_VBLimitedBox);

	UINT Indics[] = {
		0, 1,
		1, 2,
		2, 3,
		3, 0,
		4, 5,
		5, 6,
		6, 7,
		7, 4,
		0, 4,
		1, 5,
		2, 6,
		3, 7,

	};
	// 
	m_iIndexCount = 24;

	D3D11_BUFFER_DESC mIndexBufferDesc;
	mIndexBufferDesc.Usage =  D3D11_USAGE_IMMUTABLE;
	mIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	mIndexBufferDesc.ByteWidth = sizeof(UINT) * m_iIndexCount;
	mIndexBufferDesc.CPUAccessFlags = 0;
	mIndexBufferDesc.MiscFlags = 0;
	mIndexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA mIndexSubReData;
	mIndexSubReData.pSysMem = &Indics[0];


	pd3dDevice->CreateBuffer(&mIndexBufferDesc,&mIndexSubReData,&m_IBLimitedBox);


	float length_barrier = (WallLength - WallThickness * 1.5f ) / ( 3 * SPHUnitScale) ;


	Vertex vBox_barrier[8] = {
		Vertex(-length_barrier, WallThickness / (2.0f * SPHUnitScale) , -length_barrier,0,0,0,0,0,0,0),
		Vertex(-length_barrier, WallThickness / (2.0f * SPHUnitScale) , +length_barrier,0,0,0,0,0,0,0),
		Vertex(+length_barrier, WallThickness / (2.0f * SPHUnitScale) , +length_barrier,0,0,0,0,0,0,0),
		Vertex(+length_barrier, WallThickness / (2.0f * SPHUnitScale) , -length_barrier,0,0,0,0,0,0,0),
		Vertex(-length_barrier,  length_barrier, -length_barrier,0,0,0,0,0,0,0),
		Vertex(-length_barrier,  length_barrier, +length_barrier,0,0,0,0,0,0,0),
		Vertex(+length_barrier,  length_barrier, +length_barrier,0,0,0,0,0,0,0),
		Vertex(+length_barrier,  length_barrier, -length_barrier,0,0,0,0,0,0,0),
	};  

	D3D11_BUFFER_DESC mVertexBarrierBufferDesc;
	mVertexBarrierBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	mVertexBarrierBufferDesc.MiscFlags = 0;
	mVertexBarrierBufferDesc.CPUAccessFlags = 0;
	mVertexBarrierBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	mVertexBarrierBufferDesc.ByteWidth = sizeof(Vertex) * 8;
	mVertexBarrierBufferDesc.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA mVertexBarrierSubReData;
	mVertexBarrierSubReData.pSysMem = vBox_barrier;

	pd3dDevice->CreateBuffer(&mVertexBarrierBufferDesc,&mVertexBarrierSubReData,&m_VBBarrier);

	UINT Indics_barrier[] = {
		0, 1,
		1, 2,
		2, 3,
		3, 0,
		4, 5,
		5, 6,
		6, 7,
		7, 4,
		0, 4,
		1, 5,
		2, 6,
		3, 7,

	};
	// 
	m_iIndexCount = 24;

	D3D11_BUFFER_DESC mIndexBarrierBufferDesc;
	mIndexBarrierBufferDesc.Usage =  D3D11_USAGE_IMMUTABLE;
	mIndexBarrierBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	mIndexBarrierBufferDesc.ByteWidth = sizeof(UINT) * m_iIndexCount;
	mIndexBarrierBufferDesc.CPUAccessFlags = 0;
	mIndexBarrierBufferDesc.MiscFlags = 0;
	mIndexBarrierBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA mIndexBarrierSubReData;
	mIndexBarrierSubReData.pSysMem = &Indics_barrier[0];


	pd3dDevice->CreateBuffer(&mIndexBarrierBufferDesc,&mIndexBarrierSubReData,&m_IBBarrier);

}











