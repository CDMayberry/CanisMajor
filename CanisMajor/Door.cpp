#include "Door.h"
#include "Camera.h"
#include "CanisMajor.h"

using namespace doorNS;

void Door::interactWith(Camera* player)
{
	if(!isOpen)
	{
		//door is locked
		if(key!=nullptr)
		{
			if(player->checkKey(key))
			{
				player->removeKey(key);
				isOpen = true;
				key = nullptr;//unlock
				game->setStoryText(2,L"The door unlocks.");
			}
			else
			{
				game->setStoryText(2,L"The door is locked. Find a " + key->name);
			}
		}
		else//door unlocked
		{
			isOpen=true;
		}
	}
	else
	{
		isOpen=false;
	}
}
void Door::create(Vector3 pos, Vector3 rot, Key* k, bool open)
{
	Actor::create(pos);
	setRotation(rot);
	isOpen = open;
	if(isOpen)
	{
		closedRotation = rot.y+PI/2;
		openRotation = rot.y;
	}
	else
	{
		closedRotation = rot.y;
		openRotation = rot.y-PI/2;
	}
	key = k;
}

void Door::update(float dt)
{
	if(isActive)
	{
		Vector3 rot = getRotation();
		if(isOpen)
		{
			rot.y=max(rot.y-SWING_SPEED*dt,openRotation);
		}
		else
		{
			rot.y=min(rot.y+SWING_SPEED*dt,closedRotation);
		}
		setRotation(rot);
		Actor::update(dt);
	}
}