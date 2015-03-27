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

	void init(int settings) {	//Default settings! not required to use, just quick setup
		if(settings == 1) {		//Ambient light, world light
			dir      = D3DXVECTOR3(0.57735f, -0.57735f, 0.57735f);
			ambient  = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			diffuse  = D3DXCOLOR(.15f, .15f, .15f, 1.0f);
			specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		}
		else if(settings == 2) { //Pointlight, mostly our room lights
			ambient  = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
			diffuse  = D3DXCOLOR(.5f, .5f ,.5f, 1.0f);
			specular = D3DXCOLOR(.1f, .1f, .1f, 1.0f);
			att.x    = 0.0f;
			att.y    = 0.1f;
			att.z    = 0.0f;
			range    = 50.0f;
		}
		else {					//Spotlight, like our flashlight
			ambient  = D3DXCOLOR(0.2f, 0.2f, 0.15f, 1.0f);
			diffuse  = D3DXCOLOR(.5f, .5f ,.5f, 1.0f);
			specular = D3DXCOLOR(.1f, .1f, .1f, 1.0f);
			att.x    = 1.0f;
			att.y    = 0.0f;
			att.z    = 0.0f;
			spotPow  = 128.0f;
			range    = 10000.0f;

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