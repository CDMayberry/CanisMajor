#include "EnemySplit.h"
#include "NuclearLiberation.h"
using namespace EnemySplitNS;
void EnemySplit::update(float dt)
{
	if(isActive)
	{
		Vector3 disp = game->player.getPosition()-getPosition();
		if(playerSeen)
		{
			Normalize(&disp,&disp);
			Normalize(&velocity,&velocity);
			velocity += disp*0.5;//move in the general direction of player
			Normalize(&velocity,&velocity);
			velocity*=SPEED;

			cooldown = max(cooldown-dt,0);
			if(cooldown == 0)
			{
				if(rand()%100 < FIRE_CHANCE)
				{
					game->spawnEnemyBullet(getPosition(),disp*BULLET_SPEED);
					cooldown = FIRE_RATE;
				}
				else
					cooldown = rand()%3+1;
			}
		}
		else
		{
			if(Length(&disp) < ACTIVATION_RANGE)
			{
				playerSeen = true;
				velocity = disp;
				Normalize(&velocity,&velocity);
				velocity*=SPEED;
			}
		}
		
		Actor::update(dt);
	}


}


void EnemySplit::setGen(int g){
	generation = g;
	float size = pow(2, 1.0f/g);
	setScale(D3DXVECTOR3(size,size,size));
}