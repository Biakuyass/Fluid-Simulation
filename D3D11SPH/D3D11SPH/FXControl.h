#ifndef FXCONTROL_H_
#define FXCONTROL_H_
#include "MyD3D11app.h"


class FXControl{

private:
	void Release();
public:

	FXControl(){}
	~FXControl(){Release();}


	static void PrepareFXVariable();


	static HRESULT Init(ID3D11Device * pd3dDevice);

	static ID3DX11EffectMatrixVariable* m_fxMatWorld;
	static ID3DX11EffectMatrixVariable* m_fxMatWorldInverseTranspose;
	static ID3DX11EffectVectorVariable* m_fxEyePos;
	static ID3DX11EffectVectorVariable* m_fxLightDir;
	static ID3DX11EffectVariable*       m_fxSkyBoxRange;

	static ID3DX11EffectShaderResourceVariable* m_fxDiffuseMap;
	static ID3DX11EffectShaderResourceVariable* m_CubeMap;

	static ID3DX11EffectMatrixVariable *m_fxMatWorldViewProj;

	static ID3DX11EffectTechnique * m_fxTech;
	static ID3DX11EffectTechnique * m_fxTransparentTech;
	static ID3DX11EffectTechnique * m_fxShadowTech;
	static ID3DX11EffectTechnique * m_fxBillBoardTech;
	static ID3DX11EffectTechnique * m_fxSkyBox;
	static ID3DX11EffectTechnique * m_fxWireFrame;

	static ID3DX11Effect * m_Fx;
	static LPCTSTR m_sFxName;
};

#endif