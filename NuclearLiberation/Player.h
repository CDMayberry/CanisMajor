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
	const Vector3 HELIX_DISP(3,0,0);
	const int MAX_WEAPON_LEVEL = 6;
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

	void resetAll(){weaponLevel=1;weaponCooldown=0;fireCounter=0;refillAir();}
	void refresh(){weaponCooldown=0;fireCounter=0;refillAir();}

	void grantWeaponLevel(){weaponLevel = min(weaponLevel+1,playerNS::MAX_WEAPON_LEVEL);}

	void onDeath();

private:
	Vector3 input;
	Controls controls;
	int weaponLevel;
	float weaponCooldown;
	Vector3 accel;
	Vector3 oldPos;
	float airLevel;
	int fireCounter;//used to distinguish shots when firing
	float fireAngle;//used to make helix
};