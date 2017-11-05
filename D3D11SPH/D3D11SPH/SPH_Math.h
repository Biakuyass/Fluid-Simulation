#ifndef SPH_MATH_H_
#define SPH_MATH_H_

#include<windows.h>
#include<xnamath.h>
#include<vector>

float    DotF3(XMFLOAT3 a ,XMFLOAT3 b);
XMFLOAT3 MinusF3(XMFLOAT3 a ,XMFLOAT3 b);
XMFLOAT3 PlusF3(XMFLOAT3 a ,XMFLOAT3 b);
XMFLOAT3 MultiplyF3(XMFLOAT3 a ,float fb);
XMFLOAT3 DivideF3(XMFLOAT3 a ,float fb);
float    Distance(XMFLOAT3 a, XMFLOAT3 b);
float    DistanceSquare(XMFLOAT3 a, XMFLOAT3 b);

struct SPHBox
{
	float min_x,min_y,min_z;
	float max_x,max_y,max_z;

	SPHBox(float minx,float miny,float minz,float maxx,float maxy, float maxz)
	{
		min_x = minx;
		min_y = miny;
		min_z = minz;
		max_x = maxx;
		max_y = maxy;
		max_z = maxz;
	}
	SPHBox()
	{
		min_x = 0;
		min_y = 0;
		min_z = 0;
		max_x = 0;
		max_y = 0;
		max_z = 0;
	}

};

struct SPHVertex
{
	XMFLOAT3 pos;
	float val;

	SPHVertex()
	{
		pos = XMFLOAT3(0,0,0);
		val = 0;
	}
};

struct SPHCube
{
	XMFLOAT3 pos[8];
	float val[8];
	XMFLOAT3 normal[8];


	SPHCube()
	{
		for(int i = 0; i < 8; i++)
		{
			pos[i] = XMFLOAT3(0,0,0);
			val[i] = 0;
			normal[i] = XMFLOAT3(0,0,0);
		}
	}
};

struct SPHTriangle
{
	XMFLOAT3 v1,v2,v3;
	XMFLOAT3 n1,n2,n3;
	SPHTriangle(XMFLOAT3 v_1,XMFLOAT3 v_2,XMFLOAT3 v_3,
		XMFLOAT3 n_1,XMFLOAT3 n_2,XMFLOAT3 n_3)
	{
		v1 = v_1;
		v2 = v_2;
		v3 = v_3;
		n1 = n_1;
		n2 = n_2;
		n3 = n_3;
	}
};
#endif