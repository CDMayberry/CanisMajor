#include "Dog.h"
#include "CanisMajor.h"

using namespace dogNS;

void Dog::init(CanisMajor* game,Geometry *b,  float r, Vector3 s)
{
	Actor::init(game,b,r, s);
	TargetWaypoint = -1;
	following = false;
}

void Dog::SetWaypoints(Vector3* wp, int numwp){
	Waypoints = wp;
	numwaypoints = numwp;
	TargetWaypoint = 0;
}

void Dog::update(float dt){
	if(isActive)
	{
		temp = getPosition();
		if(negalight != nullptr)
			negalight->pos = position;
		if(rEyes != nullptr)
			rEyes->pos = position+Vector3(-.15,1.5*scale.y,0);

		//rotate so that the dog is always facing the player
		//distance from dog to player
		Vector2 vectortoplayer = Vector2(game->getCamera().getPosition().x - position.x, game->getCamera().getPosition().z - position.z);
		double facerot = acos((game->getCamera().getPosition().x - position.x)/(D3DXVec2Length(&vectortoplayer)));
		if (game->getCamera().getPosition().z <= position.z)
			setRotation(Vector3(0,facerot,0));
		else
			setRotation(Vector3(0,-facerot,0));


		//if player is < 5 units from the dog, begin chase
		//using vector tracking. If player exceeds 10 units,
		//stop chasing and return to waypoint path
		if (D3DXVec2Length(&vectortoplayer) <= 0.5f){
			game->playSound(BITE,position);
			game->onPlayerDeath();//the dog caught the player, died
		}
		else if (playerNearby == &game->camera && following == false){
			following = true;
		}
		else if (playerNearby != &game->camera && following ==true){
			following = false;
			targetClosestWaypoint();//reset waypoint if need be
		}

		if (following){//begin chase and start running!
			velocity.x = vectortoplayer.x;
			velocity.y = 0;
			velocity.z = vectortoplayer.y;
			D3DXVec3Normalize(&velocity, &velocity);
			velocity = velocity*RUN_SPEED;
			following = true;
		}
		else {//default to waypoint system
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
				velocity*=WALK_SPEED;
			}
		}
		direction = getPosition() - temp;

		if(position.x - game->camera.getPosition().x < 15 && position.z - game->camera.getPosition().z < 15)
		{
			if(!game->waiting)
			{
				game->start = clock();
				game->waiting = true;
			}
			game->current = clock();

			if(game->current - game->start > 10000)
			{
				game->playSound(DOGGROWL,position);
				game->waiting = false;
			}
		}

		dirToPlayer = game->camera.getPosition() - getPosition();
		Normalize(&dirToPlayer, &dirToPlayer);
		Actor::update(dt);
	}
}

void Dog::targetClosestWaypoint(){
	int waypointindex = 0;//index of shortest waypoint
	float waypointdist = 9999;//distance of shortest waypoint

	for (int i=0;i<numwaypoints;i++){
		Vector3 twp = Waypoints[i];
		float distsqrt = (twp.x - position.x)*(twp.x - position.x) + (twp.y - position.y)*(twp.y - position.y) + (twp.z - position.z)*(twp.z - position.z);
		
		if (distsqrt < waypointdist){
			waypointindex = i;
			waypointdist = distsqrt;
		}
	}

	TargetWaypoint = waypointindex;//set the new target waypoint to the closest waypoint from dog's current position
}

bool Dog::isPicked(Actor* o, float & distance)
{
	if(o->isActive && o->isVisible)
	{
		//if they are close
		if(D3DXVec3LengthSq(&(position - o->getPosition())) <= dogNS::AGRO_DIST)
		{
			return o->rayCollidesWith(getPosition(),dirToPlayer,distance);
		}
	}
	return false;
}

void Dog::setNearest(Actor* isPlayer, float dist)
{
	if(distToNearestObj > dist)
	{
		distToNearestObj = dist;
		playerNearby = isPlayer;
	}
}

void Dog::resetNearest()
{
	playerNearby=nullptr;
	distToNearestObj=dogNS::AGRO_DIST;
}
