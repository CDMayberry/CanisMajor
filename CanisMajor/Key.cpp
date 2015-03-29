#include "Key.h"
#include "Camera.h"
#include "CanisMajor.h"
void Key::interactWith(Camera* player)
{
	game->setStoryText(2,L"Found " + name);
	player->addKey(this);
	isActive = false;
}