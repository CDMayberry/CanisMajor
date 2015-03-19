#include "EnemySplit.h"
#include "CanisMajor.h"
using namespace EnemySplitNS;
using namespace EnemyNS;

void EnemySplit::Fire(){
	        Vector3 disp = game->player.getPosition()-getPosition();
			D3DXVec3Normalize(&disp,&disp);
				if(rand()%100 < (FIRE_CHANCE_BASE + FIRE_CHANCE_BUFF))
				{
					game->spawnEnemyBullet(getPosition(),getVelocity()+ disp*(BULLET_SPEED_BASE + BULLET_SPEED_BUFF));
					cooldown = (FIRE_RATE_BASE + FIRE_RATE_BUFF);
				}
				else
					cooldown = rand()%3+1;
}


void EnemySplit::setGen(int g){
	generation = g;
	float size = pow(2, 1.0f/g);
	setScale(D3DXVECTOR3(size,size,size));
}