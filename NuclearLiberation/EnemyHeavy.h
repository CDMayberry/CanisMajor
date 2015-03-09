#pragma once
#include "Enemy.h"

namespace EnemyHeavyNS{
	const float SPEED_BUFF = -10;
	const float BULLET_SPEED_BUFF = 0;
	const float FIRE_RATE_BUFF = -0.9;
	const float FIRE_CHANCE_BUFF = 500;
	const float ACTIVATION_RANGE_BUFF = 50;
};

class EnemyHeavy: public virtual Enemy
{
public:
	EnemyHeavy():Enemy(){MAX_HEALTH = 100.0f; health = MAX_HEALTH; BulletSpiralTheta = 0.0f; }
	void update(float dt);
private:
	float BulletSpiralTheta;
};