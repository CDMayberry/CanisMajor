#pragma once

#include "Actor.h"
#include "Interactable.h"

namespace staffNS{
	const float BOLT_COOLDOWN = .5;
	const float BOLT_SPEED = 40;
	const Vector3 FIRE_OFFSET(0,1,0);
};

class Staff: public virtual Actor, public virtual Interactable{
public:
	
	void init(CanisMajor* game,Geometry *b);
	virtual std::wstring getUtilText(){return L"Press LMB to take the " + name;}
	void update(float dt);

	void setDirection(Vector3 v);
	void interactWith(Camera* c);

	void fireBolt();

private:
	float boltCooldown;
	Vector3 direction;
};