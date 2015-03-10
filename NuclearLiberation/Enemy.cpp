#include "Enemy.h"
#include "NuclearLiberation.h"
using namespace EnemyNS;

void Enemy::update(float dt)
{
    if(isActive)
    {
        Vector3 disp = game->player.getPosition()-getPosition();
        if(playerSeen)
        {
            Normalize(&disp,&disp);
            if (abs(disp.x) > abs(disp.y))
                disp.y = 0;
            else
                disp.x = 0;
            Normalize(&velocity,&velocity);
            velocity += disp*0.5;//move in the general direction of player
            Normalize(&velocity,&velocity);
            velocity*=(MOVEMENT_SPEED);

            cooldown = max(cooldown-dt,0);
            if(cooldown == 0)
            {
                Fire();
            }
        }
        else
        {
            if(Length(&disp) < (ACTIVATION_RANGE_BASE))
            {
                playerSeen = true;
                velocity = disp;
                Normalize(&velocity,&velocity);
                velocity*=(MOVEMENT_SPEED);
            }
        }
        
		if( game->getFloor(getPosition().x) >
			getPosition().y){
			position.y = game->getFloor(position.x);
		}
		else if (game->getCeiling(getPosition().x) < getPosition().y){
			position.y = game->getCeiling(position.x);
		}
			

        Actor::update(dt);
    }


}


void Enemy::onDeath() {
	float h = 1 - getPosition().y/game->worldSize.y;
	bool flip = rand()%2;
	int rander = random(3);
	switch(rander) {
	case 1:
		game->audio->playCue(EXP1);
		if(rand()%100 <= h*100 && flip)
			game->spawnAir(getPosition(), Vector3(0,1,0));
		else if(rand()%100 == h*100 && flip)
			game->spawnShield(getPosition(), Vector3(0,0,0));
		else if(rand()%100 == h*100 && flip)
			game->spawnPower(getPosition(), Vector3(0,-2,0));
		break;
	case 2:
		game->audio->playCue(EXP2);
		if(rand()%100 <= h*100 && flip)
			game->spawnPower(getPosition(), Vector3(0,1,0));
		else if(rand()%100 == h*100 && flip)
			game->spawnAir(getPosition(), Vector3(0,0,0));
		else if(rand()%100 == h*100 && flip)
			game->spawnShield(getPosition(), Vector3(0,-2,0));
		break;
	case 3:
		game->audio->playCue(EXP3);
		if(rand()%100 <= h*100 && flip)
			game->spawnShield(getPosition(), Vector3(0,1,0));
		else if(rand()%100 == h*100 && flip)
			game->spawnPower(getPosition(), Vector3(0,0,0));
		else if(rand()%100 == h*100 && flip)
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