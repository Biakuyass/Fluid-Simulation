#include "RenderObject.h"
#include<Errors.h>
#include<Strsafe.h>
#include<comdef.h>
HRESULT RenderObject::ObjectInit()
{
	//VertexIndexInit();
	//m_RenderState = eBillPartilce;
	m_framecount = 0;
	VertexIndexControl::VertexIndexInit(m_pd3dDevice);

	HRESULT hr;

	hr = FXControl::Init(m_pd3dDevice);
	if(FAILED(hr))
		return E_FAIL;


	hr = VertexIndexControl::SetLayout(m_pd3dDevice);
	if(FAILED(hr))
		return E_FAIL;


	hr = StateControl::InitAllState(m_pd3dDevice);
	if(FAILED(hr))
		return E_FAIL;

	hr = D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice,L"Textures/LobbyCube.dds",0,0,&m_CubeMapSRV,0);
	//LobbyCube.dds
	if(FAILED(hr))
	{
		MessageBoxA(0,"Load Texture Failed",0,0);
		return E_FAIL;
	} 
// 	// Read Texture From File
// 	 hr = D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice,L"Textures/brick01.dds",0,0,&m_d3d11SRVFloor,0);
// 
// 	if(FAILED(hr))
// 	{
// 		MessageBoxA(0,"Load Texture Failed",0,0);
// 		return E_FAIL;
// 	}
// 
// 	hr = D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice,L"Textures/checkboard.dds",0,0,&m_d3d11SRVWall,0);
// 
// 	if(FAILED(hr))
// 	{
// 		MessageBoxA(0,"Load Texture Failed",0,0);
// 		return E_FAIL;
// 	}
// 
// 
// 	hr = D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice,L"Textures/ice.dds",0,0,&m_d3d11SRVMirror,0);
// 
// 	if(FAILED(hr))
// 	{
// 		MessageBoxA(0,"Load Texture Failed",0,0);
// 		return E_FAIL;
// 	}

	hr = D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice,L"Textures/water.png",0,0,&m_d3d11SRVWater,0);

	if(FAILED(hr))
	{
		MessageBoxA(0,"Load Texture Failed",0,0);
		return E_FAIL;
	}

	

	//Init SPHSystem
	m_SPHSystem.SPH_CreateParticlePool();
	//Init Cells
	SPHBox cubespace(-spacerange,-spacerange,-spacerange,spacerange, spacerange,spacerange);
	m_CubeCells.InitCubeCells(cubespace,m_SPHSystem.SmoothRadius());

	m_MarchingCube.Init();

	SPH_UIControl::Init(m_pd3dDevice,m_pd3dDevContext,m_SPHSystem,m_MarchingCube);


	VertexIndexControl::SPHWallInit(m_pd3dDevice,m_SPHSystem);

	return S_OK;



}


void RenderObject::Release()
{
	MyD3D11app::Release();

}

RenderObject::RenderObject():MyD3D11app()
{
	//MyD3D11app::MyD3D11app();

	m_fRadius = 100;

	m_CameraPosx = 0;
	m_CameraPosy = 0;
	m_CameraPosz = -m_fRadius;

	m_MousePosLastx = 0;
	m_MousePosLasty = 0;


	m_Phi = 0;
	m_Thea = 0;

	XMMATRIX IdenticalMatrix = XMMatrixIdentity();
	m_matWorldTranslate = IdenticalMatrix;
	m_matWorldRotate = IdenticalMatrix;
	m_matWorldScailing = IdenticalMatrix;

	m_v3WorldCoordinate = XMFLOAT3(0,0,0);
	m_v3Rotation = XMFLOAT3(0,0,0);
	m_v3Scailing = XMFLOAT3(1,1,1);

	m_vLightPos = XMFLOAT3(0,100,-10);

// 	m_v3WorldCoordinate_Skull = XMFLOAT3(0,0,-4);
// 	m_v3Rotation_Skull = XMFLOAT3(0,90,0);
// 	m_v3Scailing_Skull = XMFLOAT3(0.5,0.5,0.5);

}

RenderObject::~RenderObject()
{
	Release();

}

int RenderObject::Init(HINSTANCE hInstance,int nShowCmd)
{
	if(-1 == MyD3D11app::Init(hInstance,nShowCmd))
		return -1;
	ObjectInit();

	return 0;

}

