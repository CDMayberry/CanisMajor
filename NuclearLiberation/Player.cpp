#include "Player.h"

#include"NuclearLiberation.h"

using namespace playerNS;

void Player::init(NuclearLiberation*game,Geometry *b, float r, Controls c)
{
	Actor::init(game,b,r);
	controls = c;
	isActive = true;
	setScale(Vector3(2,1,1));
}

void Player::update(float dt)
{
	if(isActive)
	{
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
		input*=DEFAULT_SPEED;
		setVelocity(input);

		position += velocity*dt;

		float x = max(min(position.x,game->worldSize.x),0);
		float y = max(min(position.y,game->worldSize.y),0);

		setPosition(Vector3(x,y,0));

		Identity(&world);
		Vector3 scale = getScale();
		D3DXMatrixScaling(&world,scale.x,scale.y,scale.z);
		Translate(&world, position.x, position.y, position.z);

	}
}