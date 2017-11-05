#ifndef STATECONTROL_H_
#define STATECONTROL_H_
#include"MyD3D11app.h"

class StateControl{


public:
	StateControl(){}
	~StateControl(){Release();}

	static HRESULT InitAllState(ID3D11Device * pd3dDevice);

	void Release();

	static ID3D11BlendState * m_TransparentBS;


	static ID3D11RasterizerState *m_NoBackSolid;
	static ID3D11RasterizerState *m_NoBackWireFrame;
};


#endif