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
		
		Vector3 rot = getRotation();

		input*=0;
		if(GetAsyncKeyState(controls.up))
		{
			rot.z+= ROTATION_SPEED*dt;
			input.y=1;
		}
		if(GetAsyncKeyState(controls.down))
		{
			rot.z-= ROTATION_SPEED*dt;
			input.y=-1;
		}
		if(GetAsyncKeyState(controls.left))
		{
			input.x=-1;
		}
		if(GetAsyncKeyState(controls.right))
		{
			input.x=1;
		}

		if(rot.z > MAX_ROTATION_ANGLE)
			rot.z = MAX_ROTATION_ANGLE;
		if(rot.z < -MAX_ROTATION_ANGLE)
			rot.z = -MAX_ROTATION_ANGLE;

		if(rot.z > 0)
			rot.z = max(rot.z-ROTATION_SPEED/2*dt,0);
		if(rot.z < 0)
			rot.z = max(rot.z+ROTATION_SPEED/2*dt,0);

		setRotation(rot);
		Matrix rotTemp;
		Identity(&rotTemp);
		D3DXMatrixRotationZ(&rotTemp,rot.z);


		if(weaponCooldown==0 && GetAsyncKeyState(controls.fire))
		{
 			game->spawnBullet(getPosition(),rotateZ(FIRE_SPEED,rot.z));
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