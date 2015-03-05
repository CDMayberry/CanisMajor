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

		float x = max(min(position.x,game->worldSize.x),game->minPlayerPosition);
		float y = max(min(position.y,game->worldSize.y),5*(sin(2*PI*x/150.0)+2)-7+wallNS::WALL_SCALE);

		setPosition(Vector3(x,y,0));

		weaponCooldown=max(weaponCooldown-dt,0);
		
		Vector3 rot = getRotation();

		input*=0;

		bool inputRotation = false;
		bool inputThisFrame = false;
		if(GetAsyncKeyState(controls.up))
		{
			inputThisFrame = inputRotation = true;
			rot.z+= ROTATION_SPEED*dt;
			input.y+=1;
		}
		if(GetAsyncKeyState(controls.down))
		{
			inputThisFrame = inputRotation = true;
			rot.z-= ROTATION_SPEED*dt;
			input.y+=-1;
		}
		if(GetAsyncKeyState(controls.left))
		{
			inputThisFrame = true;
			input.x+=-1;
		}
		if(GetAsyncKeyState(controls.right))
		{
			inputThisFrame = true;
			input.x+=1;
		}

		if(!inputRotation)
		{
			if(rot.z > 0)
				rot.z = max(rot.z-ROTATION_SPEED*2*dt,0);
			if(rot.z < 0)
				rot.z = min(rot.z+ROTATION_SPEED*2*dt,0);
		}
		else
		{
			if(rot.z > MAX_ROTATION_ANGLE)
				rot.z = MAX_ROTATION_ANGLE;
			if(rot.z < -MAX_ROTATION_ANGLE)
				rot.z = -MAX_ROTATION_ANGLE;
		}

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

		//velocity = input * MAX_SPEED;

		
		if(inputThisFrame)
		{
			speed = min(speed+ACCEL_RATE*dt,MAX_SPEED);
			velocity = input * speed;
		}
		else
		{
			speed = max(speed-FRICTION_RATE*dt,0);
			Normalize(&velocity,&velocity);
			velocity*=speed;
		}

		
			
		

	}
}