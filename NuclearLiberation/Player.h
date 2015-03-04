#pragma once
#include"Actor.h"
#include"Controls.h"
#include "mathUtil.h"

class NuclearLiberation;

namespace playerNS{
	const float MAX_SPEED = 40;
	const float FRICTION_RATE = 30;
	const float ACCEL_RATE = 80;
	const float DEFAULT_COOLDOWN = 0.2;
	const float MAX_ROTATION_ANGLE = 1;
	const float ROTATION_SPEED = 0.5;
	const Vector3 FIRE_SPEED(75,0,0);
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
	float bulletDisp;
	Vector3 accel;
};