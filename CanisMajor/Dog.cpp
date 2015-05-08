#include "Dog.h"
#include "CanisMajor.h"
#include <fstream>
#include <iostream>
#include <time.h>
using namespace std;

using namespace dogNS;

void Dog::init(CanisMajor* game,Geometry *b,  float r, Vector3 s)
{
	Actor::init(game,b,r, s);
	TargetWaypoint = -1;
	TargetWPStage = 0;
	following = false;
	for (int i=0;i<10;i++){
		numwaypoints[i] = 0;
	}
	waypointdir = 1;
	srand(time(NULL));

	audioData3D = game->audio->buildData(LOW_GROWL);
	isGrowling = false;
	//game->audio->playCue(audioData3D);
}


void Dog::LoadWaypoints(string wpfilename, int LinInterp, int stage){
	//parse file for original waypoint verticies
	string line;
	ifstream myfile(wpfilename);
	int numwp = 0;
	if (myfile){
		while(getline(myfile,line)){
			std::size_t found = line.find("#");
			char * tok;
			char * c = strdup(line.c_str());
			if (found == std::string::npos){//if the line is not a comment
				tok = strtok(c,",");
				float x = atof(tok);
				tok = strtok(NULL,",");
				float y = atof(tok);
				tok = strtok(NULL,",");
				float z = atof(tok);
				wp[numwp] = Vector3(x,y,z);
				numwp++;
			}
		}
	}
	myfile.close();

	//interpolate
	TargetWaypoint = 0;
	for (int i=0;i<numwp*LinInterp;i++){
		if (i%LinInterp==0 && LinInterp !=1)
			Waypoints[stage][i] = Vector3(wp[i/LinInterp].x,wp[i/LinInterp].y,wp[i/LinInterp].z);
		else if (i >= (numwp*LinInterp) - (LinInterp-1)){
			Vector3 tempwp1, tempwp2;
			tempwp1 = Vector3(wp[(i/LinInterp)].x,wp[(i/LinInterp)].y,wp[(i/LinInterp)].z);
			tempwp2 = Vector3(wp[0].x,wp[0].y,wp[0].z);
			Waypoints[stage][i] = tempwp1 + (tempwp2-tempwp1)*(1.0f/LinInterp);
		}
		else{
			Vector3 tempwp1, tempwp2;
			tempwp1 = Vector3(wp[(i/LinInterp)].x,wp[(i/LinInterp)].y,wp[(i/LinInterp)].z);
			tempwp2 = Vector3(wp[(i/LinInterp)+1].x,wp[(i/LinInterp)+1].y,wp[(i/LinInterp)+1].z);
			Waypoints[stage][i] = tempwp1 + (tempwp2-tempwp1)*(1.0f/LinInterp)*(i%LinInterp);
		}
	}

	numwaypoints[stage] = numwp*LinInterp;

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

		if(game->birminghamMode)
			playerNearby = nullptr;

		#if defined(DEBUG) || defined(DEBUG)
		playerNearby = nullptr;

#endif



		if (D3DXVec2Length(&vectortoplayer) <= 4){
			#if !defined(_DEBUG) || !defined(DEBUG)
			if(!game->birminghamMode){
				game->playSound(BITE,position);
				game->onPlayerDeath();//the dog caught the player, died
			}
				
			#endif
		}
		else if (playerNearby == &game->camera && following == false){
			following = true;
			game->audio->updateCue(&audioData3D,BARKING);
			game->audio->playCue(audioData3D);
		}
		else if ((playerNearby != &game->camera && following ==true)){
			following = false;
			targetClosestWaypoint();//reset waypoint if need be
			game->audio->updateCue(&audioData3D,LOW_GROWL);
			game->audio->playCue(audioData3D);
		}

		if (following && !game->birminghamMode){//begin chase and start running!
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

				Vector3 twp = Waypoints[TargetWPStage][TargetWaypoint];//target waypoint
				float distsqrt = (twp.x - position.x)*(twp.x - position.x) + (twp.y - position.y)*(twp.y - position.y) + (twp.z - position.z)*(twp.z - position.z);
				if (distsqrt < 0.1){//dog has hit a waypoint, procceed to next wp
					TargetWaypoint+= waypointdir;
					if (TargetWaypoint == numwaypoints[TargetWPStage])
						TargetWaypoint = 0;//wrap around to first waypoint
					if (TargetWaypoint == -1)
						TargetWaypoint = numwaypoints[TargetWPStage]-1;//wrap around to last waypoint

					int switchdir = rand()%100;
					if (switchdir <= DIR_CHANGE_CHANCE){//1 in 4 chance to switch direction at any given waypoint
						waypointdir *= -1;
					}
				
				}

				twp = Waypoints[TargetWPStage][TargetWaypoint];
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
				game->start = clock(); //Why is this using a system call???
				game->waiting = true;
			}
			game->current = clock(); //No. No really. Why? We have float dt for a reason.

			if(game->current - game->start > 10000) //PLEASE FIX THIS. USE DT.
			{
				//game->playSound(DOGGROWL,position);
				game->waiting = false;
			}
		}

		dirToPlayer = game->camera.getPosition() - getPosition();
		Normalize(&dirToPlayer, &dirToPlayer);
		Actor::update(dt);
		
		if(!isGrowling)
		{
			isGrowling = true;
			game->audio->updateCue(&audioData3D,LOW_GROWL);
			game->audio->playCue(audioData3D);
		}
		else
			audioData3D->update(getPosition());
	}
	else
	{
		if(isGrowling)
		{
			game->audio->stopCue(audioData3D);
			isGrowling=false;
		}
	}
}

void Dog::targetClosestWaypoint(){
	int waypointindex = 0;//index of shortest waypoint
	float waypointdist = 9999;//distance of shortest waypoint

	for (int i=0;i<numwaypoints[TargetWPStage];i++){
		Vector3 twp = Waypoints[TargetWPStage][i];
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

void Dog::onDeath(){
	isActive=false;
	negalight->pos = rEyes->pos = Vector3(1000,1000,1000);
	
	if(heldItem!=nullptr){
		heldItem->setPosition(getPosition());
	}
}