#include "Flashlight.h"
#include "Camera.h"
using namespace flashlightNS;

void Flashlight::init(CanisMajor* game,Geometry *b, Light* l, float r)
{
	Actor::init(game,b,r);
	light = l;
	batteryLevel = MAX_BATTERY;
	direction = Vector3(1,0,0);
	isOn=false;
	name = L"flashlight";
}
void Flashlight::update(float dt)
{
	if(isActive)
	{
		light->dir = direction;
		light->pos = getPosition();
		if(isOn)
		{
			batteryLevel = max(0,batteryLevel-dt);
			float lightLevel = min(1,batteryLevel/START_BATTERY_FALLOFF);
			light->ambient = lightLevel*DEF_AMBIENT;
			light->diffuse = lightLevel*DEF_DIFFUSE;
			light->specular = lightLevel*DEF_SPECULAR;
		}
		Actor::update(dt);
	}
}

void Flashlight::setDirection(Vector3 v)
{
#ifndef DEBUG
	v.y=0;
#endif
	Normalize(&v,&v);
	direction = v;
	//float rz = asin(v.y);
	float ry = -atan2(v.z,v.x);
	setRotation(Vector3(PI/2,ry,0));
}

void Flashlight::interactWith(Camera* player)
{
	player->setFlashlight(this);
}

void Flashlight::recharge(float dt)
{
	batteryLevel=min(MAX_BATTERY,batteryLevel+dt*RECHARGE_RATE);
}