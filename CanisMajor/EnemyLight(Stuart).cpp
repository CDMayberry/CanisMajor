#include "EnemyLight.h"
#include "CanisMajor.h"
using namespace EnemyLightNS;
using namespace EnemyNS;

void EnemyLight::Fire()
{
				if(rand()%100 < (FIRE_CHANCE_BASE + FIRE_CHANCE_BUFF))
				{
					for (int i=0;i<360;i+=360/NUM_BULLETS_IN_CIRCLE){
						float x = cos((i/180.0)*PI);
						float y = sin((i/180.0)*PI);
						D3DXVECTOR3 temp(x,y,0.0f);
						game->spawnEnemyBullet(getPosition()+5*temp,temp*(BULLET_SPEED_BASE + BULLET_SPEED_BUFF) + velocity);

					}

					cooldown = (FIRE_RATE_BASE + FIRE_RATE_BUFF);
				}
				else
					cooldown = rand()%3+1;
}


