#include "EnemyHeavy.h"
#include "NuclearLiberation.h"
using namespace EnemyHeavyNS;
using namespace EnemyNS;

void EnemyHeavy::Fire()
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


