#pragma once

#include "Actor.h"
#include "CanisMajor.h"

Actor::Actor()
{
	radius = 0;
	speed = 0;
	isActive = false;
	isVisible = true;
	Identity(&world);
	Identity(&wvp);
	rotation = Vector3(0,0,0);
	MAX_HEALTH = 100;
	health = MAX_HEALTH;
	collisionType = SPHERE;
}

Actor::~Actor()
{
	 geometry = NULL;
}

void Actor::draw(ID3D10EffectMatrixVariable* fx, Matrix& camera, Matrix& projection, ID3D10EffectTechnique* tech)
{
	if (!isActive || !isVisible)
		return;

    wvp = getWorldMatrix()*camera*projection;	
	foo[0]=0;

	fx->SetMatrix((float*)&wvp);
	game->mfxWorldVar->SetMatrix((float*)&world);

	//TEXTURES
	D3DXMatrixIdentity(&texMtx);
	game->mfxTexMtxVar->SetMatrix((float*)&texMtx);
	game->mfxDiffuseMapVar->SetResource(geometry->mDiffuseMapRV);
	game->mfxSpecMapVar->SetResource(geometry->mSpecMapRV);

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
	health = 1;					//THIS MIGHT SCREW SOMETHING UP: CAELAN
	radiusSquared = radius * radius;
	isActive = false;
}

void Actor::create(Vector3 pos) {
	isActive = true;
	isVisible = true;
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
	if(isActive && gameObject->isActive && gameObject->isVisible)
	{
		if((collisionType == SPHERE) && (gameObject->collisionType==SPHERE))
		{
			Vector3 diff = position - gameObject->getPosition();
			float length = D3DXVec3LengthSq(&diff);
			float radii = radiusSquared + gameObject->getRadiusSquare();
			if (length <= radii)
				return true;
		}
		else if(collisionType == SPHERE && gameObject->collisionType==AABBox)
		{
			Vector3 min = gameObject->getGeometry()->getAABBMin(), max=gameObject->getGeometry()->getAABBMax();
			
			transformAABB(min,max);

			//if the point is closet than the radius
			return radius*radius >= SquaredDistPointAABB(getPosition(),min,max);
		}
		else if(collisionType == AABBox && gameObject->collisionType==SPHERE)
		{
			Vector3 min = getGeometry()->getAABBMin(), max=getGeometry()->getAABBMax();
			
			transformAABB(min,max);

			//if the point is closet than the radius
			return gameObject->radius*gameObject->radius >= SquaredDistPointAABB(gameObject->getPosition(),min,max);
		}

		else if(collisionType == AABBox && gameObject->collisionType==AABBox)
		{
			Vector3 min1 = getGeometry()->getAABBMin(), max1=getGeometry()->getAABBMax();
			Vector3 min2 = gameObject->getGeometry()->getAABBMin(), max2=gameObject->getGeometry()->getAABBMax();


			transformAABB(min1,max1);
			gameObject->transformAABB(min2,max2);

			if(min1.x > max2.x)
				return false;
			if(min1.y > max2.y)
				return false;
			if(min1.z > max2.z)
				return false;
			if(min2.x > max1.x)
				return false;
			if(min2.y > max1.y)
				return false;
			if(min2.z > max1.z)
				return false;
			return true;
		}

		else
		{
			throw "Justin hasn't bothered to build this yet.";
		}
	}
	return false;
}

//http://studiofreya.com/3d-math-and-physics/sphere-vs-aabb-collision-detection-test/
float Actor::SquaredDistPointAABB(Vector3 p, Vector3 min, Vector3 max)
{
    auto check = [&](const double pn,const double bmin,const double bmax ) -> double
		{
			double out = 0;
			double v = pn;
 
			if ( v < bmin )
			{
				double val = (bmin - v);
				out += val * val;
			}
			if ( v > bmax )
			{
				double val = (v - bmax);
				out += val * val;
			}
 
			return out;
		};
 
    // Squared distance
    double sq = 0.0;
 
    sq += check( p.x, min.x, max.x );
    sq += check( p.y, min.y, max.y );
    sq += check( p.z, min.z, max.z );
 
    return sq;
}


void Actor::onDeath() {
}

//all params inout
void Actor::transformAABB(Vector3 & low, Vector3 & high)
{
	low.x *= getScale().x;
	low.y *= getScale().y;
	low.z *= getScale().z;

	high.x *= getScale().x;
	high.y *= getScale().y;
	high.z *= getScale().z;

	low=rotate(low,getRotation());
	high=rotate(high,getRotation());

	low += getPosition();
	high += getPosition();

	Vector3 i,j;
	i.x = min(low.x,high.x);
	i.y = min(low.y,high.y);
	i.z = min(low.z,high.z);

	j.x = max(low.x,high.x);
	j.y = max(low.y,high.y);
	j.z = max(low.z,high.z);

	low = i;
	high = j;
}