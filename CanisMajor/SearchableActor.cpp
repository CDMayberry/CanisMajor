#include "SearchableActor.h"
#include "CanisMajor.h"

void SearchableActor::interactWith(Camera* player)
{
	if(concealedItem != nullptr)
	{
		Vector3 pos= concealedItem->getPosition();
		Vector3 rot= concealedItem->getRotation();
		concealedItem->create(pos,rot);
		concealedItem = nullptr;
		game->setStoryText(2,L"You found something in the " + name);
	}
	else
	{
		game->setStoryText(2,L"The " + name + L" is empty.");
	}
}
void SearchableActor::create(Vector3 pos, Vector3 rot, Vector3 scale, Actor* item)
{
	Actor::create(pos);
	setRotation(rot);
	setScale(scale);
	concealedItem = item;
	if(item!=nullptr)
		item->isVisible = false;
}
