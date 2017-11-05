#include "MyD3D11app.h"

namespace Temp{
	MyD3D11app * tempapp = NULL;
}
LRESULT CALLBACK MsgWndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
	return Temp::tempapp->WndProc(hwnd,message,wParam,lParam);
}



int MyD3D11app::InitWindow(HINSTANCE hInstance,int nShowCmd)
{

	WNDCLASSEX m_wndClass = { 0 };//定义一个窗口类
	m_wndClass.cbSize = sizeof(WNDCLASSEX); //结构体字节数
	m_wndClass.style = CS_HREDRAW |CS_VREDRAW; //设置窗口样式
	m_wndClass.lpfnWndProc = MsgWndProc;//设置窗口过程函数
	m_wndClass.cbClsExtra = 0;
	m_wndClass.cbWndExtra = 0;
	m_wndClass.hInstance = hInstance;//设置实例句柄
	m_wndClass.hIcon = (HICON)::LoadImage(NULL,L"1.ico",IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);//设置图标
	m_wndClass.hCursor = LoadCursor(NULL,IDC_ARROW);//设置鼠标
	m_wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);//设置背景画刷
	m_wndClass.lpszMenuName = NULL;
	m_wndClass.lpszClassName = WINDOW_NAME;


	//注册窗口类
	if(!RegisterClassEx(&m_wndClass))
		return -1;

	m_hwnd = CreateWindow(WINDOW_NAME,WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,(int)m_fWindow_Width,(int)m_fwindow_Height,NULL,NULL,hInstance,NULL);//创建窗口




	ShowWindow(m_hwnd,nShowCmd);
	UpdateWindow(m_hwnd);//对窗口进行更新

	return 0;
}

LRESULT MyD3D11app::WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	

	float fMousePointx,fMousePointy;
	switch(message)
	{
	case WM_LBUTTONDOWN:
		fMousePointx = (float)GET_X_LPARAM(lParam);
		fMousePointy = (float)GET_Y_LPARAM(lParam);
		MouseButtonDown(wParam,fMousePointx,fMousePointy);
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::LeftButton); 
		break;
	case WM_LBUTTONUP: 
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::LeftButton); 
		break; 

	case WM_RBUTTONDOWN:
		fMousePointx = (float)GET_X_LPARAM(lParam);
		fMousePointy = (float)GET_Y_LPARAM(lParam);
		MouseButtonDown(wParam,fMousePointx,fMousePointy);
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::RightButton); 
		break;
	case WM_RBUTTONUP: 
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::RightButton); 
		break;

	case WM_MBUTTONDOWN: 
		fMousePointx = (float)GET_X_LPARAM(lParam);
		fMousePointy = (float)GET_Y_LPARAM(lParam);
		MouseButtonDown(wParam,fMousePointx,fMousePointy);
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::MiddleButton); 
		break; 

	case WM_MBUTTONUP: 
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::MiddleButton); 
		break; 
	case WM_MOUSEMOVE:

		CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition((float)(LOWORD(lParam)), 
			(float)(HIWORD(lParam)));

		fMousePointx= (float)GET_X_LPARAM(lParam);
		fMousePointy = (float)GET_Y_LPARAM(lParam);
		MouseMove(wParam,fMousePointx,fMousePointy); 

		
		break;

	case WM_MOUSEWHEEL: 
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseWheelChange(((short)HIWORD(wParam)) / (120.0f));
		break;


	case WM_KEYDOWN:
		KeyDown(wParam);
		CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(CEGUI::Key::Scan(SPH_UIControl::virtualkey2scancode(wParam,lParam)));
		break;
	case WM_KEYUP:
		CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(CEGUI::Key::Scan(SPH_UIControl::virtualkey2scancode(wParam,lParam)));
		break;
	case WM_CHAR:
		CEGUI::System::getSingleton().getDefaultGUIContext().injectChar( static_cast<CEGUI::utf32>(wParam) );
		break;


	case WM_SIZE:
		if(m_pd3dDevice)
		{
		   m_fWindow_Width = LOWORD(lParam);
		   m_fwindow_Height = HIWORD(lParam);
		   if(!m_isResizing)
		   {
			   Resize(m_fWindow_Width,m_fwindow_Height);
		   }
		}
		break;
	case WM_ENTERSIZEMOVE:
		m_isResizing = TRUE;
		break;
	case WM_EXITSIZEMOVE:
		fMousePointx= (float)GET_X_LPARAM(lParam);
		fMousePointy = (float)GET_Y_LPARAM(lParam);
		m_isResizing = FALSE;
		 Resize(m_fWindow_Width,m_fwindow_Height);
		break;

	case WM_DESTROY:
		Release();
		PostQuitMessage(0);

		break;
	default:
		return DefWindowProc(hwnd,message,wParam,lParam);//默认窗口过程

	}

	return 0;

}

