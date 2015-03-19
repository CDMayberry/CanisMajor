#pragma once
#include "Geometry.h"
class Coin : public virtual Geometry
{
public:
	void init(ID3D10Device* device, D3DXCOLOR color = WHITE);

};