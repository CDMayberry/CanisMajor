#include "SlidingBookcase.h"

using namespace slidingBookcaseNS;

void SlidingBookcase::update(float dt){
if(isActive)
	{
		Vector3 rot = getRotation();
		if(getOpen())
		{
			if(D3DXVec3LengthSq(&(getPosition()-openPos)) > EPS){

				Vector3 disp = openPos - getPosition();
				Normalize(&disp,&disp);
				setVelocity(disp);

			}
			else
			{
				setPosition(openPos);setVelocity(Vector3(0,0,0));
			}

		}
		else
		{
			if(D3DXVec3LengthSq(&(getPosition()-closedPos)) > EPS){
				Vector3 disp = closedPos - getPosition();
				Normalize(&disp,&disp);
				setVelocity(disp);
			}
			else
			{
				setPosition(closedPos);setVelocity(Vector3(0,0,0));
			}
		}
		setRotation(rot);
		Actor::update(dt);
	}
}

void SlidingBookcase::create(Vector3 pos, Vector3 rot, Vector3 scale, QuestItem* k, bool open)
{
	Door::create(pos,rot,scale,k,open);
	if(open)
	{
		openPos = pos;
		closedPos = pos-SLIDING_DISP;
	}
	else
	{
		closedPos = pos;
		openPos = pos+SLIDING_DISP;
	}
}