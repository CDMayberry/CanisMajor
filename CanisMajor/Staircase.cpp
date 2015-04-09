#include "Staircase.h"
#include "CanisMajor.h"
void Staircase::interactWith(Camera* player){
	Interactable::interactWith(player);
	(game->*loadLevel)();
}