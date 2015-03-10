#include "EnemySplit.h"
#include "NuclearLiberation.h"
using namespace EnemySplitNS;
using namespace EnemyNS;
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

			velocity*=(SPEED_BASE + SPEED_BUFF);

			cooldown = max(cooldown-dt,0);
			if(cooldown == 0)
			{
				if(rand()%100 < (FIRE_CHANCE_BASE + FIRE_CHANCE_BUFF))
				{
					game->spawnEnemyBullet(getPosition(),disp*(BULLET_SPEED_BASE + BULLET_SPEED_BUFF));
					cooldown = (FIRE_RATE_BASE + FIRE_RATE_BUFF);
				}
				else
					cooldown = rand()%3+1;
			}
		}
		else
		{
			if(Length(&disp) < (ACTIVATION_RANGE_BASE + ACTIVATION_RANGE_BUFF))
			{
				playerSeen = true;
				velocity = disp;
				Normalize(&velocity,&velocity);
				velocity*=(SPEED_BASE + SPEED_BUFF);
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