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
		reset();
	}

	void reset()
	{
		level = MENU;
		bestLevel = MENU;
		/*atticDoorUnlocked = false;
		balconyDoorUnlocked = false;
		arrowPlaced=SRPlaced=MRPlaced=LRPlaced=pedestalInteractedWith = false;
		atticStairsUsed = false;
		atticKeyTaken=balconyKeyTaken=secondFloorMainKeyTaken = false;
		gameStarted=false;
		tookFlashlight = false;
		secondFloorSairsUsed=false;
		secondFloorMainStairDoorUnlocked=false;
		firstFloorSairsUsed = false;*/
	}

	Level level;
	Level bestLevel;
	/*bool atticDoorUnlocked;
	bool atticKeyTaken, balconyKeyTaken, secondFloorMainKeyTaken;
	bool balconyDoorUnlocked;
	bool secondFloorMainStairDoorUnlocked;
	bool arrowPlaced, SRPlaced, MRPlaced, LRPlaced, pedestalInteractedWith;
	bool atticStairsUsed;
	bool gameStarted;
	bool tookFlashlight;
	bool secondFloorSairsUsed;
	bool firstFloorSairsUsed;*/
	//NOT GONNA HAPPEN
};