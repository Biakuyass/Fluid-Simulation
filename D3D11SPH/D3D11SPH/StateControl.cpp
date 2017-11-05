#include "StateControl.h"

ID3D11BlendState* StateControl::m_TransparentBS = 0;
ID3D11RasterizerState* StateControl::m_NoBackSolid = 0;
ID3D11RasterizerState* StateControl::m_NoBackWireFrame = 0;

HRESULT StateControl::InitAllState(ID3D11Device * pd3dDevice)
{
//BlendState
//m_TransparentBS
	D3D11_BLEND_DESC transparent_blendDESC = {0};

	transparent_blendDESC.AlphaToCoverageEnable = false;
	transparent_blendDESC.IndependentBlendEnable = false;

	transparent_blendDESC.RenderTarget[0].BlendEnable = true;
	transparent_blendDESC.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	transparent_blendDESC.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;//
	transparent_blendDESC.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	transparent_blendDESC.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	transparent_blendDESC.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	transparent_blendDESC.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	transparent_blendDESC.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = pd3dDevice->CreateBlendState(&transparent_blendDESC,&m_TransparentBS);

//Rasterizer State
	//m_BackCullRs
	D3D11_RASTERIZER_DESC mRasterizerDesc;
	ZeroMemory(&mRasterizerDesc,sizeof(D3D11_RASTERIZER_DESC));
	mRasterizerDesc.CullMode = D3D11_CULL_NONE;
	mRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	mRasterizerDesc.DepthClipEnable = TRUE;


	hr = pd3dDevice->CreateRasterizerState(&mRasterizerDesc,&m_NoBackSolid);
	if(FAILED(hr))
	{
		MessageBoxA(0,"Fail to Create Rasterizer State",0,0);
		return E_FAIL;
	}


	D3D11_RASTERIZER_DESC mWireRasterizerDesc;
	ZeroMemory(&mWireRasterizerDesc,sizeof(D3D11_RASTERIZER_DESC));
	mWireRasterizerDesc.CullMode = D3D11_CULL_NONE;
	mWireRasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	mWireRasterizerDesc.DepthClipEnable = TRUE;


	hr = pd3dDevice->CreateRasterizerState(&mWireRasterizerDesc,&m_NoBackWireFrame);
	if(FAILED(hr))
	{
		MessageBoxA(0,"Fail to Create Rasterizer State",0,0);
		return E_FAIL;
	}
	return S_OK;

}

void StateControl::Release()
{


}


