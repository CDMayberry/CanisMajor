#pragma once
#include"Actor.h"
#include"Controls.h"

class NuclearLiberation;

namespace playerNS{
	const float MAX_SPEED = 40;
	const float DEFAULT_COOLDOWN = 0.5;
};

class Player : public virtual Actor
{
public:

	void init(NuclearLiberation*game,Geometry *b, float r, Controls c);
	void update(float dt);

private:
	Vector3 input;
	Controls controls;
	int weaponLevel;
	float weaponCooldown;
	Vector3 accel;
	float rotation;
	
};