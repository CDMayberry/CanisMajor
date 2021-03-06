#include "QuestItem.h"
#include "Camera.h"
#include "CanisMajor.h"
void QuestItem::interactWith(Camera* player)
{
	Interactable::interactWith(player);
	if(ableToBeTaken)
	{
		game->playSound(PICKUP, position);
		game->setStoryText(2,L"Found " + name);
		player->addItem(this);
		isActive = false;
	}
	else
		game->setStoryText(2,L"The " + name + L" is stuck");
}