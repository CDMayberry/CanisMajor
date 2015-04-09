#include "Door.h"
#include "Camera.h"
#include "CanisMajor.h"

using namespace doorNS;

void Door::interactWith(Camera* player)
{
	//int c = rand()%3;
	//string s = "creak" + std::to_string(c);
	if(!isOpen)
	{
		//door is locked
		if(key!=nullptr)
		{
			if(player->checkItem(key))
			{
				player->removeItem(key);
				isOpen = true;
				Interactable::interactWith(player);//set save to true if needed
				key = nullptr;//unlock
				game->playSound(UNLOCK,getPosition());
				game->setStoryText(2,L"The door unlocks.");
			}
			else
			{
				game->setStoryText(2,L"The door is locked. Find a " + key->name);
			}
		}
		else//door unlocked
		{
			game->playSound(CREAK1,getPosition());
			isOpen=true;
		}
	}
	else
	{
		game->playSound(CLOSE,getPosition());
		isOpen=false;
	}
}
void Door::create(Vector3 pos, Vector3 rot, Vector3 scale, QuestItem* k, bool open)
{
	Actor::create(pos);
	setRotation(rot);
	isOpen = open;
	setScale(scale);
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