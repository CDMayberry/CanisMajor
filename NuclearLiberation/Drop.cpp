#pragma once

#include"Drop.h"
#include"NuclearLiberation.h"

void Drop::update(float dt)
{
	if(isActive)
	{
		Actor::update(dt);
		despawner -= dt;
		if(despawner <= 0)
			isActive = false;

		if(getPosition().x < 0 || getPosition().x > game->worldSize.x || getPosition().y < 0 || getPosition().y > game->worldSize.y)
 			isActive=false;
	}

}

void Drop::onDeath() {
	game->audio->playCue(COIN);
}

void Drop::pickUp() {
	//game->
}

void Drop::create(Vector3 pos) {
	despawner = dropNS::despawnTime;
	Actor::create(pos);
}