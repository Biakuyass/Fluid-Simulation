#include<windows.h>
#include<windowsx.h>
#include<time.h>
#include<assert.h>
#include<tchar.h> //ÓÃÓÚswprintf
#include<D3D11.h>
#include<d3dx11effect.h>
#include<D3DX11async.h>
#include<xnamath.h>
#include"RenderObject.h"

// ¡¾WinMain¡¿ Function
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
//   	Skull skull;
//   	skull.Init(hInstance,nShowCmd);
//   	skull.Run();
   	RenderObject Box;
  	Box.Init(hInstance,nShowCmd);
   	Box.Run();
	
	return 0;

}
