#include "EnemyLight.h"
#include "NuclearLiberation.h"
using namespace EnemyLightNS;
using namespace EnemyNS;

void EnemyLight::update(float dt)
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


