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
		arrowPlaced=SRPlaced=MRPlaced=LRPlaced = false;
		atticStairsUsed = false;
		atticKeyTaken = false;
		gameStarted=false;
		tookFlashlight = false;
	}

	Level level;
	bool atticDoorUnlocked;
	bool atticKeyTaken;
	bool balconyDoorUnlocked;
	bool arrowPlaced, SRPlaced, MRPlaced, LRPlaced;
	bool atticStairsUsed;
	bool gameStarted;
	bool tookFlashlight;
};