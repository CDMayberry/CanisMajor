#pragma once
#include "Enemy.H"

namespace EnemyLightNS{
	const float SPEED_BUFF = -30;
	const float BULLET_SPEED_BUFF = -20;
	const float FIRE_RATE_BUFF = 0;
	const float FIRE_CHANCE_BUFF = 10;
	const float ACTIVATION_RANGE_BUFF = 0;
	const float NUM_BULLETS_IN_CIRCLE = 8;
};

class EnemyLight: public virtual Enemy
{
public:
	EnemyLight():Enemy(){MAX_HEALTH = 50.0f; health = MAX_HEALTH;  MOVEMENT_SPEED = EnemyNS::SPEED_BASE + EnemyLightNS::SPEED_BUFF;}
	void Fire();
private:
};