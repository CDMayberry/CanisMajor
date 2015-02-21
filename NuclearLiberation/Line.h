#pragma once
#include "Geometry.h"


class Line : public virtual Geometry
{
public:

	void init(ID3D10Device* device);
	void draw();
};
