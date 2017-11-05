cbuffer cbObject{
	float4x4 matWorldViewProj;
	float4x4 matWorld;
	float4x4 matWorldInverseTranspose;
	float3   EyePos;
	float3   LightDir;
	float    SkyBoxRange;

	float2 TexUV[4] = {
		float2(0,0),float2(1.0f,0),float2(0,1.0f),float2(1.0f,1.0f)
	}; 
};

Texture2D gTexture;
TextureCube gCubeTexture;

SamplerState samAnisotropic{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};

struct VertexIn{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
	float2 Size : SIZE;
	
};

struct VertexOut{
	float4 Pos : SV_POSITION;
	float3 PosW : POSITION;
	float3 Normal: NORMAL;	
	float2 Tex : TEXCOORD;
	float3 PosO : TEXCOORD1;
	float2 Size : SIZE;

	
};

struct GeoOut{
	float4 Pos : SV_POSITION;
	float3 PosW : POSITION;
	float3 Normal: NORMAL;
	float2 Tex : TEXCOORD;
	uint   PrimitiveID : SV_PrimitiveID;
};



VertexOut VS(VertexIn vin){
	VertexOut vout;
	
	vout.PosO = vin.Pos;
	vout.Pos = mul(float4(vin.Pos,1.0),matWorldViewProj);
	vout.PosW = mul(float4(vin.Pos,1.0),matWorld).xyz;
	vout.Normal = normalize(mul(vin.Normal,(float3x3)matWorldInverseTranspose));
    vout.Tex =  vin.Tex;
	vout.Size = vin.Size;
	//vout.Color = vin.Color;

	return vout;
}

[maxvertexcount (6)]
void GS(point VertexOut gin[1],uint primID : SV_PrimitiveID,inout TriangleStream<GeoOut> triStream)
{


	float3 look = normalize( EyePos - gin[0].PosO);
	//look.y = 0;
	float3 up = float3(0,1.0,0);
	float3 right = normalize(cross(up,look));
	up = cross(look,right);

	float4 v[4];

	v[0] = float4(gin[0].PosO - gin[0].Size.x / 2.0f * right + gin[0].Size.y / 2.0f * up,1.0f);
	v[1] = float4(gin[0].PosO + gin[0].Size.x / 2.0f * right + gin[0].Size.y / 2.0f * up,1.0f);
	v[2] = float4(gin[0].PosO - gin[0].Size.x / 2.0f * right - gin[0].Size.y / 2.0f * up ,1.0f);
	v[3] = float4(gin[0].PosO + gin[0].Size.x / 2.0f * right - gin[0].Size.y / 2.0f * up ,1.0f);
	/*v[0] = float4(0,0,0,1.0f);
	v[1] = float4(100,100,0,1.0f);
	v[2] = float4(-100,-100,0,1.0f);
	v[3] = float4(100,-100,0,1.0f);*/

	/*v[0] = float4(gin[0].PosO,1.0f);
	v[1] = float4(gin[0].PosO +100,1.0f);
	v[2] = float4(gin[0].PosO - 100,1.0f);
	v[3] = float4(gin[0].PosO - 200,1.0f);*/

	GeoOut geo;

	for(int i = 0;i < 4 ;i++)
	{

		geo.PosW = v[i].xyz;
		geo.Pos = mul(v[i],matWorldViewProj);
		geo.Normal = look;
		geo.Tex = TexUV[i];
		geo.PrimitiveID = primID;
		triStream.Append(geo);
	}

}

float3 Intersect(in float3 p,in float3 v)
{

	float t[6];

	float range = SkyBoxRange;

	if(v.x != 0)
		t[0] = (range - p.x) / v.x;
	else
		t[0] = -1;

	if(v.x != 0)
		t[1] = (-range - p.x) / v.x;
	else
		t[1] = -1;

	if(v.y != 0)
		t[2] = (range - p.y) / v.y;
	else
		t[2] = -1;

	if(v.y != 0)
		t[3] = (-range - p.y) / v.y;
	else
		t[3] = -1;

	if(v.z != 0)
		t[4] = (range - p.z) / v.z;
	else
		t[4] = -1;

	if(v.z != 0)
		t[5] = (-range - p.z) / v.z;
	else
		t[5] = -1;

	float tre = 100000;
	for(int i = 0; i < 6; i++)
	{
		if(t[i] >= 0 && t[i] < tre)
			tre = t[i];
	}

	float3 intersectPoint;

	intersectPoint.x = p.x + v.x * tre;
	intersectPoint.y = p.y + v.y * tre;
	intersectPoint.z = p.z + v.z * tre;


	return intersectPoint;
}

