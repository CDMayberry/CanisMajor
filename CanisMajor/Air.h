#pragma once
#include "Drop.h"

class Air : public virtual Drop
{
public:
	void update(float dt);
	void onDeath();
	void pickUp();
};