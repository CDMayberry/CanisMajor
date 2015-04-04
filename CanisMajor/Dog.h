#ifndef _DOG_H
#define _DOG_H

#include "Actor.h"
#include "Light.h"

namespace dogNS{
	const Vector3 maxmovespeed = Vector3(1.0f,1.0f,1.0f);
};

class Dog: public virtual Actor
{
public:
	void SetWaypoints(Vector3* wp, int numwp);//allows levels to set the dog's waypoint system
	void update(float dt);
	void Dog::init(CanisMajor* game,Geometry *b,  float r);
	void setNegalight(Light* nega) {negalight = nega;}
private:
	int TargetWaypoint;//waypoint the dog is traveling to. set to -1 if tracking player
	int numwaypoints;
	Light* negalight;
	Vector3 * Waypoints;//pointer to array of waypoints
};
#endif