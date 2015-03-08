#pragma once
#include "Actor.h"

namespace bulletNS{

	const float SCALE_X = .7;
	const float SCALE_Y = .2;
	const float SCALE_Z = .4;
	const float DAMAGE = 25;//Hits for 25 points of damage
};

class Bullet : public virtual Actor
{
public:
	void init(NuclearLiberation* game,Geometry *b, float r){Actor::init(game,b,r); setScale(Vector3(bulletNS::SCALE_X,bulletNS::SCALE_Y,bulletNS::SCALE_Z));}
	void update(float dt);
};