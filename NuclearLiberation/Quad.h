#pragma once
#include "Geometry.h"


class Quad : public virtual Geometry
{
public:
	void init(ID3D10Device* device, D3DXCOLOR color1, D3DXCOLOR color2);

};