void MyD3D11app::Release()
{
	RELEASE_COM(m_pd3dDevice);
	RELEASE_COM(m_pd3dDevContext);
	RELEASE_COM(m_pRenderTargetView);
	RELEASE_COM(m_pDepthStencilView);
	RELEASE_COM(m_pSwapChain);
}

HRESULT MyD3D11app::Resize(float window_width,float window_height)
{
	assert(m_pSwapChain);
	assert(m_pd3dDevice);
	assert(m_pd3dDevContext);

	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.

	RELEASE_COM(m_pRenderTargetView);
	RELEASE_COM(m_pDepthStencilView);
	// Create Render to Target View

	m_pSwapChain->ResizeBuffers(1, (UINT)window_width, (UINT)window_height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	ID3D11Texture2D* pBackBuffer;
	m_pSwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),reinterpret_cast<void**>(&pBackBuffer));//Attention!
	m_pd3dDevice->CreateRenderTargetView(pBackBuffer,0,&m_pRenderTargetView);
	RELEASE_COM(pBackBuffer);

	//Create Depth and Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = (UINT)window_width;
	depthStencilDesc.Height = (UINT)window_height;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.MiscFlags = 0;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.CPUAccessFlags = 0;

	if(FALSE == m_bEnable4x)
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m_i4xMSAAQuality - 1;
	}

	ID3D11Texture2D * pDepthStencilBuffer; //这里release不掉，注意

	HRESULT hr = m_pd3dDevice->CreateTexture2D(&depthStencilDesc,0,&pDepthStencilBuffer);
	if(FAILED(hr))
	{
		MessageBoxA(0,"Create DepthStencilBuffer Failed",0,0);
		return E_FAIL;
	}

	hr = m_pd3dDevice->CreateDepthStencilView(pDepthStencilBuffer,0,&m_pDepthStencilView);
	if(FAILED(hr))
	{
		MessageBoxA(0,"Create DepthStencilView Failed",0,0);
		return E_FAIL;
	}


	//Bind Two Views
	m_pd3dDevContext->OMSetRenderTargets(1,&m_pRenderTargetView,m_pDepthStencilView);


	//ViewPort
	D3D11_VIEWPORT mVP;
	mVP.Width = window_width;
	mVP.Height = window_height;
	mVP.TopLeftX = 0;
	mVP.TopLeftY = 0;
	mVP.MinDepth = 0;
	mVP.MaxDepth = 1.0f;

	m_pd3dDevContext->RSSetViewports(1,&mVP);

	return S_OK;

}

