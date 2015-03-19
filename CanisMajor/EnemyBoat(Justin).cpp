#include "EnemyBoat.h"
#include "CanisMajor.h"

using namespace EnemyBoatNS;
void EnemyBoat::update(float dt)
{
	if(isActive)
	{
		Enemy::update(dt);
		cooldown = max(cooldown-dt,0);
		Vector3 diffToPlayer = game->player.getPosition()-getPosition();
		if(cooldown==0 && Length(&diffToPlayer)<ACTIVATION_RANGE)
		{
			if(game->player.getPosition().y > game->worldSize.y-5)
			{
				Normalize(&diffToPlayer,&diffToPlayer);
				game->spawnEnemyBullet(getPosition(),diffToPlayer*BULLET_SPEED);
			}
			game->spawnEnemyBullet(getPosition(),Vector3(0,-BULLET_SPEED,0));
			cooldown=COOLDOWN;
		}
	}
}

void EnemyBoat::Fire(){}

void EnemyBoat::create(Vector3 pos)
{
	Enemy::create(pos);
	if(game->getCeiling(pos.x)!=game->worldSize.y)
	{
		isActive = false;
	}
	else
	{
		pos.y = game->worldSize.y;
	}
	setPosition(pos);
	MAX_HEALTH = BOAT_HEALTH;
	health = MAX_HEALTH;
}