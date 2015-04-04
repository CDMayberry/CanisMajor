#include "Staircase.h"
#include "CanisMajor.h"
void Staircase::interactWith(Camera* player){
	(game->*loadLevel)();
}