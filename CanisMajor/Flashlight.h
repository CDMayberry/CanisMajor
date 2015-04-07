#pragma once

#include "Actor.h"
#include "Light.h"
#include "Interactable.h"


namespace flashlightNS{
	const float MAX_BATTERY = 100;
	const float START_BATTERY_FALLOFF = 30;
	const Vector3 DEFAULT_DIRECTION(1,0,0);
	const D3DXCOLOR DEF_AMBIENT(0.2f, 0.2f, 0.15f, 1.0f);
	const D3DXCOLOR DEF_DIFFUSE(.5f, .5f ,.5f, 1.0f);
	const D3DXCOLOR DEF_SPECULAR(.1f, .1f, .1f, 1.0f);

	const float RECHARGE_RATE = 30;
};

class Flashlight : public virtual Actor, public Interactable
{
public:
	void init(CanisMajor* game,Geometry *b, Light* l, float r=1);
	void update(float dt);

	void setDirection(Vector3 v);

	void interactWith(Camera* c);

	void toggle(){isOn=!isOn;}
	bool isOn;
	void recharge(float dt);
private:
	Light* light;
	Vector3 direction;
	float batteryLevel;
	
};