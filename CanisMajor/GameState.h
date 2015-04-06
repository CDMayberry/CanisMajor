#pragma once

enum Level{
	MENU,
	ATTIC,
	SECOND_FLOOR,
	FIRST_FLOOR,
	BASEMENT,
	SPLASH
};

struct GameState
{
	GameState(){
		level = MENU;
		atticDoorUnlocked = false;
		balconyDoorUnlocked = false;
	}

	Level level;
	bool atticDoorUnlocked;
	bool balconyDoorUnlocked;

};