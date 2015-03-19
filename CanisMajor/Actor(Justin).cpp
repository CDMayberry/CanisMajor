#pragma once

#include "Actor.h"
#include "CanisMajor.h"

Actor::Actor()
{
	radius = 0;
	speed = 0;
	isActive = false;
	Identity(&world);
	Identity(&wvp);
	rotation = Vector3(0,0,0);
	MAX_HEALTH = 100;
	health = MAX_HEALTH;
}

Actor::~Actor()
{
	 geometry = NULL;
}

void Actor::draw(ID3D10EffectMatrixVariable* fx, Matrix& camera, Matrix& projection, ID3D10EffectTechnique* tech)
{
	if (!isActive)
		return;

    wvp = getWorldMatrix()*camera*projection;	
	foo[0]=0;
	fx->SetRawValue(&foo, 0, sizeof(int)); //MIGHT NEED TO ADD STUFF FOR SHADER VARIABLES
	fx->SetMatrix((float*)&wvp);
	setMTech(tech);

	D3D10_TECHNIQUE_DESC techDesc;
    mTech->GetDesc( &techDesc );

    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        mTech->GetPassByIndex( p )->Apply(0);
        geometry->draw();
    }
}

void Actor::init(CanisMajor* game,Geometry *b, float r)
{
	this->game = game;
	geometry = b;
	radius = r;
	position = Vector3(0,0,0);
	velocity = Vector3(0,0,0);
	speed = 0;
	scale = Vector3(1,1,1);
	health = 1;													//THIS MIGHT SCREW SOMETHING UP: CAELAN
	radiusSquared = radius * radius;
	isActive = false;
}

void Actor::create(Vector3 pos) {
	isActive = true;
	health = MAX_HEALTH;
	position = pos;
}


void Actor::update(float dt)
{

	if(isActive)
	{
		if(health <= 0) {
			isActive = false;
 			onDeath();
			return;
		}
		position += velocity*dt;
		Identity(&world);
		Identity(&s);
		Identity(&t);
		Identity(&rx);
		Identity(&ry);
		Identity(&rz);

		D3DXMatrixScaling(&s,scale.x,scale.y,scale.z);

		D3DXMatrixRotationX(&rx,rotation.x);
		D3DXMatrixRotationY(&ry,rotation.y);
		D3DXMatrixRotationZ(&rz,rotation.z);

		Translate(&t, position.x, position.y, position.z);

		world *= s*rx*ry*rz*t;
		
	}
}

bool Actor::collided(Actor *gameObject)
{
	if(isActive && gameObject->isActive)
	{
		Vector3 diff = position - gameObject->getPosition();
		float length = D3DXVec3LengthSq(&diff);
		float radii = radiusSquared + gameObject->getRadiusSquare();
		if (length <= radii)
			return true;
	}
	return false;
}

void Actor::onDeath() {
	//int rander = random(3);
	//switch(rander) {
	//	case 1:
	//		game->audio->playCue(EXP1);
	//		break;
	//	case 2:
	//		game->audio->playCue(EXP2);
	//		break;
	//	case 3:
	//		game->audio->playCue(EXP3);
	//		break;
	//}
}