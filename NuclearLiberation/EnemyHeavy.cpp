#include "EnemyHeavy.h"
#include "NuclearLiberation.h"
using namespace EnemyHeavyNS;
using namespace EnemyNS;

void EnemyHeavy::update(float dt)
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

			velocity*=(SPEED_BASE + SPEED_BUFF);

			cooldown = max(cooldown-dt,0);
			if(cooldown == 0)
			{
				if(rand()%100 < (FIRE_CHANCE_BASE + FIRE_CHANCE_BUFF))
				{
					BulletSpiralTheta += PI/10;
					if(BulletSpiralTheta > 2*PI)
						BulletSpiralTheta = BulletSpiralTheta - 2*PI;

					float x,y;
					D3DXVECTOR3 temp;
					for (float f = 0;f<2*PI;f+=PI){
						x = cos(BulletSpiralTheta+f);
						y = sin(BulletSpiralTheta+f);
						temp = D3DXVECTOR3(x,y,0.0f);
						game->spawnEnemyBullet(getPosition()+5*temp,temp*(BULLET_SPEED_BASE + BULLET_SPEED_BUFF) + velocity);
					}

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


