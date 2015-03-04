#include "EnemyLight.h"
#include "NuclearLiberation.h"
using namespace EnemyLightNS;
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


