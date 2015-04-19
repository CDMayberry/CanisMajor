#include "Dog.h"
#include "CanisMajor.h"
#include <fstream>
#include <time.h>
using namespace std;

using namespace dogNS;

void Dog::init(CanisMajor* game,Geometry *b,  float r, Vector3 s)
{
	Actor::init(game,b,r, s);
	TargetWaypoint = -1;
	following = false;
	numwaypoints = 0;
	waypointdir = 1;
	srand(time(NULL));
}

void Dog::SetWaypoints(Vector3* wp, int numwp, int LinInterp){
	if (numwaypoints != 0 ){
		delete [] Waypoints;
		numwaypoints = 0;
	}
	if(numwp ==0)
		return;

	//ofstream myfile;
	//myfile.open("C:\\users\\bowmanrs1\\Desktop\\wp.txt");
	numwaypoints = numwp*LinInterp;
	TargetWaypoint = 0;
	Waypoints = new Vector3[numwp*LinInterp];
	for (int i=0;i<numwp*LinInterp;i++){
		if (i%LinInterp==0 && LinInterp !=1)
			Waypoints[i] = Vector3(wp[i/LinInterp].x,wp[i/LinInterp].y,wp[i/LinInterp].z);
		else if (i >= (numwp*LinInterp) - (LinInterp-1)){
			Vector3 tempwp1, tempwp2;
			tempwp1 = Vector3(wp[(i/LinInterp)].x,wp[(i/LinInterp)].y,wp[(i/LinInterp)].z);
			tempwp2 = Vector3(wp[0].x,wp[0].y,wp[0].z);
			Waypoints[i] = tempwp1 + (tempwp2-tempwp1)*(1.0f/LinInterp);
		}
		else{
			Vector3 tempwp1, tempwp2;
			tempwp1 = Vector3(wp[(i/LinInterp)].x,wp[(i/LinInterp)].y,wp[(i/LinInterp)].z);
			tempwp2 = Vector3(wp[(i/LinInterp)+1].x,wp[(i/LinInterp)+1].y,wp[(i/LinInterp)+1].z);
			Waypoints[i] = tempwp1 + (tempwp2-tempwp1)*(1.0f/LinInterp)*(i%LinInterp);
		}

		//myfile << Waypoints[i].x << ", "<< Waypoints[i].y << ", " << Waypoints[i].z<<"\n";
	}
	//myfile.close();

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



		if (D3DXVec2Length(&vectortoplayer) <= 4){
			#ifndef DEBUG
				game->playSound(BITE,position);
				game->onPlayerDeath();//the dog caught the player, died
			#endif
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
				if (distsqrt < 0.1){//dog has hit a waypoint, procceed to next wp
					TargetWaypoint+= waypointdir;
					if (TargetWaypoint == numwaypoints)
						TargetWaypoint = 0;//wrap around to first waypoint
					if (TargetWaypoint == -1)
						TargetWaypoint = numwaypoints-1;//wrap around to last waypoint

					int switchdir = rand()%100;
					if (switchdir <= DIR_CHANGE_CHANCE){//1 in 4 chance to switch direction at any given waypoint
						waypointdir *= -1;
					}
				
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
