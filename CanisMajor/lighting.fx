//=============================================================================
// lighting.fx by Frank Luna (C) 2008 All Rights Reserved.
//
// Transforms and lights geometry.
//=============================================================================

#include "sharedDefines.h"
#include "lighthelper.fx"
 
cbuffer cbPerFrame
{
	Light gLight;
	bool gLightType; 
	float3 gEyePosW;
	
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWVP;
};

Light lights[MAX_LIGHTS];
int type[MAX_LIGHTS];
Light pLight;
Light ambient;
Light negaLight;

int activeLights; //Set this in-game: Number of ACTIVE lights

struct VS_IN
{
	float3 posL    : POSITION;
	float3 normalL : NORMAL;
	float4 diffuse : DIFFUSE;
	float4 spec    : SPECULAR;
};

struct VS_OUT
{
	float4 posH    : SV_POSITION;
    float3 posW    : POSITION;
    float3 normalW : NORMAL;
    float4 diffuse : DIFFUSE;
    float4 spec    : SPECULAR;
};

VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;
	
	// Transform to world space space.
	vOut.posW    = mul(float4(vIn.posL, 1.0f), gWorld);
	vOut.normalW = mul(float4(vIn.normalL, 0.0f), gWorld);
		
	// Transform to homogeneous clip space.
	vOut.posH = mul(float4(vIn.posL, 1.0f), gWVP);
	
	// Output vertex attributes for interpolation across triangle.
	vOut.diffuse = vIn.diffuse;
	vOut.spec    = vIn.spec;
	
	return vOut;
}
 

float4 PS(VS_OUT pIn) : SV_Target
{
	// Interpolating normal can make it not be of unit length so normalize it.
    pIn.normalW = normalize(pIn.normalW);
   
   
    SurfaceInfo v = {pIn.posW, pIn.normalW, pIn.diffuse, pIn.spec};
	float3 litColor = float3(0.0f, 0.0f, 0.0f);
	float3 newColor;


	//Calculations for point lights
	[loop]
	for( uint i = 0; i < activeLights; i++ )
	{
		//Calculate as Spotlight
		if(type[i] == 1) {
			newColor = Spotlight(v, lights[i], gEyePosW);
			litColor.x = max(newColor.x,litColor.x);
			litColor.y = max(newColor.y,litColor.y);
			litColor.z = max(newColor.z,litColor.z);
		}

		//Calculate as Pointlight
		else {
			newColor = PointLight(v, lights[i], gEyePosW);
			litColor.x = max(newColor.x,litColor.x);
			litColor.y = max(newColor.y,litColor.y);
			litColor.z = max(newColor.z,litColor.z);
		}

	}

	//Ambient lighting loaded first
	litColor += ParallelLight(v, ambient, gEyePosW);
    
	//Flashlight bool
	if(gLightType)
	{
		litColor += Spotlight(v, gLight, gEyePosW);
	}

	//Dark emitter
	//litColor += PointLight(v, negaLight, gEyePosW);
	   
    return float4(litColor, pIn.diffuse.a);
}

technique10 LightTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}
