#pragma once
#include "Actor.h"
#include "Quad.h"

class Background 
{
public:
	void init(NuclearLiberation* game, float r, float w, float h, D3DXCOLOR color1, D3DXCOLOR color2);
	void draw(ID3D10EffectMatrixVariable* fx, Matrix& camera, Matrix& projection, ID3D10EffectTechnique* mTech);
	void scale(float x, float y, float z);
private:
	Quad q1;
	Actor b1;
};