#pragma once
#include "Drop.h"

class Power : public virtual Drop
{
public:
	void update(float dt);
	void onDeath();
	void pickUp();
};