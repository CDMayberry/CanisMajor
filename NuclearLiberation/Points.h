#pragma once
#include "Drop.h"

class Points : public virtual Drop
{
public:
	void update(float dt);
	void onDeath();
	void pickUp();
};