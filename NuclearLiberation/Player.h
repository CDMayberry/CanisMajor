#pragma once
#include"Actor.h"
#include"Controls.h"
#include "mathUtil.h"

class NuclearLiberation;

namespace playerNS{
	const float MAX_SPEED = 55;
	const float FRICTION_RATE = 100;
	const float ACCEL_RATE = 80;
	const float DEFAULT_COOLDOWN = 0.2;
	const float MAX_ROTATION_ANGLE = 1;
	const float ROTATION_SPEED = 0.25;
	const Vector3 FIRE_SPEED(75,0,0);
	const float MAX_AIR = 30; //AIR DEPLETES AT 1 UNIT PER SECOND
};

class Player : public virtual Actor
{
public:

	void init(NuclearLiberation*game,Geometry *b, float r, Controls c);
	void update(float dt);

	void refillAir(){airLevel = playerNS::MAX_AIR;}
	float getAir(){return airLevel;}
	float topDef(float x);
	float bottomWall(float x);
	bool inGap();

private:
	Vector3 input;
	Controls controls;
	int weaponLevel;
	float weaponCooldown;
	float bulletDisp;
	Vector3 accel;
	Vector3 oldPos;
	float airLevel;
};