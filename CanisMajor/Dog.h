#ifndef _DOG_H
#define _DOG_H

#include "Actor.h"
#include "Light.h"
#include "QuestItem.h"

namespace dogNS{
	const float WALK_SPEED = 7.0;
	const float RUN_SPEED = 32.0f;
	const float AGRO_DIST = 2200;//distance to start dog agro
	const float NEUTRAL_DIST = 20.0f;//distance to leave agro field
	const int DIR_CHANGE_CHANCE = 10;// 10 percent chance of changing direction at any given waypoint
	const int MAX_NUM_WAYPOINTS = 1000;
};

class Dog: public virtual Actor
{
public:
	void LoadWaypoints(string wpfilename, int LinInterp, int stage);//allows levels to set the dog's waypoint system
	void update(float dt);
	void Dog::init(CanisMajor* game,Geometry *b,  float r, Vector3 scale = Vector3(1,1,1));
	void setNegalight(Light* nega) {negalight = nega;}
	void setEyes(Light* eyes) {rEyes = eyes;}
	bool isPicked(Actor* o, float &distance);
	void resetNearest();
	void setNearest(Actor* isPlayer, float distance);
	void setTargetWPStage(int s){TargetWPStage = s;}
	Vector3 getDirection(){return direction;}
	void giveItem(QuestItem* q){heldItem=q;}
	void onDeath();
private:
	QuestItem* heldItem;
	bool isGrowling;
	void targetClosestWaypoint();
	int TargetWaypoint;//waypoint the dog is traveling to. set to -1 if tracking player
	int TargetWPStage;
	int numwaypoints[10];
	Vector3 direction,temp;
	Light* negalight;
	Light* rEyes;
	bool following;//following player
	Vector3 Waypoints[10][dogNS::MAX_NUM_WAYPOINTS];//2d array of waypoints. [stage][waypoint] Be sure numwaypoints does not exceed 1000.
	Vector3 wp[dogNS::MAX_NUM_WAYPOINTS];//temporary waypoint list of waypoints
	float distToNearestObj;
	bool isNearObjPlayer;
	Actor* playerNearby;
	Vector3 dirToPlayer;
	int waypointdir;//indicates whether the dog is moving clockwise or counterclockwise through the waypoints
};
#endif