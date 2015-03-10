#pragma once
#include "Enemy.h"

namespace EnemyHeavyNS{
	const float SPEED_BUFF = -30;
	const float BULLET_SPEED_BUFF = 0;
	const float FIRE_RATE_BUFF = -0.7;
	const float FIRE_CHANCE_BUFF = 500;
	const float ACTIVATION_RANGE_BUFF = 50;
};

class EnemyHeavy: public virtual Enemy
{
public:
	EnemyHeavy():Enemy(){MAX_HEALTH = 100.0f; health = MAX_HEALTH; BulletSpiralTheta = 0.0f; MOVEMENT_SPEED = EnemyNS::SPEED_BASE + EnemyHeavyNS::SPEED_BUFF;}
	void Fire();
private:
	float BulletSpiralTheta;
};