HRESULT MyD3D11app::D3DInit(HWND hwnd)
{
	//Init Device and DeviceContext
	UINT icreateDeviceFlag = 0;
#if defined(DEBUG) || defined(_DEBUG)
	icreateDeviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL  mFeatureLevel;

	HRESULT hr = D3D11CreateDevice(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		icreateDeviceFlag,
		0,
		0,
		D3D11_SDK_VERSION,
		&m_pd3dDevice,
		&mFeatureLevel,
		&m_pd3dDevContext
		);

	if(FAILED(hr))
	{
		MessageBoxA(0,"Create Device Failed",0,0);
		return E_FAIL;
	}

	if(mFeatureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBoxA(0,"Don't support D3D11",0,0);
		return E_FAIL;
	}





	hr = m_pd3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_B8G8R8A8_UNORM,4,&m_i4xMSAAQuality);

	if(FAILED(hr))
	{
		MessageBoxA(0,"4xMSAA Failed",0,0);
		m_bEnable4x = FALSE;
		//return E_FAIL;
	}

	assert(m_i4xMSAAQuality > 0);

	//Describe swap chain
	DXGI_SWAP_CHAIN_DESC mSwapDesc;
	mSwapDesc.BufferDesc.Width = (UINT)m_fWindow_Width;
	mSwapDesc.BufferDesc.Height = (UINT)m_fwindow_Height;
	mSwapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	mSwapDesc.BufferDesc.RefreshRate.Denominator = 1;
	mSwapDesc.BufferDesc.RefreshRate.Numerator = 60;
	mSwapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	mSwapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	mSwapDesc.BufferCount = 1;
	mSwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	mSwapDesc.Flags = 0;
	mSwapDesc.OutputWindow = hwnd;
	mSwapDesc.Windowed = TRUE;
	mSwapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if(FALSE == m_bEnable4x)
	{
		mSwapDesc.SampleDesc.Count = 1;
		mSwapDesc.SampleDesc.Quality = 0;
	}
	else
	{
		mSwapDesc.SampleDesc.Count = 4;
		mSwapDesc.SampleDesc.Quality = m_i4xMSAAQuality - 1;
	}





	//Create swap chain
	//////////////////////////////////////////////////////////////////////////
	//这里没有仔细去看懂，根据龙书的说法直接创建的IDXGIFactory用来创建SwapChain时就会出错
	//IID tx;
	//IDXGIFactory *factory;
	//CreateDXGIFactory(tx,(void**)&factory);

	//factory->CreateSwapChain(g_pd3dDevice,&sd,&sc);
	IDXGIDevice* pdxgiDevice = 0;
	hr = (m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice),(void**)&pdxgiDevice));
	if(FAILED(hr))
	{
		MessageBoxA(0,"Create SwapChain Failed_1",0,0);
		return E_FAIL;
	}

	IDXGIAdapter* pdxgiAdapter = 0;
	hr = (pdxgiDevice->GetParent(__uuidof(IDXGIAdapter),(void**)&pdxgiAdapter));
	if(FAILED(hr))
	{
		MessageBoxA(0,"Create SwapChain Failed_1",0,0);
		return E_FAIL;
	}

	IDXGIFactory * pdxgiFactory = 0;
	hr = (pdxgiAdapter->GetParent(__uuidof(IDXGIFactory),(void**)&pdxgiFactory));
	if(FAILED(hr))
	{
		MessageBoxA(0,"Create SwapChain Failed_2",0,0);
		return E_FAIL;
	}


	hr = pdxgiFactory->CreateSwapChain(m_pd3dDevice,&mSwapDesc,&m_pSwapChain);
	if(FAILED(hr))
	{
		MessageBoxA(0,"Create SwapChain Failed_3",0,0);
		return E_FAIL;
	}

	RELEASE_COM(pdxgiDevice);
	RELEASE_COM(pdxgiAdapter);
	RELEASE_COM(pdxgiFactory);


	return S_OK;
}

int MyD3D11app::Run()
{
	MSG msg={ 0 };
	while(msg.message!=WM_QUIT)
	{
		if(PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);//信息的转换
			DispatchMessage(&msg);//信息的发送

		}
		else
		{
			QueryPerformanceCounter((LARGE_INTEGER*)&m_iNowCount);

			float deltatime = (m_iNowCount - m_iLastCount) * m_fSecPerCount;

			m_iLastCount = m_iNowCount;

			UpdateScene(deltatime);
			DrawScene();
			
		

		}
	}

	UnregisterClass(WINDOW_NAME,m_wndClass.hInstance);

	return 0;
}

MyD3D11app::MyD3D11app()
{
	m_isResizing = FALSE;
	m_bEnable4x = TRUE;
	m_pRenderTargetView = NULL;
	m_pDepthStencilView = NULL;

	m_fWindow_Width = 1280;
	m_fwindow_Height = 720;

	QueryPerformanceCounter((LARGE_INTEGER*)&m_iLastCount);
	QueryPerformanceCounter((LARGE_INTEGER*)&m_iNowCount);;
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_counterPerSec);

	m_fSecPerCount = 1.0f / (float)m_counterPerSec;

	Temp::tempapp = this;
}

int MyD3D11app::Init(HINSTANCE hInstance,int nShowCmd)
{
	if(-1 == InitWindow(hInstance,nShowCmd))
		return -1;
	if(FAILED(D3DInit(m_hwnd)))
		return -1;
	Resize(m_fWindow_Width,m_fwindow_Height);

	return 0;

}

