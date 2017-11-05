#include "SPH_Math.h"

float DotF3(XMFLOAT3 a ,XMFLOAT3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

XMFLOAT3 MinusF3(XMFLOAT3 a ,XMFLOAT3 b)
{
	return XMFLOAT3(a.x - b.x,a.y - b.y,a.z - b.z);
}

XMFLOAT3 PlusF3(XMFLOAT3 a ,XMFLOAT3 b)
{
	return XMFLOAT3(a.x + b.x,a.y + b.y,a.z + b.z);
}

XMFLOAT3 MultiplyF3(XMFLOAT3 a ,float fb)
{
	return XMFLOAT3(a.x * fb,a.y * fb,a.z * fb);
}

XMFLOAT3 DivideF3(XMFLOAT3 a ,float fb)
{
	if(fb == 0)
		MessageBoxA(0,"DivideError",0,0);
	return XMFLOAT3(a.x / fb,a.y / fb,a.z / fb);
}

float Distance(XMFLOAT3 a, XMFLOAT3 b)
{ 
	return pow((a.x - b.x) *(a.x - b.x) + (a.y - b.y)*(a.y - b.y)+ (a.z - b.z)*(a.z - b.z),0.5f);
}

float DistanceSquare(XMFLOAT3 a, XMFLOAT3 b)
{ 
	return (a.x - b.x) *(a.x - b.x) + (a.y - b.y)*(a.y - b.y)+ (a.z - b.z)*(a.z - b.z);
}