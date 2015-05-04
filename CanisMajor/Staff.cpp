#include "Staff.h"
#include "Camera.h"
#include "CanisMajor.h"

using namespace staffNS;
using namespace interactableNS;

void Staff::init(CanisMajor* game,Geometry *b){
	Actor::init(game,b);
	boltCooldown=0;
	direction = Vector3(1,0,0);
	collisionType = AABBox;
	sprite = HAND_SPRITE;
	name = L"Staff";
}
void Staff::update(float dt){
	if(isActive){	
		boltCooldown = max(boltCooldown-dt,0);
		if(boltCooldown>0){
			Vector3 rot = getRotation();
			float x = boltCooldown/BOLT_COOLDOWN;
			float swingAmt = -pow(((x-.5)/.5),2)+1;
			rot.z = asin(direction.y) - PI/4 + swingAmt*PI/8;
			setRotation(rot);
		}
		Actor::update(dt);
	}
}

void Staff::setDirection(Vector3 v){
	Normalize(&v,&v);
	direction = v;
	
	float rz = asin(v.y);
	float ry = -atan2(v.z,v.x);

	setRotation(Vector3(0,ry,rz-PI/4));
}
void Staff::interactWith(Camera* c){
	Interactable::interactWith(c);
	c->setStaff(this);
	game->setStoryText(8,L"What is this? Why does it feel... powerful...\nPress RMB");
}

void Staff::fireBolt(){
	if(boltCooldown==0){
		boltCooldown=BOLT_COOLDOWN;
		game->spawnBolt(getPosition()+FIRE_OFFSET+direction,direction*BOLT_SPEED);
		game->playSound(MAGIC_MISSILE,getPosition());
	}
}