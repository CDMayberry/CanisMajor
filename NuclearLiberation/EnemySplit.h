#pragma once
#include "Actor.h"

namespace EnemySplitNS{
	const float SPEED = 30;
	const float BULLET_SPEED = 50;
	const float FIRE_RATE = 1;
	const float FIRE_CHANCE = 50;
	const float ACTIVATION_RANGE = 100;
};

class EnemySplit: public virtual Actor
{
public:
	EnemySplit():Actor(){playerSeen=false;cooldown=0;MAX_HEALTH = 50.0f; health = MAX_HEALTH; }
	void update(float dt);
private:
	float cooldown;
	bool playerSeen;
};