void RenderObject::UpdateScene(const float dt)
{
//matWorld
	m_matWorldTranslate = XMMatrixTranslation(m_v3WorldCoordinate.x,m_v3WorldCoordinate.y,m_v3WorldCoordinate.z);
	m_matWorldRotate =  XMMatrixRotationX(m_v3Rotation.x) * XMMatrixRotationY(m_v3Rotation.y);
	m_matWorldScailing = XMMatrixScaling(m_v3Scailing.x,m_v3Scailing.y,m_v3Scailing.z);



//matView
	if(m_Phi > PI - 0.1f)
		m_Phi = PI - 0.1f;
	if(m_Phi < 0.1f)
		m_Phi = 0.1f;

	float x = m_fRadius*sinf(m_Phi)*cosf(m_Thea);
	float z = m_fRadius*sinf(m_Phi)*sinf(m_Thea);
	float y = m_fRadius*cosf(m_Phi);

	m_CameraPosx = x;
	m_CameraPosy = y;
	m_CameraPosz = z;

	XMVECTOR pos = XMVectorSet(m_CameraPosx,m_CameraPosy,m_CameraPosz,1.0);
	XMVECTOR target = XMVectorSet(0,0,0,1.0);
	XMVECTOR up = XMVectorSet(0,1.0f,0,1.0);

	m_matView = XMMatrixLookAtLH(pos,target,up);

	//切线空间
	XMMATRIX matWorldInverseTranspose;
	matWorldInverseTranspose = MatrixInverseTranspose(m_matWorld);



//matWorldViewProj
	//m_matWorldViewProj = m_matWorld * m_matView * m_matProj;


	//摄像机位置
	XMFLOAT3 EyePos(m_CameraPosx,m_CameraPosy,m_CameraPosz);
	//平行光方向
	XMFLOAT3 LightDirection(m_v3WorldCoordinate.x - m_vLightPos.x, m_v3WorldCoordinate.y - m_vLightPos.y, m_v3WorldCoordinate.z - m_vLightPos.z);


	FXControl::m_fxEyePos->SetRawValue((float*)&EyePos,0,sizeof(XMFLOAT3));
	FXControl::m_fxLightDir->SetRawValue((float*)&LightDirection,0,sizeof(LightDirection));



	m_CubeCells.InsertParticles(m_SPHSystem);
	m_SPHSystem.UpdateScene(m_CubeCells,dt);
// 	m_SPHSystem.SPH_DensityPressure(m_CubeCells);
// 	m_SPHSystem.SPH_AccelerationFigure();
// 	m_SPHSystem.SPH_ParticleUpdate(dt);


	if(eMarchingCube == m_SPHSystem.SPHRenderState() || eRender ==m_SPHSystem.SPHRenderState())
	{
		m_MarchingCube.UpdateScene(m_CubeCells,m_MarchingCube.Thresholdvalue(),m_SPHSystem);
// 	m_MarchingCube.GridValueFigure(m_CubeCells,m_SPHSystem);
// 	m_MarchingCube.Polygonise(m_CubeCells,m_MarchingCube.Thresholdvalue(),m_SPHSystem);
	}

	FrameCount(dt);
    SPH_UIControl::Update(m_framecount);

	float fskybox = SkyBoxRange;
	FXControl::m_fxSkyBoxRange->SetRawValue(&fskybox,0,sizeof(float));

}

HRESULT RenderObject::Resize(float window_width,float window_height)
{
	if(FAILED(MyD3D11app::Resize(window_width,window_height)))
		return E_FAIL;

	m_matProj = XMMatrixPerspectiveFovLH(PI/4.0f,(float)window_width/(float)window_height,1.0f,1000.0f);

	return S_OK;
}

