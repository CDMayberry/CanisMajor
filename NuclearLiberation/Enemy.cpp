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
			velocity*=(SPEED_BASE);

			cooldown = max(cooldown-dt,0);
			if(cooldown == 0)
			{
				if(rand()%100 < (FIRE_CHANCE_BASE))
				{
					game->spawnEnemyBullet(getPosition(),disp*(BULLET_SPEED_BASE));
					cooldown = (FIRE_RATE_BASE);
				}
				else
					cooldown = rand()%3+1;
			}
		}
		else
		{
			if(Length(&disp) < (ACTIVATION_RANGE_BASE))
			{
				playerSeen = true;
				velocity = disp;
				Normalize(&velocity,&velocity);
				velocity*=(SPEED_BASE);
			}
		}
		
		Actor::update(dt);
	}


}