//=======================================================================================
// Light.h by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#ifndef LIGHT_H
#define LIGHT_H

#include "d3dUtil.h"

struct Light
{
	Light(bool settings = false)
	{
		ZeroMemory(this, sizeof(Light));
		if(settings) {						//Default light source for our game

			// Pointlight--position is changed every frame to animate.
			ambient  = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
			diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			att.x    = 0.0f;
			att.y    = 0.1f;
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