void RenderObject::DrawScene()
{
	//Start to Draw
	m_pd3dDevContext->ClearRenderTargetView(m_pRenderTargetView,(const float*)&Colors::LightBlue);
	m_pd3dDevContext->ClearDepthStencilView(m_pDepthStencilView,D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,1.0,0);

	m_pd3dDevContext->IASetInputLayout(VertexIndexControl::m_InputLayout);
	m_pd3dDevContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
//Common Variable
	UINT iVertexStrides = sizeof(Vertex);
	UINT ioffset = 0;
	float fBlendFactors[] = {0,0,0,0};
	XMMATRIX matWorldInverseTranspose;
//Technique
	D3DX11_TECHNIQUE_DESC techDesc;
	FXControl::m_fxTech->GetDesc(&techDesc);

	D3DX11_TECHNIQUE_DESC billtechDesc;
	FXControl::m_fxBillBoardTech->GetDesc(&billtechDesc);

	D3DX11_TECHNIQUE_DESC skytechDesc;
	FXControl::m_fxSkyBox->GetDesc(&skytechDesc);

	D3DX11_TECHNIQUE_DESC WireFrametechDesc;
	FXControl::m_fxWireFrame->GetDesc(&WireFrametechDesc);

//Different Object Render
// Set Buffer

	if(eBillPartilce == m_SPHSystem.SPHRenderState() || eParticle == m_SPHSystem.SPHRenderState())
	{
		m_pd3dDevContext->IASetVertexBuffers(0,1,&VertexIndexControl::m_VB,&iVertexStrides,&ioffset);
		//m_pd3dDevContext->IASetIndexBuffer(VertexIndexControl::m_IB,DXGI_FORMAT_R32_UINT,0);
		// Render 500 Particle
		m_pd3dDevContext->OMSetBlendState(StateControl::m_TransparentBS,fBlendFactors,0xffffffff);

		for(int i = 0;i < m_SPHSystem.m_iParticleCount; i++)
		{
			//Set State
			m_pd3dDevContext->RSSetState(StateControl::m_NoBackSolid);
			//Set Matrix
			m_matWorld = XMMatrixTranslation(m_SPHSystem.m_ParticlePool[i].PosRender().x,m_SPHSystem.m_ParticlePool[i].PosRender().y,m_SPHSystem.m_ParticlePool[i].PosRender().z);
			matWorldInverseTranspose = MatrixInverseTranspose(m_matWorld);
			m_matWorldViewProj = m_matWorld * m_matView * m_matProj;
			//Set FX Variable
			FXControl::m_fxMatWorldViewProj->SetMatrix((float*)&m_matWorldViewProj);
			FXControl::m_fxMatWorld->SetMatrix((float*)&m_matWorld);
			FXControl::m_fxMatWorldInverseTranspose->SetMatrix((float*)&matWorldInverseTranspose);

			//Particle
			//Set Texture
			FXControl::m_fxDiffuseMap->SetResource(m_d3d11SRVWater);

			if( eParticle == m_SPHSystem.SPHRenderState())
				for(int p = 0; p < WireFrametechDesc.Passes; p++ )
				{
					m_pd3dDevContext->OMSetBlendState(0,fBlendFactors,0xffffffff);
					FXControl::m_fxWireFrame->GetPassByIndex(p)->Apply(0,m_pd3dDevContext);
					m_pd3dDevContext->Draw(1,0);
				}

				if(eBillPartilce == m_SPHSystem.SPHRenderState())
					for(int p = 0; p < billtechDesc.Passes; p++ )
					{
						FXControl::m_fxBillBoardTech->GetPassByIndex(p)->Apply(0,m_pd3dDevContext);
						m_pd3dDevContext->Draw(1,0);
					}
		}
		m_pd3dDevContext->OMSetBlendState(0,fBlendFactors,0xffffffff);
	}
	//draw box
	m_pd3dDevContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	m_pd3dDevContext->IASetVertexBuffers(0,1,&VertexIndexControl::m_VBLimitedBox,&iVertexStrides,&ioffset);
	m_pd3dDevContext->IASetIndexBuffer(VertexIndexControl::m_IBLimitedBox,DXGI_FORMAT_R32_UINT,0);
	//Set State
	m_pd3dDevContext->RSSetState(StateControl::m_NoBackWireFrame);
		//Set Matrix
		m_matWorld = XMMatrixTranslation(0,0,0);
		matWorldInverseTranspose = MatrixInverseTranspose(m_matWorld);
		m_matWorldViewProj = m_matWorld * m_matView * m_matProj;
		//Set FX Variable
		FXControl::m_fxMatWorldViewProj->SetMatrix((float*)&m_matWorldViewProj);
		FXControl::m_fxMatWorld->SetMatrix((float*)&m_matWorld);
		FXControl::m_fxMatWorldInverseTranspose->SetMatrix((float*)&matWorldInverseTranspose);

		//Particle
		//Set Texture
		//FXControl::m_fxDiffuseMap->SetResource(m_d3d11SRVFloor);
		for(int p = 0; p < techDesc.Passes; p++ )
		{
			FXControl::m_fxTech->GetPassByIndex(p)->Apply(0,m_pd3dDevContext);
			m_pd3dDevContext->DrawIndexed(VertexIndexControl::m_iIndexCount,0,0);
		}
	//drawbarrier
		if(m_SPHSystem.BarrierActive())
		{
			//draw box
			m_pd3dDevContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
			m_pd3dDevContext->IASetVertexBuffers(0,1,&VertexIndexControl::m_VBBarrier,&iVertexStrides,&ioffset);
			m_pd3dDevContext->IASetIndexBuffer(VertexIndexControl::m_IBBarrier,DXGI_FORMAT_R32_UINT,0);
			//Set State
			m_pd3dDevContext->RSSetState(StateControl::m_NoBackWireFrame);
			//Set Matrix
			//matWorldInverseTranspose = MatrixInverseTranspose(m_matWorld);
			m_matWorld = XMMatrixTranslation(0,0,0);
			m_matWorldViewProj = m_matWorld * m_matView * m_matProj;
			//Set FX Variable
			FXControl::m_fxMatWorldViewProj->SetMatrix((float*)&m_matWorldViewProj);
			FXControl::m_fxMatWorld->SetMatrix((float*)&m_matWorld);
			FXControl::m_fxMatWorldInverseTranspose->SetMatrix((float*)&matWorldInverseTranspose);

			for(int p = 0; p < techDesc.Passes; p++ )
			{
				FXControl::m_fxTech->GetPassByIndex(p)->Apply(0,m_pd3dDevContext);
				m_pd3dDevContext->DrawIndexed(VertexIndexControl::m_iIndexCount,0,0);
			}
		}
	//CreateTriangleBuffer();

	if(eRender == m_SPHSystem.SPHRenderState())
	{
		//draw SkyBox
		m_pd3dDevContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_pd3dDevContext->IASetVertexBuffers(0,1,&VertexIndexControl::m_VBSkyBox,&iVertexStrides,&ioffset);
		m_pd3dDevContext->IASetIndexBuffer(VertexIndexControl::m_IBSkyBox,DXGI_FORMAT_R32_UINT,0);
		//Set State
		m_pd3dDevContext->RSSetState(StateControl::m_NoBackSolid);
		//Set Matrix
		m_matWorld = XMMatrixTranslation(0,0,0);
		matWorldInverseTranspose = MatrixInverseTranspose(m_matWorld);
		m_matWorldViewProj = m_matWorld * m_matView * m_matProj;
		//Set FX Variable
		FXControl::m_fxMatWorldViewProj->SetMatrix((float*)&m_matWorldViewProj);
		FXControl::m_fxMatWorld->SetMatrix((float*)&m_matWorld);
		FXControl::m_fxMatWorldInverseTranspose->SetMatrix((float*)&matWorldInverseTranspose);

		//Particle
		//Set Texture
		//FXControl::m_fxDiffuseMap->SetResource(m_d3d11SRVFloor);
		for(int p = 0; p < skytechDesc.Passes; p++ )
		{
			FXControl::m_fxSkyBox->GetPassByIndex(p)->Apply(0,m_pd3dDevContext);
			m_pd3dDevContext->DrawIndexed(36,0,0);
		}
	}

	if(eMarchingCube == m_SPHSystem.SPHRenderState() || eRender == m_SPHSystem.SPHRenderState())
	{
		ID3D11Buffer * m_VB;

		Vertex* v = new Vertex[m_MarchingCube.m_trianglePos.size() * 3]; 

		for(int i = 0; i < m_MarchingCube.m_trianglePos.size(); i++)
		{

			Vertex vertex1;
			Vertex vertex2;
			Vertex vertex3;
			if(eMarchingCube == m_SPHSystem.SPHRenderState())
			{
				vertex1 = Vertex(m_MarchingCube.m_trianglePos[i].v1.x,m_MarchingCube.m_trianglePos[i].v1.y,m_MarchingCube.m_trianglePos[i].v1.z,0,0,0,0,0,0,0);
				vertex2 = Vertex(m_MarchingCube.m_trianglePos[i].v2.x,m_MarchingCube.m_trianglePos[i].v2.y,m_MarchingCube.m_trianglePos[i].v2.z,0,0,0,0,0,0,0);
				vertex3 = Vertex(m_MarchingCube.m_trianglePos[i].v3.x,m_MarchingCube.m_trianglePos[i].v3.y,m_MarchingCube.m_trianglePos[i].v3.z,0,0,0,0,0,0,0);
			}
			if(eRender == m_SPHSystem.SPHRenderState())
			{
		    	float n1x = m_MarchingCube.m_trianglePos[i].n1.x;
		    	float n1y = m_MarchingCube.m_trianglePos[i].n1.y;
				float n1z = m_MarchingCube.m_trianglePos[i].n1.z;
				float n2x = m_MarchingCube.m_trianglePos[i].n2.x;
				float n2y = m_MarchingCube.m_trianglePos[i].n2.y;
				float n2z = m_MarchingCube.m_trianglePos[i].n2.z;
				float n3x = m_MarchingCube.m_trianglePos[i].n3.x;
				float n3y = m_MarchingCube.m_trianglePos[i].n3.y;
				float n3z = m_MarchingCube.m_trianglePos[i].n3.z;

				vertex1 = Vertex(m_MarchingCube.m_trianglePos[i].v1.x,m_MarchingCube.m_trianglePos[i].v1.y,m_MarchingCube.m_trianglePos[i].v1.z,n1x,n1y,n1z,0,0,0,0);
				vertex2 = Vertex(m_MarchingCube.m_trianglePos[i].v2.x,m_MarchingCube.m_trianglePos[i].v2.y,m_MarchingCube.m_trianglePos[i].v2.z,n2x,n2y,n2z,0,0,0,0);
				vertex3 = Vertex(m_MarchingCube.m_trianglePos[i].v3.x,m_MarchingCube.m_trianglePos[i].v3.y,m_MarchingCube.m_trianglePos[i].v3.z,n3x,n3y,n3z,0,0,0,0);
			}

			v[3 * i] = vertex1;
			v[3 * i + 1] = vertex2;
			v[3 * i + 2] = vertex3;
		}

		D3D11_BUFFER_DESC mVertexBufferDesc;
		mVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		mVertexBufferDesc.MiscFlags = 0;
		mVertexBufferDesc.CPUAccessFlags = 0;
		mVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		mVertexBufferDesc.ByteWidth = sizeof(Vertex) * m_MarchingCube.m_trianglePos.size() * 3;
		mVertexBufferDesc.StructureByteStride = 0;


		D3D11_SUBRESOURCE_DATA mVertexSubReData;
		mVertexSubReData.pSysMem = v;

		m_pd3dDevice->CreateBuffer(&mVertexBufferDesc,&mVertexSubReData,&m_VB);



		m_pd3dDevContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_pd3dDevContext->IASetVertexBuffers(0,1,&m_VB,&iVertexStrides,&ioffset);
		//Set State
		m_pd3dDevContext->OMSetBlendState(StateControl::m_TransparentBS,fBlendFactors,0xffffffff);

		if(eMarchingCube == m_SPHSystem.SPHRenderState())
		m_pd3dDevContext->RSSetState(StateControl::m_NoBackWireFrame);

		if(eRender == m_SPHSystem.SPHRenderState())
		m_pd3dDevContext->RSSetState(StateControl::m_NoBackSolid);

		//Set Matrix
		FXControl::m_CubeMap->SetResource(m_CubeMapSRV);
		m_matWorld = XMMatrixTranslation(0,0,0);
		matWorldInverseTranspose = MatrixInverseTranspose(m_matWorld);
		m_matWorldViewProj = m_matWorld * m_matView * m_matProj;
		//Set FX Variable
		FXControl::m_fxMatWorldViewProj->SetMatrix((float*)&m_matWorldViewProj);
		FXControl::m_fxMatWorld->SetMatrix((float*)&m_matWorld);
		FXControl::m_fxMatWorldInverseTranspose->SetMatrix((float*)&matWorldInverseTranspose);

		//Particle
		//Set Texture
		//FXControl::m_fxDiffuseMap->SetResource(m_d3d11SRVFloor);

		if(eRender == m_SPHSystem.SPHRenderState())
		{
			 for(int p = 0; p < techDesc.Passes; p++ )
			 {
				FXControl::m_fxTech->GetPassByIndex(p)->Apply(0,m_pd3dDevContext);
				m_pd3dDevContext->Draw(m_MarchingCube.m_trianglePos.size() * 3,0);
			 }

		}
		if(eMarchingCube == m_SPHSystem.SPHRenderState())
		{
			for(int p = 0; p < techDesc.Passes; p++ )
			{
				FXControl::m_fxWireFrame->GetPassByIndex(p)->Apply(0,m_pd3dDevContext);
				m_pd3dDevContext->Draw(m_MarchingCube.m_trianglePos.size() * 3,0);
			}
		}
		m_pd3dDevContext->OMSetBlendState(0,fBlendFactors,0xffffffff);
		delete [] v;

		RELEASE_COM(m_VB);
	}
	

	SPH_UIControl::Render();
	
//Set Default State
	m_pd3dDevContext->OMSetBlendState(0,fBlendFactors,0xffffffff);
	m_pd3dDevContext->OMSetDepthStencilState(0,0);

	HRESULT hr = m_pSwapChain->Present(0,0); //错误原因应该是内存泄露，检查一下顶点分配问题

	if(FAILED(hr))
	{
		_com_error err(hr);
		LPCTSTR errMsg = err.ErrorMessage();

		
		MessageBox(0,errMsg,0,0);
	}
}