float4 PS(VertexOut pin, uniform bool istransparent,uniform bool isshadow) : SV_Target{

	//默认各种光强都为1.0f

	float4 DiffuseMaterial = float4(0.5f,0.5f,0.5f,1.0f);
	float4 SpecularMaterial = float4(0.5f,0.5f,0.5f,1.0f);
	float4 AmbientMaterial = float4(0.5f,0.5f,0.5f,1.0f);


	float3 PostoEye = normalize(EyePos - pin.PosW);

	float AmbientPower = 0.3f;

	float PrepareDiffuse = dot(normalize(-LightDir),pin.Normal);
	float DiffusePower = clamp(PrepareDiffuse,0,1);

	float SpecularPower = 0;

	if(PrepareDiffuse > 0)
	SpecularPower = pow(max(dot(reflect(normalize(LightDir),pin.Normal),PostoEye),0),10);
	//SpecularPower = 1;
	float4 TexColor;
		TexColor = gTexture.Sample(samAnisotropic,pin.Tex);


	//	SpecularPower = 0;
	float3 incident = -normalize(PostoEye);

	float3 reflectionvector = normalize(reflect(incident,pin.Normal));
	float3 reflect_point = Intersect(pin.PosW,reflectionvector);

	float4 ReflectColor = gCubeTexture.Sample(samAnisotropic,reflect_point);


	float3 refractionvector = normalize(refract(incident,pin.Normal,0.9));
	float3 refract_point = Intersect(pin.PosW,refractionvector);

	float4 RefractColor = gCubeTexture.Sample(samAnisotropic,refract_point);

	

	float4 Color = float4(0,0,0,1.0f) * (AmbientPower * AmbientMaterial +  SpecularPower * SpecularMaterial  + DiffusePower * DiffuseMaterial);
	Color.w = 1;
	//Color /= Color.w; //float4(0.15625,0.5859375,0.78125f,1.0f) float4(0,0,0,1.0f)
	float r = 0.02037f; 
    float fastFresnel = r + ( 1- r) * pow(1.0 - dot(-incident,pin.Normal),5);

//	float4 rl_color = float4((ReflectColor * fastFresnel).xyz,1.0f);
//	float4 ra_color = float4((RefractColor * (1 - fastFresnel)).xyz,1.0f);
//	Color += rl_color;
//	Color += ra_color;


	Color += ReflectColor * fastFresnel;//* (AmbientPower * AmbientMaterial +  SpecularPower * SpecularMaterial  + DiffusePower * DiffuseMaterial);
    Color.w = 1;
	Color += RefractColor * (1 - fastFresnel);
	


	return Color;

//

}

float4 PS_BillBoard(GeoOut pin) : SV_Target{

	//默认各种光强都为1.0f

/*	float4 DiffuseMaterial = float4(0.5f,0.5f,0.5f,1.0f);
	float4 SpecularMaterial = float4(0.5f,0.5f,0.5f,1.0f);
	float4 AmbientMaterial = float4(0.5f,0.5f,0.5f,1.0f);


	float3 PostoEye = normalize(EyePos - pin.PosW);

	float AmbientPower = 0.3f;

	float PrepareDiffuse = dot(normalize(-LightDir),pin.Normal);
	float DiffusePower = clamp(PrepareDiffuse,0,1);

	float SpecularPower = 0;

	if(PrepareDiffuse > 0)
	SpecularPower = pow(max(dot(reflect(normalize(LightDir),pin.Normal),PostoEye),0),10);

	float4 TexColor;
		TexColor = gTexture.Sample(samAnisotropic,pin.Tex);


	float4 Color = TexColor * (AmbientPower * AmbientMaterial +  SpecularPower * SpecularMaterial  + DiffusePower * DiffuseMaterial);

	if(istransparent)
		Color.w = 0.5f;

    if(isshadow)
			Color = float4(0,0,0,0.5f);*/

		float4 Color = gTexture.Sample(samAnisotropic,pin.Tex);

       // float4 Color = float4(1.0f,1.0f,1.0f,1.0f);
		return Color;




}


