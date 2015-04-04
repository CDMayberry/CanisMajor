#include "Dog.h"
#include "CanisMajor.h"

void Dog::init(CanisMajor* game,Geometry *b,  float r)
{
	Actor::init(game,b,r);
	TargetWaypoint = -1;
}

void Dog::SetWaypoints(Vector3* wp, int numwp){
	Waypoints = wp;
	numwaypoints = numwp;
	TargetWaypoint = 0;
}

void Dog::update(float dt){
	if(isActive)
	{
		//write waypoint system here for transitioning between points
		if (TargetWaypoint != -1){
			//check if we've reached a waypoint
			Vector3 twp = Waypoints[TargetWaypoint];//target waypoint
			float distsqrt = (twp.x - position.x)*(twp.x - position.x) + (twp.y - position.y)*(twp.y - position.y) + (twp.z - position.z)*(twp.z - position.z);
			if (distsqrt < 0.25){
				TargetWaypoint++;
				if (TargetWaypoint == numwaypoints)
					TargetWaypoint = 0;//wrap around to first waypoint
			}

			twp = Waypoints[TargetWaypoint];
			//vector track to waypoint
			velocity.x = twp.x - position.x;
			velocity.y = twp.y - position.y;
			velocity.z = twp.z - position.z;
			D3DXVec3Normalize(&velocity,&velocity);
			velocity*=5;
		}
		Actor::update(dt);
	}
}