void RenderObject::KeyDown(WPARAM wParam)
{
	if(wParam == 'W')
		m_v3WorldCoordinate.z += 0.05f;
	if(wParam == 'D')
		m_v3WorldCoordinate.x += 0.05f;
	if(wParam == 'S')
		m_v3WorldCoordinate.z -= 0.05f;
	if(wParam == 'A')
		m_v3WorldCoordinate.x -= 0.05f;

	if(wParam == 'K')
	{
		m_SPHSystem.SPH_Reset();
		m_SPHSystem.SPH_CreateParticlePool();
		m_MarchingCube.Reset();
	}

	if(wParam == 'B')
	{
		m_SPHSystem.SPHRenderState(eParticle);
		m_MarchingCube.IsRender(false);

		SPH_UIControl::setRenderModeBoxState(eParticle);
	}

	if(wParam == 'N')
	{
		m_SPHSystem.SPHRenderState(eBillPartilce);
		m_MarchingCube.IsRender(false);

		SPH_UIControl::setRenderModeBoxState(eBillPartilce);
	}

	if(wParam == 'M')
	{
		m_SPHSystem.SPHRenderState(eMarchingCube);
		m_MarchingCube.IsRender(false);

		SPH_UIControl::setRenderModeBoxState(eMarchingCube);
	}

	if(wParam == 'V')
	{
		m_SPHSystem.SPHRenderState(eRender);
		m_MarchingCube.IsRender(true);

		SPH_UIControl::setRenderModeBoxState(eRender);
	}

}

