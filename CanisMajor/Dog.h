#ifndef _DOG_H
#define _DOG_H

#include "Actor.h"
#include "Light.h"

namespace dogNS{
	const float WALK_SPEED = 5.0;
	const float RUN_SPEED = 15.0f;
	const float AGRO_DIST = 7.0f;//distance to start dog agro
	const float NEUTRAL_DIST = 20.0f;//distance to leave agro field
};

class Dog: public virtual Actor
{
public:
	void SetWaypoints(Vector3* wp, int numwp);//allows levels to set the dog's waypoint system
	void update(float dt);
	void Dog::init(CanisMajor* game,Geometry *b,  float r, Vector3 scale = Vector3(1,1,1));
	void setNegalight(Light* nega) {negalight = nega;}
	void setEyes(Light* eyes) {rEyes = eyes;}
private:
	void targetClosestWaypoint();
	int TargetWaypoint;//waypoint the dog is traveling to. set to -1 if tracking player
	int numwaypoints;
	Light* negalight;
	Light* rEyes;
	bool following;//following player
	Vector3 * Waypoints;//pointer to array of waypoints
};
#endif