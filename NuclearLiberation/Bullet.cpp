#pragma once

#include"Bullet.h"
#include"NuclearLiberation.h"

void Bullet::update(float dt)
{
	if(isActive)
	{
		Actor::update(dt);

		lifeTime+= dt;
		if(lifeTime > bulletNS::LINESPAN)
			isActive = false;

		if(getPosition().x < 0 || getPosition().x > game->worldSize.x || getPosition().y < 0 || getPosition().y > game->worldSize.y)
 			isActive=false;
	}

}