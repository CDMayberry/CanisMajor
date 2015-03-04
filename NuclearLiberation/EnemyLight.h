#pragma once
#include "Actor.h"

namespace EnemyLightNS{
	const float SPEED = 30;
	const float BULLET_SPEED = 50;
	const float FIRE_RATE = 1;
	const float FIRE_CHANCE = 50;
	const float ACTIVATION_RANGE = 100;
};

class EnemyLight: public virtual Actor
{
public:
	EnemyLight():Actor(){playerSeen=false;cooldown=0;}
	void update(float dt);
private:
	float cooldown;
	bool playerSeen;
};