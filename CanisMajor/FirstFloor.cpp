#include "CanisMajor.h"

void CanisMajor::loadFirstFloor()
{
	state.level = FIRST_FLOOR;
	clearLevel();
	flashlight.isActive = true;
	camera.setPosition(Vector3(35,0,55));
	QuestItem *k=nullptr;
	k=spawnQuestItem(&mKey,L"Obscure Key",Vector3(15,2,-5),Vector3(0,0,0),Vector3(4,4,4));
	//floor
	spawnScenery(&mCube,Vector3(60,-4,27.5),Vector3(0,0,0),Vector3(60,1,27.5));
	spawnScenery(&mCube,Vector3(57.5,-4,62.5),Vector3(0,0,0),Vector3(17.5,1,7.5));

	//West Wall
	spawnScenery(&mWallpanel,Vector3(0,0,3),Vector3(0,0,0),Vector3(1,1.6,.55));
	spawnScenery(&mWindowPanel,Vector3(0,2,10),Vector3(0,0,0), CM::WALL_SCALE2);
	spawnScenery(&mWallpanel,Vector3(0,0,27.5),Vector3(0,0,0),Vector3(1,1.6,2.5));
	spawnScenery(&mWindowPanel,Vector3(0,2,45),Vector3(0,0,0), CM::WALL_SCALE2);
	spawnScenery(&mWallpanel,Vector3(0,0,52.5),Vector3(0,0,0),Vector3(1,1.6,.5));
	


	//North wall and servant quarters
	spawnScenery(&mWallpanel,Vector3(16,0,55),Vector3(0,PI/2,0),Vector3(1,1.6,3.2));
	spawnScenery(&mWallpanel,Vector3(42,0,55),Vector3(0,PI/2,0),Vector3(1,1.6,.4));
	spawnScenery(&mWallpanel,Vector3(40,0,62.5),Vector3(0,0,0),Vector3(1,1.6,1.5));
	spawnScenery(&mWallpanel,Vector3(46.5,0,70),Vector3(0,PI/2,0),Vector3(1,1.6,1.25));
	spawnScenery(&mWindowPanel,Vector3(57.5,2,70),Vector3(0,PI/2,0), CM::WALL_SCALE2);
	spawnScenery(&mWallpanel,Vector3(68.5,0,70),Vector3(0,PI/2,0),Vector3(1,1.6,1.25));
	spawnScenery(&mWallpanel,Vector3(75,0,62.5),Vector3(0,0,0),Vector3(1,1.6,1.5));
	spawnScenery(&mWallpanel,Vector3(57.5,0,55),Vector3(0,PI/2,0),Vector3(1,1.6,1.7));
	spawnScenery(&mWallpanel,Vector3(73,0,55),Vector3(0,PI/2,0),Vector3(1,1.6,.4));
	spawnScenery(&mWallpanel,Vector3(82.5,0,55),Vector3(0,PI/2,0),Vector3(1,1.6,1.5));
	spawnScenery(&mWindowPanel,Vector3(95,2,55),Vector3(0,PI/2,0), CM::WALL_SCALE2);
	spawnScenery(&mWallpanel,Vector3(110,0,55),Vector3(0,PI/2,0),Vector3(1,1.6,2));
	spawnDoor(Vector3(44,-4.2,55),Vector3(0,-PI/2,0),Vector3(2,4,2.5),nullptr);
	spawnDoor(Vector3(66,-4.2,55),Vector3(0,-PI/2,0),Vector3(2,4,2.5),nullptr);

	//East Wall
	spawnScenery(&mWallpanel,Vector3(120,0,47.5),Vector3(0,0,0), Vector3(1,1.6,1.5));
	for(int i = 0; i<4; i++)
	{
		spawnScenery(&mWindowPanel,Vector3(120,2,35-(i*10)),Vector3(0,0,0), CM::WALL_SCALE2);
	}

	//South wall
	spawnScenery(&mWallpanel,Vector3(91,0,0),Vector3(0,PI/2,0),Vector3(1,1.6,5.8));
	spawnScenery(&mWindowPanel,Vector3(57,2,0),Vector3(0,PI/2,0),CM::WALL_SCALE2);
	spawnScenery(&mWindowPanel,Vector3(38,2,0),Vector3(0,PI/2,0),CM::WALL_SCALE2);
	spawnScenery(&mWallpanel,Vector3(16.5,0,0),Vector3(0,PI/2,0),Vector3(1,1.6,3.3));
	spawnDoor(Vector3(53,-4.2,0),Vector3(0,PI/2,0),Vector3(2,4,5),k);

	//Parlor walls
	spawnScenery(&mWallpanel,Vector3(30,0,5),Vector3(0,0,0),CM::WALL_SCALE);
	spawnScenery(&mWallpanel,Vector3(30,0,32.5),Vector3(0,0,0),Vector3(1,1.6,2.5));
	spawnScenery(&mWallpanel,Vector3(35,0,45),Vector3(0,PI/2,0),CM::WALL_SCALE);
	spawnScenery(&mWallpanel,Vector3(60,0,45),Vector3(0,PI/2,0),CM::WALL_SCALE);
	spawnScenery(&mWallpanel,Vector3(65,0,32.5),Vector3(0,0,0),Vector3(1,1.6,2.5));
	spawnScenery(&mWallpanel,Vector3(65,0,5),Vector3(0,0,0),CM::WALL_SCALE);

	//Baby room walls
	spawnScenery(&mWallpanel,Vector3(20,0,2.5),Vector3(0,0,0),Vector3(1,1.6,.5));
	spawnScenery(&mWallpanel,Vector3(20,0,12.5),Vector3(0,0,0),Vector3(1,1.6,.5));
	spawnScenery(&mWallpanel,Vector3(16.25,0,15),Vector3(0,PI/2,0),Vector3(1,1.6,.75));
	spawnScenery(&mWallpanel,Vector3(3.75,0,15),Vector3(0,PI/2,0),Vector3(1,1.6,.75));
	spawnDoor(Vector3(20,-4.2,5),Vector3(0,PI,0),Vector3(2,4,2.5),nullptr);

	//East bathroom wall
	spawnScenery(&mWallpanel,Vector3(20,0,28.75),Vector3(0,0,0),Vector3(1,1.6,2.75));
	spawnDoor(Vector3(7.5,-4.2,15),Vector3(0,-PI/2,0),Vector3(2,4,2.5),nullptr);
	spawnDoor(Vector3(12.5,-4.2,35),Vector3(0,PI/2,0),Vector3(2,4,2.5),nullptr);

	//Children room
	spawnScenery(&mWallpanel,Vector3(20,0,51.25),Vector3(0,0,0),Vector3(1,1.6,.75));
	spawnScenery(&mWallpanel,Vector3(16.25,0,35),Vector3(0,PI/2,0),Vector3(1,1.6,.75));
	spawnScenery(&mWallpanel,Vector3(3.75,0,35),Vector3(0,PI/2,0),Vector3(1,1.6,.75));
	spawnDoor(Vector3(20,-4.2,42.5),Vector3(0,PI,0),Vector3(2,4,2.5),nullptr);

	//Kitchen and closet walls
	spawnScenery(&mWallpanel,Vector3(90,0,14),Vector3(0,0,0),Vector3(1,1.6,.25));
	spawnScenery(&mWallpanel,Vector3(90,0,1.25),Vector3(0,0,0),Vector3(1,1.6,.25));
	spawnScenery(&mWallpanel,Vector3(82.5,0,15),Vector3(0,PI/2,0),Vector3(1,1.6,1.5));
	spawnScenery(&mWallpanel,Vector3(75,0,17.5),Vector3(0,0,0),Vector3(1,1.6,.5));
	spawnScenery(&mWallpanel,Vector3(82.5,0,20),Vector3(0,PI/2,0),Vector3(1,1.6,1.5));
	spawnScenery(&mWallpanel,Vector3(90,0,32.5),Vector3(0,0,0),Vector3(1,1.6,2.5));
	spawnDoor(Vector3(90,-4.2,15.1),Vector3(0,PI,0),Vector3(2,4,2.5),nullptr);

	//North interior walls
	spawnScenery(&mWallpanel,Vector3(87.5,0,45),Vector3(0,PI/2,0),Vector3(1,1.6,2));
	spawnScenery(&mWallpanel,Vector3(117.5,0,45),Vector3(0,PI/2,0),Vector3(1,1.6,.5));

	//Ceiling
	spawnScenery(&mWallpanel,Vector3(15,8,25),Vector3(0,0,PI/2),Vector3(1,3,6));
	spawnScenery(&mWallpanel,Vector3(70,8,50),Vector3(0,0,PI/2),Vector3(1,9.8,1));
	spawnScenery(&mWallpanel,Vector3(57.5,8,62.5),Vector3(0,0,PI/2),Vector3(1,3.5,1.5));
	spawnScenery(&mWallpanel,Vector3(77.5,8,23),Vector3(0,0,PI/2),Vector3(1,2.5,5));

	//Parlor Ceiling
	spawnScenery(&mWallpanel,Vector3(57,14.6,22.5),Vector3(0,0,1),Vector3(1,2.4,4.5));
	spawnScenery(&mWallpanel,Vector3(37,14.6,22.5),Vector3(0,0,-1),Vector3(1,2.4,4.5));
	spawnScenery(&mWallpanel,Vector3(47,17,0),Vector3(0,PI/2,0),Vector3(1,2,3.9));
	spawnScenery(&mWallpanel,Vector3(47,17,45),Vector3(0,PI/2,0),Vector3(1,2,3.9));

	//Dining Hall Ceiling
	spawnScenery(&mWallpanel,Vector3(110,13.8,22.5),Vector3(0,0,1),Vector3(1,2.35,4.5));
	spawnScenery(&mWallpanel,Vector3(100,13.8,22.5),Vector3(0,0,-1),Vector3(1,2.35,4.5));
	spawnScenery(&mWallpanel,Vector3(103,17,0),Vector3(0,PI/2,0),Vector3(1,2.2,4.2));
	spawnScenery(&mWallpanel,Vector3(103,17,45),Vector3(0,PI/2,0),Vector3(1,2.2,4.2));

	//Stairwell

}