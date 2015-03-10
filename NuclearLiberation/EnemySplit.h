#pragma once
#include "Enemy.h"

namespace EnemySplitNS{
	const float SPEED_BUFF = 0;
	const float BULLET_SPEED_BUFF = -15;
	const float FIRE_RATE_BUFF = 0;
	const float FIRE_CHANCE_BUFF = -10;
	const float ACTIVATION_RANGE_BUFF = 0;
};

class EnemySplit: public virtual Enemy
{
public:
	EnemySplit():Enemy(){MAX_HEALTH = 50.0f; health = MAX_HEALTH; }
	void setGen(int g);
	int getGen(){return generation;}
	void Fire();
private:
	int generation;
};