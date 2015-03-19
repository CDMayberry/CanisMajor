#pragma once

#include"Bullet.h"
#include"CanisMajor.h"

void Bullet::update(float dt)
{
	if(isActive)
	{
		Actor::update(dt);

		lifeTime+= dt;
		if(lifeTime > bulletNS::LINESPAN)
			isActive = false;

		if(getPosition().x < 0 || getPosition().x > game->worldSize.x || getPosition().y < game->getFloor(getPosition().x)|| getPosition().y > game->getCeiling(getPosition().x))
 			isActive=false;
	}

}