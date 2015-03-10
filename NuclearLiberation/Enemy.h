#pragma once
#include "Actor.h"

namespace EnemyNS{
	const float SPEED_BASE = 30;
	const float BULLET_SPEED_BASE = 25;
	const float FIRE_RATE_BASE = 1;
	const float FIRE_CHANCE_BASE = 50;
	const float ACTIVATION_RANGE_BASE = 100;
	const float SHL_RATE = 75;
	const float AIR_RATE = 100;
	const float POW_RATE = 75;
};

class Enemy: public virtual Actor
{
public:
	Enemy():Actor(){playerSeen=false;cooldown=0;MAX_HEALTH = 100.0f; health = MAX_HEALTH; MOVEMENT_SPEED = EnemyNS::SPEED_BASE;}
	virtual void update(float dt);
	void onDeath();
	virtual void Fire() = 0;
protected:
	float MOVEMENT_SPEED;
	float cooldown;
	bool playerSeen;
};