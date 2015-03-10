#pragma once
#include "Actor.h"

namespace bulletNS{

	const float SCALE_X = .7;
	const float SCALE_Y = .2;
	const float SCALE_Z = .4;
	const float LINESPAN = 1;
	const float DAMAGE = 1;//Hits for 25 points of damage
};

class Bullet : public virtual Actor
{
private:
	float lifeTime;
public:
	void create(Vector3 pos){Actor::create(pos);lifeTime = 0;}
	void init(NuclearLiberation* game,Geometry *b, float r){Actor::init(game,b,r); setScale(Vector3(bulletNS::SCALE_X,bulletNS::SCALE_Y,bulletNS::SCALE_Z));}
	void update(float dt);
};