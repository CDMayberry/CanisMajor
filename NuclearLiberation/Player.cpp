#include "Player.h"

#include"NuclearLiberation.h"

using namespace playerNS;

void Player::init(NuclearLiberation*game,Geometry *b, float r, Controls c)
{
	Actor::init(game,b,r);
	controls = c;
	isActive = true;
	accel = Vector3(0,0,0);
	airLevel = MAX_AIR;
	weaponLevel = 1;
	fireCounter = 0;
}

void Player::update(float dt)
{
	bool inWall = false;
	if(isActive)
	{
		oldPos = getPosition();
		Actor::update(dt);

		airLevel = max(airLevel-dt,0);
		if(airLevel == 0)
		{
			game->onPlayerDeath();
			return;
		}

		float x = max(min(position.x,game->worldSize.x),game->minPlayerPosition);
		float y = max(min(position.y,game->worldSize.y),5*(sin(2*PI*x/150.0)+2)-7+wallNS::WALL_SCALE);

		position.x = x;
		position.y = y;

		if(y == game->worldSize.y) 
			refillAir();

		int iter = oldPos.x;

		while(iter < position.x + wallNS::WALL_SCALE)
		{
			if(topDef(iter)-position.y > 47 && topDef(iter)-position.y < 70 && !inGap())
			{
				position = oldPos;
				break;
			}

			iter++;
		}

		inWall = false;

		setPosition(position);

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

		static bool keyDown = false;
		if(GetAsyncKeyState('U'))
		{
			if(!keyDown)
			{
				grantWeaponLevel();
				keyDown = true;
			}
		}
		else
			keyDown = false;

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
			game->audio->playCue(FIRING);
			switch(weaponLevel)
			{
			case 1:
				game->spawnBullet(getPosition(),rotateZ(FIRE_SPEED,rot.z));
				weaponCooldown = DEFAULT_COOLDOWN;
				break;
			case 2:
				game->spawnBullet(getPosition()+Vector3(0,1.5,0),rotateZ(FIRE_SPEED,rot.z));
				game->spawnBullet(getPosition()+Vector3(0,-1.5,0),rotateZ(FIRE_SPEED,rot.z));
				weaponCooldown = DEFAULT_COOLDOWN;
				break;	
			case 3:
				fireAngle = (fireCounter-3)*PI/6;
				game->spawnBullet(getPosition()+rotateZ(HELIX_DISP,fireAngle),rotateZ(FIRE_SPEED,rot.z));
				game->spawnBullet(getPosition()+rotateZ(HELIX_DISP,-fireAngle),rotateZ(FIRE_SPEED,rot.z));
				fireCounter++;
				if(fireCounter>6)fireCounter = 0;
				weaponCooldown = DEFAULT_COOLDOWN/2;
				break;
			case 4:
				fireAngle = (fireCounter-3)*PI/6;
				game->spawnBullet(getPosition()+rotateZ(HELIX_DISP,fireAngle),rotateZ(FIRE_SPEED,rot.z));
				game->spawnBullet(getPosition()+rotateZ(HELIX_DISP,-fireAngle),rotateZ(FIRE_SPEED,rot.z));
				if(fireCounter==0)
					game->spawnBullet(getPosition(),rotateZ(FIRE_SPEED,rot.z),1.5);
				fireCounter++;
				if(fireCounter>6)fireCounter = 0;
				weaponCooldown = DEFAULT_COOLDOWN/2;
				break;
			case 5:
				fireAngle = (fireCounter-3)*PI/6;
				game->spawnBullet(getPosition()+rotateZ(HELIX_DISP,fireAngle),rotateZ(FIRE_SPEED,rot.z));
				game->spawnBullet(getPosition()+rotateZ(HELIX_DISP,-fireAngle),rotateZ(FIRE_SPEED,rot.z));
				if(fireCounter==0)
					game->spawnBullet(getPosition(),rotateZ(FIRE_SPEED,rot.z),1.5);
				if(fireCounter%2==0)
					game->spawnBullet(getPosition(),rotateZ(FIRE_SPEED,rot.z+PI*0.4));
				else
					game->spawnBullet(getPosition(),rotateZ(FIRE_SPEED,rot.z-PI*0.4));
				fireCounter++;
				if(fireCounter>6)fireCounter = 0;
				weaponCooldown = DEFAULT_COOLDOWN/2;
				break;
			case 6:
				fireAngle = abs(fireCounter-6)*PI/12-PI/2;
				game->spawnBullet(getPosition()+Vector3(0,5,0)+rotateZ(2*HELIX_DISP,fireAngle),rotateZ(FIRE_SPEED,rot.z+0.1));
				game->spawnBullet(getPosition()-Vector3(0,5,0)+rotateZ(2*HELIX_DISP,-fireAngle),rotateZ(FIRE_SPEED,rot.z-0.1));
				fireAngle = (fireCounter%6-3)*PI/6;
				game->spawnBullet(getPosition()+rotateZ(HELIX_DISP,fireAngle),rotateZ(FIRE_SPEED,rot.z));
				game->spawnBullet(getPosition()+rotateZ(HELIX_DISP,-fireAngle),rotateZ(FIRE_SPEED,rot.z));
				if(fireCounter%6==0)
					game->spawnBullet(getPosition(),rotateZ(FIRE_SPEED,rot.z),1.5);

				if(fireCounter%2==0)
					game->spawnBullet(getPosition(),rotateZ(FIRE_SPEED,rot.z+PI*0.4));
				else
					game->spawnBullet(getPosition(),rotateZ(FIRE_SPEED,rot.z-PI*0.4));
				fireCounter++;
				if(fireCounter>=12)fireCounter = 0;
				weaponCooldown = DEFAULT_COOLDOWN/2;
				break;
			}
			
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

float Player::topDef(float x)
{
	float y = 5*(sin(2*PI*x/150.0)+2)+90;
	return y;
}

float Player::bottomWall(float x)
{
	float y = 5*(sin(2*PI*x/150.0)+2)+30;
	return y;
}

bool Player::inGap()
{
	if(position.x > 100 && position.x < 150)
		return true;
	else if(position.x > 250 && position.x < 270)
		return true;
	else 
		return false;
}

void Player::onDeath() {
	game->audio->playCue(PEXP);
}