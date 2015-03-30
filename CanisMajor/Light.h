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

	//(1) ambient (2) spotlight (3) negalight (0) pointlight
	void init(int settings = 0) {	//Default settings! not required to use, just quick setup
		if(settings == 1) {		//Ambient light, world light
			dir      = D3DXVECTOR3(0.57735f, -0.57735f, 0.57735f);
			ambient  = D3DXCOLOR(0.0078125f, 0.015625f, 0.03125f, 1.0f);
			diffuse  = D3DXCOLOR(.001f, .001f, .001f, 1.0f);
			specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		}
		else if(settings == 2) {					//Spotlight, like our flashlight
			ambient  = D3DXCOLOR(0.2f, 0.2f, 0.15f, 1.0f);
			diffuse  = D3DXCOLOR(.5f, .5f ,.5f, 1.0f);
			specular = D3DXCOLOR(.1f, .1f, .1f, 1.0f);
			att.x    = 1.0f;
			att.y    = 0.0f;
			att.z    = 0.0f;
			spotPow  = 128.0f;
			range    = 10000.0f;

		}
		else if(settings == 3) { //Negalight, for the dog or spooky places
			ambient  = D3DXCOLOR(-.5f, -.5f, -.5f, 1.0f);
			diffuse  = D3DXCOLOR(-.5f, -.5f, -.5f, 1.0f);
			specular = D3DXCOLOR(-.5f, -.5f, -.5f, 1.0f);
			att.x    = 0.0f;
			att.y    = .1f;
			att.z    = 0.0f;
			range    = 30.0f;
		}

		else { //Pointlight, mostly our room lights
			ambient  = D3DXCOLOR(0.45490196078f, 0.29803921568f, 0.12549019607f, 1.0f);
			diffuse  = D3DXCOLOR(.1f, .1f ,.1f, 1.0f);
			specular = D3DXCOLOR(.1f, .1f, .1f, 1.0f);
			att.x    = 0.0f;
			att.y    = .1f;
			att.z    = 0.0f;
			range    = 50.0f;
		}


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

#endif // LIGHT_H