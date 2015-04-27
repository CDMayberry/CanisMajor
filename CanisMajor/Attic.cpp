#include "CanisMajor.h"

const wstring helpUs =  L"We couldn't get out...\nSomeone help us";

void CanisMajor::loadAttic()
{
	state.level = ATTIC;
	clearLevel();
	int iter = 0;
	if(!state.gameStarted)
	{
		setStoryText(10,L"Where am I?");
		audio->playCue(BG);
		state.gameStarted = true;
		camera.setPosition(Vector3(5,0,5));
		camera.setDirection(Vector3(1,0,0));
	}
	else
	{
		camera.setPosition(Vector3(38,0,27));
		camera.setDirection(Vector3(-1,0,0));
	}

	QuestItem* k = nullptr;
	if(!state.atticKeyTaken)
	{
		k=spawnQuestItem(&mKey,L"GOLD KEY",Vector3(37.3,2,53),Vector3(0,0,0),Vector3(4,4,4));
		k->setStateSwitch(&state,&GameState::atticKeyTaken);
	}

	if(!state.tookFlashlight)
	{
		flashlight.setPosition(Vector3(18,-2,15));
		flashlight.setDirection(Vector3(1,0,-.5));
		flashlight.isActive = true;
		flashlight.setStateSwitch(&state,&GameState::tookFlashlight);
	}

	spawnScenery(&mCube,Vector3(0,-4,0),Vector3(0,0,0),Vector3(40,1,60));

	//Left wall
	for(int i = 0; i < 6; i++)
	{
		spawnScenery(&mWallpanel,Vector3(0,0,i*10),Vector3(0,0,0),CM::WALL_SCALE);
	}

	iter = 0;

	//far end lower
	for(int i = 6; i < 10; i++)
	{
		spawnScenery(&mWallpanel,Vector3(5+iter*10,0,55),Vector3(0,1.5707963268,0),CM::WALL_SCALE);
		iter++;
	}

	iter = 0;
	//Right wall
	for(int i = 10; i< 16; i++)
	{
		if(i !=13)
			spawnScenery(&mWallpanel,Vector3(40,0,iter*10),Vector3(0,0,0),CM::WALL_SCALE);
		iter++;
	}

	iter = 0;
	//close end lower
	for(int i = 17; i < 21; i++)
	{
		spawnScenery(&mWallpanel,Vector3(5+iter*10,0,-5),Vector3(0,1.5707963268,0),CM::WALL_SCALE);
		iter++;
	}

	iter = 0;
	//left lower roof
	for(int i = 21; i < 27; i++)
	{
		spawnScenery(&mWallpanel,Vector3(4.5,10, iter*10),Vector3(0,0,2),Vector3(1,1.1,1));
		iter++;
	}

	iter = 0;
	//left upper roof. Missing one panel for hole
	for(int i = 27; i < 33; i++)
	{
		if(i != 28)//WTF
			spawnScenery(&mWallpanel,Vector3(14.5,14.6, iter*10),Vector3(0,0,2),Vector3(1,1.5,1));
		iter++;
	}

	iter = 0;
	//right side of roof
	for(int i = 33; i < 39; i++)
	{
		spawnScenery(&mWallpanel,Vector3(30,13, iter*10),Vector3(0,0,1.1),Vector3(1,2.2,1));
		iter++;
	}

	//Spawning room objects
	spawnScenery(&mWallpanel,Vector3(10,5,-5.01),Vector3(0,1.5707963268,0),Vector3(1,4,6));

	spawnScenery(&mWallpanel,Vector3(10,5,55.01),Vector3(0,1.5707963268,0),Vector3(1,4,6));

	spawnScenery(&mRoofHole,Vector3(17,10.7,7.1),Vector3(0,0,.42),Vector3(6.6, 4.6, 5.2));

	spawnScenery(&mWallpanel,Vector3(40,5,32),Vector3(0,0,0),Vector3(1,1.6,.6));


	//Bookcases
	spawnSearchable(&mBookcase,L"Bookcase",k,Vector3(37.3,2.3,53.7),Vector3(0,0,0), CM::BOOKCASE_SCALE);
	spawnSearchable(&mBookcase,L"Bookcase",nullptr,Vector3(31,2.3,53.7),Vector3(0,0,0), CM::BOOKCASE_SCALE);

	//Table and chairs
	spawnSearchable(&mTable,L"Table",nullptr,Vector3(10.5,-2.5,35),Vector3(0,PI,0),Vector3(1.2,1.4,1));
	spawnScenery(&mChair,Vector3(8,-3,34),Vector3(0,0,0), CM::CHAIR_SCALE);
	spawnScenery(&mChair,Vector3(11,-1.4,39),Vector3(PI/2,.9,0), CM::CHAIR_SCALE);
	spawnScenery(&mChair,Vector3(13,2.3,34),Vector3(PI,PI,0), CM::CHAIR_SCALE);

	//Bottle on table
	spawnScenery(&mBottle,Vector3(13,.8,36));
	spawnReadable(&mBook, L"Note", nullptr, Vector3(10,.3,36),Vector3(0,0,PI),Vector3(.5,.5,.5),helpUs);

	//Boxes
	spawnSearchable(&mBox,L"Cardboard Box",nullptr,Vector3(37.5,-2,20),Vector3(0,0,0), CM::BOX_SCALE);
	spawnScenery(&mCube,Vector3(37.5,-2,15.7),Vector3(0,0,0), CM::BOX_SCALE);
	spawnScenery(&mCube,Vector3(37.5,-2,11.5),Vector3(0,0,0), CM::BOX_SCALE);
	spawnSearchable(&mBox,L"Cardboard Box",nullptr,Vector3(37.5, 2.1,13.5),Vector3(0,.5,0), CM::BOX_SCALE);

	//Ligthing fixture. We should put a point light here
	spawnScenery(&mFixture,Vector3(20,3, 54),Vector3(0,PI,0));

	//Stairwell
	//Note, you cannot progress forward because of the axis-aligned bounding box
	//Comment out the last wall panel to move forward.
	spawnStaircase(L"downstairs",&CanisMajor::loadSecondFloor,Vector3(40,-4, 27), Vector3(0,0,0), Vector3(1,1,.6));
	spawnScenery(&mWallpanel,Vector3(45,0,30),Vector3(0,1.5707963268,0),Vector3(1,3,1));
	spawnScenery(&mWallpanel,Vector3(45,0,24.7),Vector3(0,1.5707963268,0),Vector3(1,3,1));
	spawnScenery(&mWallpanel,Vector3(47,5,26),Vector3(0,0,.8),Vector3(1,2,3));
	spawnScenery(&mWallpanel,Vector3(51,-3,26),Vector3(0,0,0),Vector3(1,4,1));

	//Comedic effect cubes
	spawnSearchable(&mBox,L"Conspicuous Cube",nullptr,Vector3(10,-2,15),Vector3(0,0,0),CM::BOX_SCALE);
	spawnSearchable(&mBox,L"Inconspicuous Cube",nullptr,Vector3(22,-2,6),Vector3(0,PI/2,0),CM::BOX_SCALE);


	Door *d = spawnDoor(Vector3(39.9,-3.5,29),Vector3(0,0,0),Vector3(1.4,3.5,2.1),k,state.atticDoorUnlocked);
	d->setStateSwitch(&state,&GameState::atticDoorUnlocked);

	//negaLight.pos = Vector3(20, 0, 30);
	pLight.pos = Vector3(20, -212, 20);
	spawnLight(Vector3(20,3.5, 53.5));
	spawnLight(Vector3(10.5, 20, 9.8),Vector3(.8, -1, 0), 4);
}