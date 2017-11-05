#include"FXControl.h"


ID3DX11EffectMatrixVariable* FXControl::m_fxMatWorld = 0;
ID3DX11EffectMatrixVariable* FXControl::m_fxMatWorldInverseTranspose = 0;
ID3DX11EffectVectorVariable* FXControl::m_fxEyePos = 0;
ID3DX11EffectVectorVariable* FXControl::m_fxLightDir = 0;
ID3DX11EffectVariable* FXControl::m_fxSkyBoxRange = 0;

ID3DX11EffectShaderResourceVariable* FXControl::m_fxDiffuseMap = 0;
ID3DX11EffectShaderResourceVariable* FXControl::m_CubeMap = 0;

ID3DX11EffectMatrixVariable* FXControl::m_fxMatWorldViewProj = 0;

ID3DX11EffectTechnique* FXControl::m_fxTech = 0;
ID3DX11EffectTechnique* FXControl::m_fxTransparentTech = 0;
ID3DX11EffectTechnique* FXControl::m_fxShadowTech = 0;
ID3DX11EffectTechnique* FXControl::m_fxBillBoardTech = 0;
ID3DX11EffectTechnique * FXControl::m_fxSkyBox = 0;
ID3DX11EffectTechnique * FXControl::m_fxWireFrame = 0;

ID3DX11Effect* FXControl::m_Fx = 0;

LPCTSTR FXControl::m_sFxName = L"Texture.fx";


HRESULT FXControl::Init(ID3D11Device * pd3dDevice)
{
	//BuildFX

	DWORD shaderflag = 0;
#if defined(DEBUG) || defined(_DEBUG)
	shaderflag |= D3D10_SHADER_DEBUG;
#endif

	ID3D10Blob * compiledShader = 0;
	ID3D10Blob * compiledMsg = 0;

	HRESULT hr = D3DX11CompileFromFile(m_sFxName,0,0,0,"fx_5_0",shaderflag,0,0,&compiledShader,&compiledMsg,0);

	if(0 != compiledMsg)
	{
		MessageBoxA(0,(char*)compiledMsg->GetBufferPointer(),0,0);
		RELEASE_COM(compiledMsg);
	}
	if(FAILED(hr))
	{
		MessageBoxA(0,"Fail to Compile Shader",0,0);
		return E_FAIL;
	}

	hr = D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(),compiledShader->GetBufferSize(),0,pd3dDevice,&m_Fx);
	if(FAILED(hr))
	{
		MessageBoxA(0,"Fail to Create Effect",0,0);
		return E_FAIL;
	}
	RELEASE_COM(compiledShader);

	PrepareFXVariable();

	return S_OK;

}

void FXControl::Release()
{
	RELEASE_COM(m_Fx);
}

void FXControl::PrepareFXVariable()
{
	m_fxTech = m_Fx->GetTechniqueByName("BasicTech");
	m_fxTransparentTech = m_Fx->GetTechniqueByName("TransparentTech");
	m_fxShadowTech = m_Fx->GetTechniqueByName("ShadowTech");
	m_fxBillBoardTech = m_Fx->GetTechniqueByName("BillBoardTech");
	m_fxSkyBox = m_Fx->GetTechniqueByName("SkyboxTech");
	m_fxWireFrame = m_Fx->GetTechniqueByName("WireFrameTech");
	m_fxMatWorldViewProj = m_Fx->GetVariableByName("matWorldViewProj")->AsMatrix();
	m_fxMatWorld = m_Fx->GetVariableByName("matWorld")->AsMatrix();
	m_fxMatWorldInverseTranspose = m_Fx->GetVariableByName("matWorldInverseTranspose")->AsMatrix();
	m_fxEyePos = m_Fx->GetVariableByName("EyePos")->AsVector();
	m_fxLightDir = m_Fx->GetVariableByName("LightDir")->AsVector();
	m_fxDiffuseMap = m_Fx->GetVariableByName("gTexture")->AsShaderResource();
	m_CubeMap = m_Fx->GetVariableByName("gCubeTexture")->AsShaderResource();
	m_fxSkyBoxRange = m_Fx->GetVariableByName("SkyBoxRange");
	
}














