#include "Player.h"

#include"NuclearLiberation.h"

using namespace playerNS;

void Player::init(NuclearLiberation*game,Geometry *b, float r, Controls c)
{
	Actor::init(game,b,r);
	controls = c;
	isActive = true;
	accel = Vector3(0,0,0);
}

void Player::update(float dt)
{
	if(isActive)
	{
		Actor::update(dt);

		float x = max(min(position.x,game->worldSize.x),0);
		float y = max(min(position.y,game->worldSize.y),0);

		setPosition(Vector3(x,y,0));

		weaponCooldown=max(weaponCooldown-dt,0);

		input*=0;
		if(GetAsyncKeyState(controls.up))
			input.y=1;
		if(GetAsyncKeyState(controls.down))
			input.y=-1;
		if(GetAsyncKeyState(controls.left))
			input.x=-1;
		if(GetAsyncKeyState(controls.right))
			input.x=1;
		if(weaponCooldown==0 && GetAsyncKeyState(controls.fire))
		{
 			game->spawnBullet(getPosition(),Vector3(100,0,0));//TODO, figure out better velocity vector
			weaponCooldown = DEFAULT_COOLDOWN;
		}

		Normalize(&input,&input);
		
		accel += input;
		accel*=0.5;

		if(Length(&accel) < 0.5)
		{
			accel = Vector3(0,0,0);
			velocity*=0.99;
		}
		else
		{
			velocity+=accel;
			if(Length(&velocity) > playerNS::MAX_SPEED)
			{
				Normalize(&velocity,&velocity);
				velocity*=playerNS::MAX_SPEED;
			}
		}
		
		//game->cameraDisplacement.x = -velocity.x/6;
		//game->cameraDisplacement.y = -velocity.y/6;		
		

	}
}