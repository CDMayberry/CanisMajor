#pragma once
#include "Enemy.h"

namespace EnemyBoatNS{
	const float COOLDOWN = 0.3;
	const float BOAT_HEALTH = 20;
	const float BULLET_SPEED = 50;
	const float ACTIVATION_RANGE = 50;
};

class EnemyBoat : public virtual Enemy
{
public:
	void update(float dt);
	void create(Vector3 pos);
};