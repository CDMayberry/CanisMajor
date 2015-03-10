#include "Enemy.h"
#include "NuclearLiberation.h"
using namespace EnemyNS;

void Enemy::onDeath() {
	float h = 1 - getPosition().y/game->worldSize.y;
	int rander = random(3);
	switch(rander) {
	case 1:
		game->audio->playCue(EXP1);
		if(rand()%100 <= h*100)
			game->spawnAir(getPosition(), Vector3(0,1,0));
		else if(rand()%100 == h*100)
			game->spawnShield(getPosition(), Vector3(0,0,0));
		else if(rand()%100 == h*100)
			game->spawnPower(getPosition(), Vector3(0,-2,0));
		break;
	case 2:
		game->audio->playCue(EXP2);
		if(rand()%100 <= h*100)
			game->spawnPower(getPosition(), Vector3(0,1,0));
		else if(rand()%100 == h*100)
			game->spawnAir(getPosition(), Vector3(0,0,0));
		else if(rand()%100 == h*100)
			game->spawnShield(getPosition(), Vector3(0,-2,0));
		break;
	case 3:
		game->audio->playCue(EXP3);
		if(rand()%100 <= h*100)
			game->spawnShield(getPosition(), Vector3(0,1,0));
		else if(rand()%100 == h*100)
			game->spawnPower(getPosition(), Vector3(0,0,0));
		else if(rand()%100 == h*100)
			game->spawnAir(getPosition(), Vector3(0,-2,0));
		break;
	default:
		game->audio->playCue(EXP1);
		if(rand()%100 <= h*100)
			game->spawnShield(getPosition(), Vector3(0,1,0));
		else if(rand()%100 == h*100)
			game->spawnPower(getPosition(), Vector3(0,0,0));
		else if(rand()%100 == h*100)
			game->spawnAir(getPosition(), Vector3(0,-2,0));
		break;
	}


}