void RenderObject::MouseButtonDown(WPARAM wParam,float fMousePointx,float fMousePointy)
{
	m_MousePosLastx = fMousePointx;
	m_MousePosLasty = fMousePointy;
}

void RenderObject::MouseMove(WPARAM wParam,float fMousePointx,float fMousePointy)
{

	if( (wParam & MK_LBUTTON) != 0 )
	{

		m_MousePosNowx = fMousePointx;
		m_MousePosNowy = fMousePointy;
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(m_MousePosNowx - m_MousePosLastx));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(m_MousePosNowy - m_MousePosLasty));

		// Update angles based on input to orbit camera around box.
		m_Thea += dx;
		m_Phi  += dy;

		if(m_Phi > PI - 0.1f)
			m_Phi = PI - 0.1f;
		if(m_Phi < 0.1f)
			m_Phi = 0.1f;


		// Restrict the angle mPhi.
	}
	if( (wParam & MK_RBUTTON) != 0 )
	{
		m_MousePosNowx = fMousePointx;
		m_MousePosNowy = fMousePointy;

		// Make each pixel correspond to 0.01 unit in the scene.
		float dx = 0.05f*static_cast<float>(m_MousePosNowx - m_MousePosLastx);
		float dy = 0.05f*static_cast<float>(m_MousePosNowy - m_MousePosLasty);

		// Update the camera radius based on input.
		m_fRadius += dx - dy;

	}

	m_MousePosLastx = m_MousePosNowx;
	m_MousePosLasty = m_MousePosNowy;

}



XMMATRIX RenderObject::MatrixInverseTranspose(XMMATRIX& matrix)
{
	XMMATRIX matTemp = matrix;
	//matTemp.r[3] = XMVectorSet(0,0,0,1.0f); //这里不知道为什么

	XMVECTOR det = XMMatrixDeterminant(matTemp);

	return XMMatrixTranspose(XMMatrixInverse(&det,matTemp));
}

void RenderObject::CreateTriangleBuffer()
{
}

void RenderObject::FrameCount(const float dt)
{
	static int count = 0;
	static float time = 0;

	time += dt;
	count += 1;

	if(time >= 1)
	{
		time = 0;
		m_framecount = count;
		count = 0;
	}



}


