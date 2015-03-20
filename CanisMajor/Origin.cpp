#include"Origin.h"
#include "CanisMajor.h"

void Origin::init(CanisMajor* game, float r)
{
	lr.init(game->getDevice(),RED);
	lg.init(game->getDevice(),GREEN);
	lb.init(game->getDevice(),WHITE);
	x.init(game,&lr,r);
	y.init(game,&lg,r);
	z.init(game,&lb,r);
	x.isActive = y.isActive = z.isActive = true;
}

void Origin::draw(ID3D10EffectMatrixVariable* fx, Matrix& camera, Matrix& projection, ID3D10EffectTechnique* mTech)
{
	x.setScale(Vector3(100,0,0));
	y.setScale(Vector3(100,0,0));
	z.setScale(Vector3(100,0,0));

	x.setRotation(Vector3(0,0,0));
	y.setRotation(Vector3(0,0,PI/2));
	z.setRotation(Vector3(0,-PI/2,0));

	x.update(0);
	y.update(0);
	z.update(0);

	x.draw(fx,camera,projection,mTech);
	y.draw(fx,camera,projection,mTech);
	z.draw(fx,camera,projection,mTech);
}