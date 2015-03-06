#include"Background.h"
#include "NuclearLiberation.h"

void Background::init(NuclearLiberation* game, float r, float w, float h, D3DXCOLOR color1, D3DXCOLOR color2)
{
	q1.init(game->getDevice(), color1, color2); 
	b1.init(game,&q1,r);
	b1.isActive = true;
	b1.setPosition(Vector3(1*w,1*h,0));
}

void Background::draw(ID3D10EffectMatrixVariable* fx, Matrix& camera, Matrix& projection, ID3D10EffectTechnique* mTech)
{
	
	b1.setScale(Vector3(100,17.5,1));
	

	b1.update(0);
	b1.draw(fx, camera, projection, mTech);
}

void Background::scale(float x, float y, float z)
{
	b1.setScale(Vector3(x,y,z));
}