#pragma once
#include "Enemy.H"

namespace EnemyLightNS{
	const float SPEED_BUFF = 10;
	const float BULLET_SPEED_BUFF = 0;
	const float FIRE_RATE_BUFF = 0;
	const float FIRE_CHANCE_BUFF = 10;
	const float ACTIVATION_RANGE_BUFF = 0;
};

class EnemyLight: public virtual Enemy
{
public:
	EnemyLight():Enemy(){MAX_HEALTH = 50.0f; health = MAX_HEALTH; }
	void update(float dt);
private:
};