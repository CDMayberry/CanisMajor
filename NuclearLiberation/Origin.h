#pragma once
#include "Actor.h"
#include "Line.h"

class Origin
{
public:
	void init(NuclearLiberation* game, float r);
	void draw(ID3D10EffectMatrixVariable* fx, Matrix& camera, Matrix& projection, ID3D10EffectTechnique* mTech);
private:
	Actor x,y,z;
	Line lr,lg,lb;
};