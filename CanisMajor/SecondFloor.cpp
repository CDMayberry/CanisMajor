#include "CanisMajor.h"

const wstring itemList = L"The book contains a list of items labeled \'Artifacts\':\nDiamond\nStaff\nAltar";
const wstring theDog = L"August 19th:\n I'll have to put down that dog soon,\n it can barely walk, that dog has never\n completely recovered from it's trip to Egypt.\n The twins will be sad, they loved to talk to it.";
const wstring arrowLoc =  L"Scrawled in child's writing:\nDolan loves it when we hide daddy's things!\n We even hid one in his dresser.";
const wstring thieves =  L"August 20th:\nBloody hell, where have those children hidden my artifacts???";
const wstring constellation = L"Canis Major can be seen rising just over the horizon";
const wstring failure = L"The lower half of a damaged note:\n-What have I brought back with me?";

void CanisMajor::loadSecondFloor()
{
	state.bestLevel = SECOND_FLOOR;
	state.level = SECOND_FLOOR;
	clearLevel();
	setStoryText(10,L"this is a big house...");

	QuestItem* patKey = nullptr;
	patKey=spawnQuestItem(&mKey,L"BALCONY KEY",Vector3(7,1,15));

	QuestItem* mainKey = nullptr;

	mainKey = spawnQuestItem(&mKey,L"KEY",Vector3(81,1,20));


	QuestItem *r1 = spawnQuestItem(&mRing,L"Large Ring",Vector3(18,0,17),Vector3(0,0,0),Vector3(1.2f,1,1.2f));
	QuestItem *r2 = spawnQuestItem(&mRing,L"Medium Ring",Vector3(25,1.5f,32),Vector3(0,0,0),Vector3(1,1,1));
	QuestItem *r3 = spawnQuestItem(&mRing,L"Small Ring",Vector3(2,2,47),Vector3(0,0,0),Vector3(.8f,1,.8f));
	QuestItem *a = spawnQuestItem(&mArrow,L"Arrow",Vector3(40,1.5f,14),Vector3(0,0,0),Vector3(1,1,1));
	pedestal.create(mainKey,a,r1,r2,r3,Vector3(82,-.5f,20),Vector3(0,0,0),Vector3(1,1,1));
	pedestal.load(&state);

	state.level = SECOND_FLOOR;
	camera.setPosition(Vector3(37,3,65));

	flashlight.setPosition(Vector3(10,-2.5f,10));
	flashlight.isActive = true;

	dog.isActive = true;
	dog.setPosition(Vector3(35,0,2));
	dog.setTargetWPStage(2);
	//Floor panels
	spawnScenery(&mCube,Vector3(0,-4,30),Vector3(0,0,0),Vector3(28,1,30));
	spawnScenery(&mCube,Vector3(53,-4,30),Vector3(0,0,0),Vector3(17,1,30));
	spawnScenery(&mCube,Vector3(32,-4,4),Vector3(0,0,0),Vector3(4,1,5));
	spawnScenery(&mCube,Vector3(32,-4,40),Vector3(0,0,0),Vector3(4,1,20));

	//Roof panels
	spawnScenery(&mCube,Vector3(35,10,30),Vector3(0,0,0),Vector3(35,1,30));

	//Balcony railing and floor
	spawnScenery(&mCube,Vector3(80,-4,30),Vector3(0,PI/2,0),Vector3(15,1,10));
	for(int i= 0; i<10;i++)
		spawnScenery(&mRail,Vector3(71.5f+(i*2),-3,45),Vector3(0,PI/2,0),Vector3(2,2,1.6f));
	for(int i= 0; i<15;i++)
		spawnScenery(&mRail,Vector3(89.5f,-3,43-(i*2)),Vector3(0,PI,0),Vector3(2,2,1.6f));
	for(int i= 0; i<10;i++)
		spawnScenery(&mRail,Vector3(71.5f+(i*2),-3,15),Vector3(0,PI/2,0),Vector3(2,2,1.6f));

	//Telescope. It should contain something for a puzzle
	spawnReadable(&mTelescope,L"Telescope",nullptr,Vector3(85,-2.7f,40),Vector3(0,-2*PI/3,0),Vector3(7,7,7),constellation);

	//Left outer wall
	spawnScenery(&mWallpanel,Vector3(0,3,10),Vector3(0,0,0), Vector3(1,1.2f,2));
	spawnScenery(&mWindowPanel,Vector3(0,3,25),Vector3(PI,0,0),CM::WALL_SCALE2);
	spawnScenery(&mWindowPanel,Vector3(0,3,35),Vector3(PI,0,0),CM::WALL_SCALE2);
	spawnScenery(&mWallpanel,Vector3(0,3,50),Vector3(0,0,0), Vector3(1,1.2f,2));

	//Far wall with door to attic
	spawnScenery(&mWallpanel,Vector3(10,3,60),Vector3(0,1.5707963268f,0), Vector3(1,1.2f,2));
	spawnScenery(&mWindowPanel,Vector3(25,3,60),Vector3(0,1.5707963268f,0), CM::WALL_SCALE2);
	spawnScenery(&mWallpanel,Vector3(32.5f,3,60),Vector3(0,1.5707963268f,0), Vector3(1,1.2f,.5f));
	spawnDoor(Vector3(35,-2.6f,60),Vector3(0,-PI/2,0),Vector3(2,5,2));
	spawnScenery(&mWallpanel,Vector3(41.5f,3,60),Vector3(0,1.5707963268f,0), Vector3(1,1.2f,.5f));
	spawnScenery(&mWindowPanel,Vector3(49,3,60),Vector3(0,1.5707963268f,0), CM::WALL_SCALE2);
	spawnScenery(&mWallpanel,Vector3(62,3,60),Vector3(0,1.5707963268f,0), Vector3(1,1.2f,1.6f));

	//Right outer wall
	spawnScenery(&mWallpanel,Vector3(70,3,45),Vector3(0,0,0), Vector3(1,1.2f,3));
	spawnDoor(Vector3(70,-3.5f,30),Vector3(0,0,0),Vector3(2,4,3),patKey);
	spawnScenery(&mWallpanel,Vector3(70,3,12),Vector3(0,0,0), Vector3(1,1.2f,2.5f));

	//Close outer wall
	spawnScenery(&mWallpanel,Vector3(10,3,0),Vector3(0,1.5707963268f,0), Vector3(1,1.2f,2));
	spawnScenery(&mWindowPanel,Vector3(25,3,0),Vector3(0,1.5707963268f,0), CM::WALL_SCALE2);
	spawnScenery(&mWallpanel,Vector3(62.5f,3,0),Vector3(0,1.5707963268f,0), Vector3(1,1.2f,1.5f));
	spawnScenery(&mWindowPanel,Vector3(50,3,0),Vector3(0,1.5707963268f,0), CM::WALL_SCALE2);
	spawnScenery(&mWallpanel,Vector3(37.5f,3,0),Vector3(0,1.5707963268f,0), Vector3(1,1.2f,1.5f));

	//Library walls
	spawnScenery(&mWallpanel,Vector3(5,3,20),Vector3(0,1.5707963268f,0), CM::WALL_SCALE2);
	spawnDoor(Vector3(13.9f,-3,20),Vector3(0,-3*PI/2,0),Vector3(2,5,2));
	spawnScenery(&mWallpanel,Vector3(17,3,20),Vector3(0,1.5707963268f,0), Vector3(1,1.2f,.65f));
	spawnScenery(&mWallpanel,Vector3(20,3,2.5f),Vector3(0,0,0), Vector3(1,1.2f,.5f));
	spawnDoor(Vector3(20,-3,5),Vector3(0,PI,0),Vector3(2,5,2));
	spawnScenery(&mWallpanel,Vector3(20,3,14.4f),Vector3(0,0,0), Vector3(1,1.2f,1.1f));
	spawnScenery(&mFixture,Vector3(1,5,10),Vector3(0,PI/2,0));
	spawnLight(Vector3(1.3f,5.3f,10));


	//Office walls
	spawnScenery(&mWallpanel,Vector3(5,3,40),Vector3(0,1.5707963268f,0), CM::WALL_SCALE2);
	spawnDoor(Vector3(10,-3,40),Vector3(0,-PI/2,0),Vector3(2,5,2));
	spawnScenery(&mWallpanel,Vector3(17,3,40),Vector3(0,1.5707963268f,0), Vector3(1,1.2f,.65f));
	spawnScenery(&mWallpanel,Vector3(20,3,57.5f),Vector3(0,0,0), Vector3(1,1.2f,.5));
	spawnDoor(Vector3(20,-3,51),Vector3(0,PI,0),Vector3(2,5,2));
	spawnScenery(&mWallpanel,Vector3(20,3,45.6f),Vector3(0,0,0), Vector3(1,1.2f,1.1f));

	//MasterBed walls
	spawnScenery(&mWallpanel,Vector3(44.5f,3,45.5),Vector3(0,1.5707963268f,0), Vector3(1,1.2f,3.1f));
	spawnScenery(&mWallpanel,Vector3(60,3,37),Vector3(0,0,0), Vector3(1,1.2f,1.75f));
	spawnDoor(Vector3(60,-3.5f,30),Vector3(0,0,0),Vector3(2,4,3));
	spawnScenery(&mWallpanel,Vector3(60,3,16.6f),Vector3(0,0,0), Vector3(1,1.2f,1.5f));
	spawnDoor(Vector3(55.9f,-3.5f,9.2f),Vector3(0,-PI/2,0),Vector3(2,4,2));
	spawnScenery(&mWallpanel,Vector3(46,3,9),Vector3(0,PI/2,0), Vector3(1,1.2f,2));
	spawnScenery(&mWallpanel,Vector3(36,3,18),Vector3(0,0,0), Vector3(1,1.2f,1.75f));
	spawnScenery(&mWallpanel,Vector3(29,3,25.5f),Vector3(0,PI/2,0), Vector3(1,1.2f,1.4f));
	spawnScenery(&mWallpanel,Vector3(22,3,29.5f),Vector3(0,0,0), Vector3(1,1.2f,.8f));
	spawnScenery(&mWallpanel,Vector3(25.5f,3,33.5f),Vector3(0,PI/2,0), Vector3(1,1.2,.7));
	spawnScenery(&mWallpanel,Vector3(29,3,39.5f),Vector3(0,0,0), Vector3(1,1.2f,1.2f));
	spawnDoor(Vector3(36,-3.5f,26.7f),Vector3(0,PI,0),Vector3(2,4,2));
	spawnScenery(&mWallpanel,Vector3(36,3,37.95f),Vector3(0,0,0), Vector3(1,1.2f,1.45f));

	//Staircase to attic
	spawnScenery(&mCube,Vector3(37,-4,65),Vector3(0,0,0),Vector3(4,1,5));
	spawnScenery(&mWallpanel,Vector3(33,5,70),Vector3(0,0,0),Vector3(4,3,2));
	spawnScenery(&mWallpanel,Vector3(42,5,70),Vector3(0,0,0),Vector3(4,3,2));
	spawnScenery(&mWallpanel,Vector3(42,5,70),Vector3(0,0,0),Vector3(4,3,2));
	//spawnStaircase(L"upstairs",&CanisMajor::loadAttic,Vector3(37,3, 76), Vector3(0,PI/2,0), Vector3(1,1,.9f));
	spawnScenery(&mStaircase,Vector3(37,3, 76), Vector3(0,PI/2,0), Vector3(1,1,.9f));
	spawnScenery(&mStaircase,Vector3(37,11, 79), Vector3(0,PI/2,0), Vector3(1,4,.9f));
	spawnScenery(&mWallpanel,Vector3(37,10, 70), Vector3(0,0,PI/2), Vector3(1,1,3));

	//Staircase to main floor
	spawnScenery(&mWallpanel, Vector3(28,-4,15),Vector3(0,0,0),Vector3(1,4,1.2f));
	spawnDoor(Vector3(28,-3,25.3f),Vector3(0,0,0),Vector3(2,4,2.2f),nullptr);
	spawnScenery(&mWallpanel, Vector3(32,-4,9),Vector3(0,PI/2,0),Vector3(1,4,.8f));
	spawnScenery(&mWallpanel, Vector3(36,-18,15),Vector3(0,0,0),Vector3(1,3,1.2f));
	spawnScenery(&mStaircase, Vector3(32,-9,14),Vector3(0,PI/2,0),Vector3(1,1,1));
	spawnStaircase(L"downstairs",&CanisMajor::loadFirstFloor, Vector3(32,-3,20),Vector3(0,PI/2,0),Vector3(1,1,1));

	//master bed decor
	spawnScenery(&mMasterbed,Vector3(49,-3,39),Vector3(0,1.5707963268f,0), Vector3(5,4,5));
	spawnScenery(&mTable,Vector3(42,-3,43),Vector3(0,0,0), Vector3(.8f,1.5f,.8f));
	spawnReadable(&mBook3, L"Book", nullptr,Vector3(42,-0.4,43),Vector3(PI/16,PI,0),Vector3(1,1,1), failure);
	spawnScenery(&mTable,Vector3(55.5f,-3,43),Vector3(0,0,0), Vector3(.8f,1.5f,.8f));
	spawnReadable(&mBook2, L"Book", nullptr,Vector3(55.5,-0.4,43),Vector3(PI/16,PI,0),Vector3(1,1,1), theDog);
	spawnSearchable(&mDresser,L"Dresser",a,Vector3(40,-3,11),Vector3(0,-PI/2,0), Vector3(2,2,2));
	spawnSearchable(&mDresser,L"Dresser",nullptr,Vector3(50,-3,11),Vector3(0,-PI/2,0), Vector3(2,2,2));

	//Library decor
	//Bookcases
	spawnSearchable(&mBookcase,L"Bookcase",nullptr,Vector3(1,2.3,2.5),Vector3(0,-PI/2,0),CM::BOOKCASE_SCALE);
	spawnSearchable(&mBookcase,L"Bookcase",nullptr,Vector3(1,2.3,6.5),Vector3(0,-PI/2,0),CM::BOOKCASE_SCALE);
	spawnSearchable(&mBookcase,L"Bookcase",nullptr,Vector3(7,2.3,1),Vector3(0,PI,0),CM::BOOKCASE_SCALE);
	spawnSearchable(&mBookcase,L"Bookcase",nullptr,Vector3(11,2.3,1),Vector3(0,PI,0),CM::BOOKCASE_SCALE);
	spawnSearchable(&mBookcase,L"Bookcase",nullptr,Vector3(15,2.3,1),Vector3(0,PI,0),CM::BOOKCASE_SCALE);
	spawnSearchable(&mBookcase,L"Bookcase",r1,Vector3(17,2.3,19),Vector3(0,0,0),CM::BOOKCASE_SCALE);

	//Table with stuff on it and chair
	spawnSearchable(&mTable,L"Table",patKey,Vector3(5,-2.4,16),Vector3(0,PI/2,0),Vector3(1.5,1.5,1.5));
	spawnScenery(&mChair,Vector3(4.7,-1.8,11),Vector3(1,-PI/2,0), CM::CHAIR_SCALE);
	spawnReadable(&mBook, L"Book", nullptr,Vector3(5.0,.5,17.5),Vector3(0,PI/2,0),Vector3(.8,1,.8),thieves);
	spawnScenery(&mBookStack,Vector3(2.5,.5,16.8),Vector3(0,0,0),Vector3(1,1,1));
	spawnScenery(&mBottle,Vector3(5,1,13.5),Vector3(0,0,0),Vector3(1,1,1));
	spawnScenery(&mBottle,Vector3(2,.4,14.5),Vector3(0,0,PI/2),Vector3(1,1,1));

	//Office Decor
	spawnSearchable(&mDesk,L"Desk",r3,Vector3(2,-3,47),Vector3(0,PI,0),Vector3(1,1,1));
	spawnScenery(&mChair,Vector3(2.5,-2.6,46),Vector3(0,PI,.6), CM::CHAIR_SCALE);
	spawnScenery(&mBookStack,Vector3(1,1,48.5),Vector3(0,0,0),Vector3(1,1,1));
	spawnReadable(&mBook, L"Book", nullptr,Vector3(3.2,.65,45),Vector3(PI/8,PI/2,0),Vector3(.5,.75,.5),arrowLoc, 2.0f, GIGGLE1, true);
	spawnScenery(&mFixture,Vector3(1,5,51),Vector3(0,PI/2,0));
	spawnLight(Vector3(1.3,5.3,51));
	spawnScenery(&mTable,Vector3(6,-1.5,57),Vector3(0,0,0),Vector3(2,1,1.2));
	spawnScenery(&mChair,Vector3(4,-3,53),Vector3(0,-PI/2,0), CM::CHAIR_SCALE);
	spawnScenery(&mBookStack,Vector3(2,.7,55.5),Vector3(0,0,0),Vector3(1,1,1));
	spawnReadable(&mBook3, L"Book", nullptr,Vector3(6,.4,55),Vector3(0,PI,0),Vector3(1.5,1.5,1.5), itemList);
	spawnSearchable(&mBookcase,L"Bookcase",nullptr,Vector3(17,2.3,59),Vector3(0,0,0),CM::BOOKCASE_SCALE);
	spawnSearchable(&mBookcase,L"Bookcase",nullptr,Vector3(19,2.3,43),Vector3(0,PI/2,0),CM::BOOKCASE_SCALE);

	//Bathroom Decor
	spawnScenery(&mToilet, Vector3(25,-2.75,28.5),Vector3(0,PI/2,0),Vector3(1.5,1.5,1.5));
	spawnSearchable(&mSink, L"Sink",r2,Vector3(25,-2.75,32.5),Vector3(0,PI/2,0),Vector3(1.5,1.7,1.5));
	spawnSearchable(&mTub,L"Bath Tub",nullptr,Vector3(33.5,-1.2,40),Vector3(0,PI/2,0), Vector3(1.5,2,1.5));


	std::wstring treeNames[SpriteNS::SPRITES] = 
	{ //Put all file names in here.
		/* L".\\textures\\.dds", */
		L".\\textures\\tree0.dds",
		L".\\textures\\tree1.dds",
		L".\\textures\\tree2.dds",
		L".\\textures\\tree3.dds",
	};

	D3DXVECTOR3 centers3[256]; //Number of billboards placed
	float r = 160;
	float xOffset = 60;
	float zOffset = 30;
	for(int i = 0; i < 256; i++) { //Trees centered outside the area of the house
		float rander = RandF(90,120);
		Vector3 center(RandF(-50,50),-20,RandF(-50,50));
		Vector3 dir;
		Normalize(&dir, &center);
		center.x = dir.x*rander;
		center.z = dir.z*rander;
		center.x += xOffset;
		center.z += zOffset;
	
		centers3[i] = center;
	}

	//Was screwing up the lighting, I need to reset something correctly.
	trees.init(md3dDevice, centers3, 256, treeNames,4);

	//Ground for the trees
	spawnScenery(&mCube,Vector3(0,-27,25),Vector3(0,0,0),Vector3(160,1,160));
}