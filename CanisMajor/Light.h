//=======================================================================================
// Light.h by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#ifndef LIGHT_H
#define LIGHT_H

#include "d3dUtil.h"

struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}

	//(0) room pointlight (1) ambient (2) spotlight (3) negalight (4) moonlight: spot
	int init(int settings = 0) {	//Default settings! not required to use, just quick setup
		
		if(settings == 1) {		//Ambient light, world light
			dir      = D3DXVECTOR3(0.57735f, -0.57735f, 0.57735f);
			ambient  = D3DXCOLOR(0.0078125f, 0.015625f, 0.03125f, 1.0f);
			diffuse  = D3DXCOLOR(.001f, .001f, .001f, 1.0f);
			specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

			return 2;
		}

		else if(settings == 2) { //Spotlight, like our flashlight
			ambient  = D3DXCOLOR(0.2f, 0.2f, 0.15f, 1.0f);
			diffuse  = D3DXCOLOR(.0005f, .0005f ,.0005f, 1.0f);
			specular = D3DXCOLOR(.01f, .01f, .01f, 1.0f);
			att.x    = 1.0f;
			att.y    = 0.0f;
			att.z    = 0.f;
			spotPow  = 16.0f;
			range    = 10000.0f;
			return 1;
		}

		else if(settings == 3) { //Negalight, for the dog or spooky places
			ambient  = D3DXCOLOR(-.5f, -.5f, -.5f, 1.0f);
			diffuse  = D3DXCOLOR(-.5f, -.5f, -.5f, 1.0f);
			specular = D3DXCOLOR(-.5f, -.5f, -.5f, 1.0f);
			att.x    = 1.0f;
			att.y    = .1f;
			att.z    = 0.01f;
			range    = 15.0f;
			return 0;
		}

		else if(settings == 4) { //Spotlight, moonlight
			ambient  = D3DXCOLOR(0.02745098039f, 0.07843137254f, 0.21176470588f, 1.0f);
			diffuse  = D3DXCOLOR(.001f, .001f, .001f, 1.0f);
			specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			att.x    = 15.0f;	//Flat increase/decrease
			att.y    = .1f;		//Linear increase/decrease
			att.z    = 0.1f;	//Exponential increase/decrease
			range    = 50.0f;	
			spotPow  = 16.0f;
			return 1;
		}

		else { //Pointlight, mostly our room lights
			ambient  = D3DXCOLOR(0.45490196078f, 0.29803921568f, 0.12549019607f, 1.0f);
			diffuse  = D3DXCOLOR(.1f, .1f ,.1f, 1.0f);
#ifdef DEBUG
			specular = D3DXCOLOR(.1f, .1f, .1f, 1.0f);
#else
			specular = D3DXCOLOR(.00001f, .00001f, .00001f, 1.0f);
#endif
			att.x    = 0.3f;	//Flat increase/decrease
			att.y    = .1f;		//Linear increase/decrease
#ifdef DEBUG
			att.z    = 0.00f; //Exponential increase/decrease
#else
			att.z    = 0.05f; //Exponential increase/decrease
#endif

			range    = 35.0f;
			return 0;
		}
		
		return -1;
	}

	D3DXVECTOR3 pos;
	float pad1;      // not used
	D3DXVECTOR3 dir;
	float pad2;      // not used
	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR specular;
	D3DXVECTOR3 att;
	float spotPow;
	float range;
};

struct Material
{
	Material()
	{
		ZeroMemory(this, sizeof(Material));
	}

	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR specular;
	float specPower;
};

#endif // LIGHT_H