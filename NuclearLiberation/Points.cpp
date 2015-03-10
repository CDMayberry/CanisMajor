#pragma once

#include"Points.h"
#include"NuclearLiberation.h"

void Points::update(float dt)
{
	if(isActive)
	{
		Drop::update(dt);
	}

}

void Points::onDeath() {
	game->audio->playCue(COIN);
	pickUp();
}

void Points::pickUp() {
	//game->player.grantWeaponLevel();
}