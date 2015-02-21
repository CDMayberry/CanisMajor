
#include "Actor.h"


Actor::Actor()
{
	radius = 0;
	speed = 0;
	isActive = false;
	Identity(&world);
	Identity(&wvp);
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

void Actor::init(NuclearLiberation* game,Geometry *b, float r)
{
	this->game = game;
	geometry = b;
	radius = r;
	position = Vector3(0,0,0);
	velocity = Vector3(0,0,0);
	speed = 0;
	scale = Vector3(1,1,1);
	radiusSquared = radius * radius;
	isActive = false;
}

void Actor::create(Vector3 pos)
{
	isActive = true;
	position = pos;
}


void Actor::update(float dt)
{
	if(isActive)
	{
		position += velocity*dt;
		Identity(&world);
		D3DXMatrixScaling(&world,scale.x,scale.y,scale.z);
		Translate(&world, position.x, position.y, position.z);
		
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