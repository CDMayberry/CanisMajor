//=============================================================================
// lighting.fx by Frank Luna (C) 2008 All Rights Reserved.
//
// Transforms and lights geometry.
//=============================================================================

#include "sharedDefines.h"

struct Light
{
	float3 pos;
	float3 dir;
	float4 ambient;
	float4 diffuse;
	float4 spec;
	float3 att;
	float  spotPower;
	float  range;
};

struct SurfaceInfo
{
	float3 pos;
    float3 normal;
    float4 diffuse;
    float4 spec;
};
 
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


//#define MAX_LIGHTS 4

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


float3 ParallelLight(SurfaceInfo v, Light L, float3 eyePos)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);
 
	// The light vector aims opposite the direction the light rays travel.
	float3 lightVec = -L.dir;
	
	// Add the ambient term.
	litColor += v.diffuse * L.ambient;	
	
	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.
	
	float diffuseFactor = dot(lightVec, v.normal);
	[branch]
	if( diffuseFactor > 0.0f )
	{
		float specPower  = max(v.spec.a, 1.0f);
		float3 toEye     = normalize(eyePos - v.pos);
		float3 R         = reflect(-lightVec, v.normal);
		float specFactor = pow(max(dot(R, toEye), 0.0f), specPower);
					
		// diffuse and specular terms
		litColor += diffuseFactor * v.diffuse * L.diffuse;
		litColor += specFactor * v.spec * L.spec;
	}
	
	return litColor;
}

float3 PointLight(SurfaceInfo v, Light L, float3 eyePos)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);
	
	// The vector from the surface to the light.
	float3 lightVec = L.pos - v.pos;
		
	// The distance from surface to light.
	float d = length(lightVec);
	
	if( d > L.range )
		return float3(0.0f, 0.0f, 0.0f);
		
	// Normalize the light vector.
	lightVec /= d; 
	
	// Add the ambient light term.
	litColor += v.diffuse * L.ambient;	
	
	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.
	
	float diffuseFactor = dot(lightVec, v.normal);
	[branch]
	if( diffuseFactor > 0.0f )
	{
		float specPower  = max(v.spec.a, 1.0f);
		float3 toEye     = normalize(eyePos - v.pos);
		float3 R         = reflect(-lightVec, v.normal);
		float specFactor = pow(max(dot(R, toEye), 0.0f), specPower);
	
		// diffuse and specular terms
		litColor += diffuseFactor * v.diffuse * L.diffuse;
		litColor += specFactor * v.spec * L.spec;
	}
	
	// attenuate
	return litColor / dot(L.att, float3(1.0f, d, d*d));
}

float3 Spotlight(SurfaceInfo v, Light L, float3 eyePos)
{
	float3 litColor = PointLight(v, L, eyePos);
	
	// The vector from the surface to the light.
	float3 lightVec = normalize(L.pos - v.pos);
	
	float s = pow(max(dot(-lightVec, L.dir), 0.0f), L.spotPower);
	
	// Scale color by spotlight factor.
	return litColor*s;
}


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