float4 PS_Skybox(VertexOut pin) : SV_Target{

	//默认各种光强都为1.0f

/*	float4 DiffuseMaterial = float4(0.5f,0.5f,0.5f,1.0f);
	float4 SpecularMaterial = float4(0.5f,0.5f,0.5f,1.0f);
	float4 AmbientMaterial = float4(0.5f,0.5f,0.5f,1.0f);


	float3 PostoEye = normalize(EyePos - pin.PosW);

	float AmbientPower = 0.3f;

	float PrepareDiffuse = dot(normalize(-LightDir),pin.Normal);
	float DiffusePower = clamp(PrepareDiffuse,0,1);

	float SpecularPower = 0;

	if(PrepareDiffuse > 0)
	SpecularPower = pow(max(dot(reflect(normalize(LightDir),pin.Normal),PostoEye),0),10);

	float4 TexColor;
		TexColor = gTexture.Sample(samAnisotropic,pin.Tex);


	float4 Color = TexColor * (AmbientPower * AmbientMaterial +  SpecularPower * SpecularMaterial  + DiffusePower * DiffuseMaterial);

	if(istransparent)
		Color.w = 0.5f;

    if(isshadow)
			Color = float4(0,0,0,0.5f);*/
	
	float4 Color = gCubeTexture.Sample(samAnisotropic,pin.PosW);

       // float4 Color = float4(1.0f,1.0f,1.0f,1.0f);
		return Color;




}

float4 PS_WireFrame(VertexOut pin) : SV_Target{

	//默认各种光强都为1.0f

/*	float4 DiffuseMaterial = float4(0.5f,0.5f,0.5f,1.0f);
	float4 SpecularMaterial = float4(0.5f,0.5f,0.5f,1.0f);
	float4 AmbientMaterial = float4(0.5f,0.5f,0.5f,1.0f);


	float3 PostoEye = normalize(EyePos - pin.PosW);

	float AmbientPower = 0.3f;

	float PrepareDiffuse = dot(normalize(-LightDir),pin.Normal);
	float DiffusePower = clamp(PrepareDiffuse,0,1);

	float SpecularPower = 0;

	if(PrepareDiffuse > 0)
	SpecularPower = pow(max(dot(reflect(normalize(LightDir),pin.Normal),PostoEye),0),10);

	float4 TexColor;
		TexColor = gTexture.Sample(samAnisotropic,pin.Tex);


	float4 Color = TexColor * (AmbientPower * AmbientMaterial +  SpecularPower * SpecularMaterial  + DiffusePower * DiffuseMaterial);

	if(istransparent)
		Color.w = 0.5f;

    if(isshadow)
			Color = float4(0,0,0,0.5f);*/
	
	float4 Color = float4(1.0f,1.0f,1.0f,0.5f);

       // float4 Color = float4(1.0f,1.0f,1.0f,1.0f);
	return Color;




}
 
technique11  BasicTech{

	pass P0{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(false,false) ) );
	}
	/*pass P0{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader( gs_5_0, GS()) );
        SetPixelShader( CompileShader( ps_5_0, PS_BillBoard() ) );
	}*/

}

technique11  SkyboxTech{

	pass P0{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS_Skybox() ) );
	}


}

technique11  WireFrameTech{

	pass P0{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS_WireFrame() ) );
	}


}

technique11 BillBoardTech{

	pass P0{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader( gs_5_0, GS()) );
        SetPixelShader( CompileShader( ps_5_0, PS_BillBoard() ) );
	}

}

technique11 TransparentTech{

	pass P0{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(true,false) ) );
	}

}

technique11 ShadowTech{

	pass P0{
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(false,true) ) );
	}

}


/*cbuffer cbPerObject
{
	float4x4 gWorldViewProj; 
};

struct VertexIn
{
	float3 PosL  : POSITION;
    float4 Color : COLOR;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
    float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	
	// Just pass vertex color into the pixel shader.
    vout.Color = vin.Color;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    return pin.Color;
}

technique11 ColorTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}*/