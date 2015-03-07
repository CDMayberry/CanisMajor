#pragma once
#include "Actor.h"

namespace dropNS{

	const float SCALE_X = .5;
	const float SCALE_Y = .5;
	const float SCALE_Z = .5;
	const float despawnTime = 5;
};

class Drop : public virtual Actor
{
public:
	void init(NuclearLiberation* game,Geometry *b, float r){Actor::init(game,b,r); setScale(Vector3(dropNS::SCALE_X,dropNS::SCALE_Y,dropNS::SCALE_Z)); despawner = dropNS::despawnTime;}
	void update(float dt);
private:
	float despawner;
};