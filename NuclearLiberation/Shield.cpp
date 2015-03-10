#pragma once

#include"Shield.h"
#include"NuclearLiberation.h"

void Shield::update(float dt)
{
	if(isActive)
	{
		Drop::update(dt);
	}

}

void Shield::onDeath() {
	game->audio->playCue(SHIELD);
	pickUp();
}

void Shield::pickUp() {
	game->player.grantShield();
}