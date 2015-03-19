#include "Player.h"

#include"CanisMajor.h"

using namespace playerNS;

void Player::init(CanisMajor*game,Geometry *hull, Geometry *point, Geometry *shield, float r, Controls c)
{
	Actor::init(game,hull,r);
	hitBoxIndicatior.init(game,point,r);
	hitBoxIndicatior.isActive = true;
	hitBoxIndicatior.setScale(Vector3(r,r,1));
	shieldIndicator.init(game,shield,SHIELD_SIZE);
	shieldIndicator.setScale(Vector3(SHIELD_SIZE,SHIELD_SIZE,1));
	shieldIndicator.isActive = true;
	controls = c;
	isActive = true;
	accel = Vector3(0,0,0);
	airLevel = MAX_AIR;
	weaponLevel = 1;
	fireCounter = 0;
	drowning = false;
	shieldActive=false;
	airAudio = false;
	lives = 3;
}

void Player::draw(ID3D10EffectMatrixVariable* fx, Matrix& camera, Matrix& projection, ID3D10EffectTechnique* mTech)
{
	if(isActive){
		Actor::draw(fx,camera,projection,mTech);
		hitBoxIndicatior.draw(fx,camera,projection,mTech);
		if(shieldActive)shieldIndicator.draw(fx,camera,projection,mTech);
	}
}

void Player::update(float dt)
{
	if(isActive)
	{
		airLevel = max(airLevel-dt,0);
		weaponCooldown=max(weaponCooldown-dt,0);

		Vector3 pos = getPosition();
		pos.y=min(pos.y,game->getCeiling(pos.x));
		pos.y=max(pos.y,game->getFloor(pos.x));
		pos.x=min(pos.x,game->worldSize.x);
		pos.x=max(pos.x,game->invisibleWallLocation);
		setPosition(pos);
		pos.z-=getScale().z+0.01;
		hitBoxIndicatior.setPosition(pos);
		pos.z+=getScale().z+0.01;
		shieldIndicator.setPosition(pos);
		if(getPosition().y == game->worldSize.y) {
			refillAir();
			airAudio = true;
		}
		else
			airAudio = false;

		if(airLevel < 14 && drowning == false) {
			game->audio->playCue(DROWN);
			drowning = true;
		}

		if(airLevel == 0)
		{
			onDeath();
			game->onPlayerDeath();
			return;
		}

		input*=0;

		bool inputThisFrame = false;
		if(GetAsyncKeyState(controls.up))
		{
			inputThisFrame = true;
			input.y+=1;
		}
		if(GetAsyncKeyState(controls.down))
		{
			inputThisFrame = true;
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

		

		Normalize(&input,&input);	
		
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


		Vector3 rot = getRotation();

		float goal = MAX_ROTATION_ANGLE*input.y;
		if(abs(goal - rot.z) < ROTATION_EPS)
			rot.z = goal;
		else if(goal > rot.z) 
			rot.z = min(rot.z+ROTATION_SPEED*dt,MAX_ROTATION_ANGLE);
		else if(goal < rot.z)
			rot.z = max(rot.z-ROTATION_SPEED*dt,-MAX_ROTATION_ANGLE);

		setRotation(rot);

		if(weaponCooldown==0 && GetAsyncKeyState(controls.fire))
		{
			fireBullets();
		}

		Actor::update(dt);
		hitBoxIndicatior.update(dt);
		shieldIndicator.update(dt);
		
	}
}


void Player::refillAir() {
	airLevel = playerNS::MAX_AIR; 
	if(!airAudio)
		game->audio->playCue(BUB);
	game->audio->stopCue(DROWN); 
	drowning = false;
}


void Player::fireBullets()
{
	Vector3 rot = getRotation();
	game->audio->playCue(FIRING);
	switch(weaponLevel)
	{
	case 1:
		game->spawnBullet(getPosition()+Vector3(0,1.5,0),rotateZ(FIRE_SPEED,rot.z));
		game->spawnBullet(getPosition()+Vector3(0,-1.5,0),rotateZ(FIRE_SPEED,rot.z));
		weaponCooldown = DEFAULT_COOLDOWN;
		break;	
	case 2:
		fireAngle = (fireCounter-3)*PI/6;
		game->spawnBullet(getPosition()+rotateZ(HELIX_DISP,fireAngle),rotateZ(FIRE_SPEED,rot.z));
		game->spawnBullet(getPosition()+rotateZ(HELIX_DISP,-fireAngle),rotateZ(FIRE_SPEED,rot.z));
		fireCounter++;
		if(fireCounter>6)fireCounter = 0;
		weaponCooldown = DEFAULT_COOLDOWN/2;
		break;
	case 3:
		fireAngle = (fireCounter-3)*PI/6;
		game->spawnBullet(getPosition()+rotateZ(HELIX_DISP,fireAngle),rotateZ(FIRE_SPEED,rot.z));
		game->spawnBullet(getPosition()+rotateZ(HELIX_DISP,-fireAngle),rotateZ(FIRE_SPEED,rot.z));
		if(fireCounter==0)
			game->spawnBullet(getPosition(),rotateZ(FIRE_SPEED,rot.z),1.5);
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
		if(fireCounter%2==0)
			game->spawnBullet(getPosition(),rotateZ(FIRE_SPEED,rot.z+PI*0.3));
		else
			game->spawnBullet(getPosition(),rotateZ(FIRE_SPEED,rot.z-PI*0.3));
		fireCounter++;
		if(fireCounter>6)fireCounter = 0;
		weaponCooldown = DEFAULT_COOLDOWN/2;
		break;
	case 5:
		fireAngle = abs(fireCounter-6)*PI/12-PI/2;
		game->spawnBullet(getPosition()+Vector3(0,5,0)+rotateZ(2*HELIX_DISP,fireAngle),rotateZ(FIRE_SPEED,rot.z+0.1));
		game->spawnBullet(getPosition()-Vector3(0,5,0)+rotateZ(2*HELIX_DISP,-fireAngle),rotateZ(FIRE_SPEED,rot.z-0.1));
		fireAngle = (fireCounter%6-3)*PI/6;
		game->spawnBullet(getPosition()+rotateZ(HELIX_DISP,fireAngle),rotateZ(FIRE_SPEED,rot.z));
		game->spawnBullet(getPosition()+rotateZ(HELIX_DISP,-fireAngle),rotateZ(FIRE_SPEED,rot.z));
		if(fireCounter%6==0)
			game->spawnBullet(getPosition(),rotateZ(FIRE_SPEED,rot.z),1.5);

		if(fireCounter%2==0)
			game->spawnBullet(getPosition(),rotateZ(FIRE_SPEED,rot.z+PI*0.3));
		else
			game->spawnBullet(getPosition(),rotateZ(FIRE_SPEED,rot.z-PI*0.3));
		fireCounter++;
		if(fireCounter>=12)fireCounter = 0;
		weaponCooldown = DEFAULT_COOLDOWN/2;
		break;
	}
}

void Player::takeDamage(){
	if(shieldActive) {
		shieldActive=false;
		game->audio->playCue(BLOCK);
	}
	else
		game->onPlayerDeath();
}