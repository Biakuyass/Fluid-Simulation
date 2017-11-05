#ifndef MYD3D11APP_H_
#define MYD3D11APP_H_
#include<time.h>
#include<assert.h>
#include<tchar.h> //”√”⁄swprintf
#include<D3D11.h>
#include<d3dx11effect.h>
#include<D3DX11async.h>
#include<xnamath.h>
#include "SPH_UI.h"
#include<windows.h>
#include<windowsx.h>

#define WINDOW_TITLE L"D3D11 Program"
#define WINDOW_NAME L"All for Loli"
#define RELEASE_COM(x) {if(x){ x->Release(); x = NULL;} }
 
class MyD3D11app{

private:
	WNDCLASSEX m_wndClass;
	UINT m_i4xMSAAQuality;
	BOOL m_bEnable4x;
  
	HRESULT D3DInit(HWND hwnd);
    int InitWindow(HINSTANCE hInstance,int nShowCmd);
public:
	MyD3D11app();
	virtual ~MyD3D11app(){Release(); UnregisterClass(WINDOW_NAME,m_wndClass.hInstance);}
	ID3D11Device * m_pd3dDevice;
	ID3D11DeviceContext * m_pd3dDevContext;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;
	IDXGISwapChain* m_pSwapChain;

	float m_fWindow_Width;
	float m_fwindow_Height;

	BOOL m_isResizing;

	__int64 m_iLastCount;
	__int64 m_iNowCount;
	float m_fSecPerCount;
	__int64 m_counterPerSec;


	HWND m_hwnd;
	LRESULT WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);

	int Run();

	virtual int Init(HINSTANCE hInstance,int nShowCmd);



protected:
	virtual void KeyDown(WPARAM wParam){};
	virtual void MouseButtonDown(WPARAM wParam,float fMousePointx,float fMousePointy){};
	virtual void MouseMove(WPARAM wParam,float fMousePointx,float fMousePointy){};
	virtual void Release();
	virtual void DrawScene() = 0;
	virtual void UpdateScene(const float dt) = 0;

	virtual HRESULT Resize(float window_width,float window_height);


};
#endif