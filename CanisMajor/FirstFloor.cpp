#include "CanisMajor.h"

const wstring lastWords = L"Servant's Journal:\n The master has us holding onto the book.\n He told us to keep it in here,\n out of the twins' reach.";
const wstring dolan = L"Dolan said daddy's gonna put him down soon.\n Dolan said we could save him by hiding daddy's key,\n so we put in the servant's room!\n\n Scrawled in blood beneath:\n Thank you, children.";
const wstring pic1 = L"An artist's drawing of a man and his dog,\n striking a triumphant pose over his find.\nThe Nile can be seen in the background.";
const wstring pic2 = L"temp";
const wstring book3 = L"welp";
const wstring forgotten = L"The upper half of a ripped note:\n\"The fool will find it, and in his arrogance\n he will free that which was meant to remain chained.\"";
const wstring theEnd = L"";

void CanisMajor::loadFirstFloor()
{
	state.level = FIRST_FLOOR;
	state.bestLevel = FIRST_FLOOR;
	clearLevel();
	flashlight.isActive = true;
	camera.setPosition(Vector3(35,0,55));
	QuestItem *k=nullptr;
	k=spawnQuestItem(&mKey,L"Obscure Key",Vector3(15,2,-5),Vector3(0,0,0),Vector3(4,4,4));
	QuestItem* kidsRoomKey = spawnQuestItem(&mKey,L"Children's Key",Vector3(73,3.2,2.3),Vector3(0,PI/2,0),Vector3(2,2,2));
	kidsRoomKey->collisionType=AABBox;

	QuestItem* servantDoorKey = spawnQuestItem(&mKey,L"Servant's Key",Vector3(2.5,2,3),Vector3(0,PI/2,0),Vector3(3,3,3));
	servantDoorKey->collisionType=AABBox;

	dog.isActive = true;
	dog.setPosition(Vector3(47,0,14));
	dog.setTargetWPStage(1);
	dog.giveItem(k);

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
	spawnScenery(&mFixture,Vector3(48,4,69),Vector3(0,PI,0));
	spawnLight(Vector3(48,4.3,69));
	spawnScenery(&mFixture,Vector3(66,4,69),Vector3(0,PI,0));
	spawnLight(Vector3(66,4.3,69));
	spawnScenery(&mWallpanel,Vector3(57.5,2,70),Vector3(0,PI/2,0), CM::WALL_SCALE2);
	spawnScenery(&mWallpanel,Vector3(68.5,0,70),Vector3(0,PI/2,0),Vector3(1,1.6,1.25));
	spawnScenery(&mWallpanel,Vector3(75,0,62.5),Vector3(0,0,0),Vector3(1,1.6,1.5));
	spawnScenery(&mWallpanel,Vector3(57.5,0,55),Vector3(0,PI/2,0),Vector3(1,1.6,1.7));
	spawnScenery(&mWallpanel,Vector3(73,0,55),Vector3(0,PI/2,0),Vector3(1,1.6,.4));
	spawnScenery(&mWallpanel,Vector3(82.5,0,55),Vector3(0,PI/2,0),Vector3(1,1.6,1.5));
	spawnScenery(&mWindowPanel,Vector3(95,2,55),Vector3(0,PI/2,0), CM::WALL_SCALE2);
	spawnScenery(&mWallpanel,Vector3(110,0,55),Vector3(0,PI/2,0),Vector3(1,1.6,2));
	spawnDoor(Vector3(66,-4.2,55),Vector3(0,-PI/2,0),Vector3(2,4,2.5),servantDoorKey);
	spawnScenery(&mDoor,Vector3(44,6-4.2,55),Vector3(0,-PI/2,0),Vector3(2,4,2.5));

	//East Wall
	spawnScenery(&mWallpanel,Vector3(120,0,47.5),Vector3(0,0,0), Vector3(1,1.6,1.5));
	for(int i = 0; i<4; i++)
	{
		spawnScenery(&mWindowPanel,Vector3(120,2,35-(i*10)),Vector3(0,0,0), CM::WALL_SCALE2);
	}
	spawnScenery(&mFixture,Vector3(119.2,4,30),Vector3(0,-PI/2,0));
	spawnLight(Vector3(119.2,4.3,30));
	spawnScenery(&mFixture,Vector3(119.2,4,20),Vector3(0,-PI/2,0));
	spawnLight(Vector3(119.2,4.3,20));
	spawnScenery(&mFixture,Vector3(119.2,4,10),Vector3(0,-PI/2,0));
	spawnLight(Vector3(119.2,4.3,10));

	//South wall
	spawnScenery(&mWallpanel,Vector3(91,0,0),Vector3(0,PI/2,0),Vector3(1,1.6,5.8));
	spawnScenery(&mWindowPanel,Vector3(57,2,0),Vector3(0,PI/2,0),CM::WALL_SCALE2);
	spawnScenery(&mWindowPanel,Vector3(38,2,0),Vector3(0,PI/2,0),CM::WALL_SCALE2);
	spawnScenery(&mWallpanel,Vector3(16.5,0,0),Vector3(0,PI/2,0),Vector3(1,1.6,3.3));
	spawnDoor(Vector3(53,-4.2,0),Vector3(0,PI/2,0),Vector3(2,4,5),k);

	//spawn exit
	spawnScenery(&mCube,Vector3(47.5,-4,-10),Vector3(0,0,0),Vector3(6,1,10));

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
	spawnDoor(Vector3(20,-4.2,5),Vector3(0,PI,0),Vector3(2,4,2.5),kidsRoomKey);

	//East bathroom wall
	spawnScenery(&mWallpanel,Vector3(20,0,28.75),Vector3(0,0,0),Vector3(1,1.6,2.75));
	spawnDoor(Vector3(7.5,-4.2,15),Vector3(0,-PI/2,0),Vector3(2,4,2.5),nullptr);
	spawnDoor(Vector3(12.5,-4.2,35),Vector3(0,PI/2,0),Vector3(2,4,2.5),nullptr);

	//Children room
	spawnScenery(&mWallpanel,Vector3(20,0,51.25),Vector3(0,0,0),Vector3(1,1.6,.75));
	spawnScenery(&mWallpanel,Vector3(16.25,0,35),Vector3(0,PI/2,0),Vector3(1,1.6,.75));
	spawnScenery(&mWallpanel,Vector3(3.75,0,35),Vector3(0,PI/2,0),Vector3(1,1.6,.75));
	spawnDoor(Vector3(20,-4.2,42.5),Vector3(0,PI,0),Vector3(2,4,2.5),kidsRoomKey);

	//Kitchen and closet walls
	spawnScenery(&mWallpanel,Vector3(90,0,14),Vector3(0,0,0),Vector3(1,1.6,.25));
	spawnScenery(&mWallpanel,Vector3(90,0,1.25),Vector3(0,0,0),Vector3(1,1.6,.25));
	spawnScenery(&mWallpanel,Vector3(82.5,0,15),Vector3(0,PI/2,0),Vector3(1,1.6,1.5));
	spawnScenery(&mWallpanel,Vector3(75,0,17.5),Vector3(0,0,0),Vector3(1,1.6,.6));
	spawnScenery(&mWallpanel,Vector3(82.5,0,20),Vector3(0,PI/2,0),Vector3(1,1.6,1.5));
	spawnScenery(&mWallpanel,Vector3(90,0,32.5),Vector3(0,0,0),Vector3(1,1.6,2.5));
	//spawnDoor(Vector3(90,-4.2,15.1),Vector3(0,PI,0),Vector3(2,4,2.5),nullptr);
	QuestItem * sbK = spawnQuestItem(&mBook,L"Book",Vector3(42,2.8,67));
	slidingBookcase.create(Vector3(90.4,1.5,17.6),Vector3(0,-PI/2,0),CM::BOOKCASE_SCALE+Vector3(.8,.4,0),sbK);

	//North interior walls
	spawnScenery(&mWallpanel,Vector3(87.5,0,45),Vector3(0,PI/2,0),Vector3(1,1.6,2));
	spawnScenery(&mWallpanel,Vector3(117.5,0,45),Vector3(0,PI/2,0),Vector3(1,1.6,.5));

	//Ceiling
	spawnScenery(&mWallpanel,Vector3(15,8,25),Vector3(0,0,PI/2),Vector3(1,3,6));
	spawnScenery(&mWallpanel,Vector3(70,8,50),Vector3(0,0,PI/2),Vector3(1,10,1));
	spawnScenery(&mWallpanel,Vector3(57.5,8,62.5),Vector3(0,0,PI/2),Vector3(1,3.5,1.5));
	spawnScenery(&mWallpanel,Vector3(77.5,8,23),Vector3(0,0,PI/2),Vector3(1,2.5,5));

	//Parlor Ceiling
	spawnScenery(&mWallpanel,Vector3(57,14.6,22.5),Vector3(0,0,1),Vector3(1,2.4,4.5));
	spawnScenery(&mWallpanel,Vector3(37,14.6,22.5),Vector3(0,0,-1),Vector3(1,2.4,4.5));
	spawnScenery(&mWallpanel,Vector3(47,17,0),Vector3(0,PI/2,0),Vector3(1,2,3.9));
	spawnScenery(&mWallpanel,Vector3(47,17.7,45),Vector3(0,PI/2,0),Vector3(1,2,3.9));
	spawnScenery(&mFixture,Vector3(47.5,8,1),Vector3(0,0,0));
	spawnLight(Vector3(47.5,8.3,1));

	//Dining Hall Ceiling
	spawnScenery(&mWallpanel,Vector3(110,13.8,22.5),Vector3(0,0,1),Vector3(1,2.35,4.5));
	spawnScenery(&mWallpanel,Vector3(100,13.8,22.5),Vector3(0,0,-1),Vector3(1,2.35,4.5));
	spawnScenery(&mWallpanel,Vector3(103,17,0),Vector3(0,PI/2,0),Vector3(1,2.2,4.2));
	spawnScenery(&mWallpanel,Vector3(103,17,45),Vector3(0,PI/2,0),Vector3(1,2.2,4.2));

	//spawnSprite(Vector3(105,12.5,28),L".\\textures\\chandelier.png");

	//Stairwell
	//Staircase *sec = spawnStaircase(L"Upstairs",&CanisMajor::loadSecondFloor,Vector3(35,8,69),Vector3(0,PI/2,0),Vector3(2,2,1.2));
	//sec->setStateSwitch(&state,&GameState::firstFloorSairsUsed);
	spawnScenery(&mStaircase,Vector3(35,8,69),Vector3(0,PI/2,0),Vector3(2,2,1.2));
	spawnScenery(&mWallpanel,Vector3(31.5,0,60),Vector3(0,0,0),CM::WALL_SCALE);
	spawnScenery(&mWallpanel,Vector3(35,8.5,62),Vector3(0,0,PI/2),Vector3(1,2,1.5));
	spawnScenery(&mWallpanel,Vector3(35,-4,62),Vector3(0,0,PI/2),Vector3(1,2,1.5));

	//Putting crap in the level

	

	//Baby room
	spawnSearchable(&mCradle,L"Cradle",servantDoorKey,Vector3(2.5,-3,3),Vector3(0,.4,0), Vector3(1,1,1), CRYING, true);
	spawnScenery(&mTable,Vector3(13,-1,4),Vector3(PI,0.2,0),Vector3(.5,.7,.4));
	spawnScenery(&mChair,Vector3(14,-2,12),Vector3(PI/2,0.2,0),Vector3(1,1,1));
	spawnScenery(&mFixture,Vector3(10,4,1),Vector3(0,0,0));
	spawnLight(Vector3(10,4.3,1));

	//Kid room
	spawnSearchable(&mServantbed,L"Bed",nullptr,Vector3(5,-3,50),Vector3(0,0,0),Vector3(1,1,1.5));
	spawnSearchable(&mServantbed,L"Bed",nullptr,Vector3(13,-3,50),Vector3(0,0,0),Vector3(1,1,1.5));
	spawnSearchable(&mDresser,L"Dresser",nullptr,Vector3(3,-3,37),Vector3(0,-PI/2,0),Vector3(1.5,1.5,1));
	spawnSearchable(&mDresser,L"Dresser",nullptr,Vector3(16,-3,37),Vector3(0,-PI/2,0),Vector3(1.5,1.5,1));
	spawnReadable(&mBook,L"Child's Notes",nullptr,Vector3(16,2.2,37),Vector3(0,0,0),Vector3(.75,.75,.75), dolan, 2.0f, GIGGLE2, true);

	//Bathroom
	spawnScenery(&mTub,Vector3(3,-1.2,25),Vector3(0,PI/2,0),Vector3(1.5,2,1.5));
	spawnSearchable(&mSink, L"Sink",nullptr,Vector3(18,-2.75,21),Vector3(0,PI,0),Vector3(1.5,1.7,1.5));
	spawnScenery(&mToilet, Vector3(18,-2.75,27),Vector3(0,-PI/2,0),Vector3(1.5,1.5,1.5));

	//Kitchen
	spawnScenery(&mStove,Vector3(85,-3.5,1.2),Vector3(0,0,0),Vector3(1.7,2,1.6));
	spawnLight(Vector3(85,-1.5,2),6);

	spawnScenery(&mSinkCounter,Vector3(77,-0.9,1.5),Vector3(0,-PI/2,0),Vector3(2,2.05,2));
	Door* j = spawnDoor(Vector3(74.7,-7,3.6),Vector3(PI,-PI/2,0),CM::CABINET_DOOR2,nullptr);
	j->setGeometry(&mCabDoor);
	Door* l = spawnDoor(Vector3(77.7,-7,3.6),Vector3(PI,-PI/2,0),CM::CABINET_DOOR2,nullptr);
	l->setGeometry(&mCabDoor);

	spawnScenery(&mCounter,Vector3(69,-0.9,1.5),Vector3(0,-PI/2,0),Vector3(2,2.05,2));
	Door* m = spawnDoor(Vector3(69.5,-7,3.6),Vector3(PI,-PI/2,0),CM::CABINET_DOOR2,nullptr);
	m->setGeometry(&mCabDoor);
	Door* n = spawnDoor(Vector3(66.3,-7,3.6),Vector3(PI,-PI/2,0),CM::CABINET_DOOR2,nullptr);
	n->setGeometry(&mCabDoor);
	
	//South cabinets
	for(int i = 75; i>66;i-=2)
		spawnScenery(&mCabinet,Vector3(i,4.5,1.5),Vector3(0,-PI/2,0),Vector3(1,1,1));
	Door* a = spawnDoor(Vector3(74,-1.55,2.5),Vector3(PI,-PI/2,0),CM::CABINET_DOOR,nullptr);
	a->setGeometry(&mCabDoor);
	Door* b = spawnDoor(Vector3(72,-1.55,2.5),Vector3(PI,-PI/2,0),CM::CABINET_DOOR,nullptr);
	b->setGeometry(&mCabDoor);
	Door* c = spawnDoor(Vector3(70,-1.55,2.5),Vector3(PI,-PI/2,0),CM::CABINET_DOOR,nullptr);
	c->setGeometry(&mCabDoor);
	Door* d = spawnDoor(Vector3(68,-1.55,2.5),Vector3(PI,-PI/2,0),CM::CABINET_DOOR,nullptr);
	d->setGeometry(&mCabDoor);
	Door* e = spawnDoor(Vector3(66,-1.55,2.5),Vector3(PI,-PI/2,0),CM::CABINET_DOOR,nullptr);
	e->setGeometry(&mCabDoor);
	

	spawnScenery(&mCounter,Vector3(83,-0.9,12.5),Vector3(0,PI/2,0),Vector3(2,2.05,2));
	Door* o = spawnDoor(Vector3(82.55,-7,10.5),Vector3(PI,-3*PI/2,0),CM::CABINET_DOOR2,nullptr);
	o->setGeometry(&mCabDoor);
	Door* p = spawnDoor(Vector3(85.7,-7,10.5),Vector3(PI,-3*PI/2,0),CM::CABINET_DOOR2,nullptr);
	p->setGeometry(&mCabDoor);

	//North cabinets
	for(int i = 86; i>79;i-=2)
		spawnScenery(&mCabinet,Vector3(i,4.5,13.8),Vector3(0,PI/2,0),Vector3(1,1,1));

	Door* f = spawnDoor(Vector3(87,-1.55,12.8),Vector3(PI,-3*PI/2,0),CM::CABINET_DOOR,nullptr);
	f->setGeometry(&mCabDoor);
	Door* g = spawnDoor(Vector3(85,-1.55,12.8),Vector3(PI,-3*PI/2,0),CM::CABINET_DOOR,nullptr);
	g->setGeometry(&mCabDoor);
	Door* h = spawnDoor(Vector3(83,-1.55,12.8),Vector3(PI,-3*PI/2,0),CM::CABINET_DOOR,nullptr);
	h->setGeometry(&mCabDoor);
	Door* i = spawnDoor(Vector3(81,-1.55,12.8),Vector3(PI,-3*PI/2,0),CM::CABINET_DOOR,nullptr);
	i->setGeometry(&mCabDoor);

	spawnScenery(&mFixture,Vector3(8,4,54),Vector3(0,PI,0));
	spawnLight(Vector3(8,4.3,54));

	staff.create(Vector3(77,0,17));

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
		Vector3 center(RandF(-50,50),0,RandF(-50,50));
		//center.y = ;
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
	spawnScenery(&mCube,Vector3(0,-7,25),Vector3(0,0,0),Vector3(300,1,300));
	

	//servant quarters
	spawnSearchable(&mServantbed,L"Servant Bed",nullptr,Vector3(52,-3,59),Vector3(0,0,0),Vector3(1,1,1.5));
	spawnSearchable(&mServantbed,L"Servant Bed",nullptr,Vector3(61,-3,59),Vector3(0,0,0),Vector3(1,1,1.5));

	spawnSearchable(&mTable,L"Table",nullptr,Vector3(72,-2.5,67),Vector3(0,0,0),Vector3(0.75,0.75,0.75));
	spawnReadable(&mBook2,L"Journal",nullptr,Vector3(72,-1,67),Vector3(0,0,0),Vector3(0.75,0.75,0.75), lastWords, 2.0f);
	spawnSearchable(&mDresser,L"Dresser",sbK,Vector3(42,-2.5,67),Vector3(0,0,0),Vector3(1,1.5,1));

	spawnSearchable(&mBookcase,L"Bookcase",nullptr,Vector3(61,2,69.5),Vector3(0,0,0),CM::BOOKCASE_SCALE);
	spawnScenery(&mBookcase,Vector3(46,2,58.6),Vector3(-PI/5,0,0),CM::BOOKCASE_SCALE);

	spawnScenery(&mBook,Vector3(46,-2,57),Vector3(0,0,0));
	spawnScenery(&mBook2,Vector3(46,-2.5,56.5),Vector3(0,PI/3,0));
	spawnScenery(&mBook3,Vector3(47,-2.5,58),Vector3(0,PI/4,0));
	spawnScenery(&mBook,Vector3(45,-2.5,59),Vector3(0,-PI/5,0));
	spawnScenery(&mBook2,Vector3(48,-2.5,57),Vector3(0,PI/6,0));
	spawnScenery(&mBook3,Vector3(44,-2.5,58),Vector3(0,PI/5,0));
	spawnScenery(&mBook,Vector3(43,-2.5,57),Vector3(0,-PI/3,0));

	//Dining room
	spawnScenery(&mTable,Vector3(105,-1.5,25),Vector3(0,0,0),Vector3(2,1.2,4));
	spawnScenery(&mChair,Vector3(99.5,-1.8,14),Vector3(PI/2,0.4,0),CM::CHAIR_SCALE);
	spawnScenery(&mChair,Vector3(99,-3,20),Vector3(0,.3,0),CM::CHAIR_SCALE);
	spawnScenery(&mChair,Vector3(100,3,25),Vector3(0,PI,PI),CM::CHAIR_SCALE);
	spawnScenery(&mChair,Vector3(100,-2,28),Vector3(0,0,PI/2),CM::CHAIR_SCALE);
	spawnScenery(&mChair,Vector3(98,-0.5,32),Vector3(0,-PI/2,-2.2),CM::CHAIR_SCALE);
	spawnScenery(&mChair,Vector3(91,-0.5,32),Vector3(0,0,-2.2),CM::CHAIR_SCALE);
	spawnScenery(&mChair,Vector3(95,-3,34),Vector3(0,PI/2+.3,0),CM::CHAIR_SCALE);
	spawnScenery(&mChair,Vector3(110,-0.5,20),Vector3(0,-PI/2,-2.2),CM::CHAIR_SCALE);
	spawnScenery(&mChair,Vector3(109,3,17),Vector3(0,0,PI),CM::CHAIR_SCALE);
	spawnScenery(&mChair,Vector3(109,-3,31),Vector3(0,PI-.3,0),CM::CHAIR_SCALE);
	spawnScenery(&mChair,Vector3(109,-1.8,13),Vector3(PI/2,PI+.4,0),CM::CHAIR_SCALE);
	spawnScenery(&mChair,Vector3(106,2.85,27),Vector3(0,-PI/2,-2.2),CM::CHAIR_SCALE);
	spawnScenery(&mWineGlass,Vector3(104,.8,20),Vector3(0,0,0),CM::WINE_GLASS);
	spawnScenery(&mWineGlass,Vector3(106,.8,16),Vector3(0,0,0),CM::WINE_GLASS);
	spawnScenery(&mWineGlass,Vector3(102,.8,30),Vector3(PI/2,0,0),CM::WINE_GLASS);
	spawnScenery(&mTeaSaucer,Vector3(108,.6,33),Vector3(0,0,0),Vector3(.1,.1,.1));
	spawnScenery(&mTeaCup,Vector3(105,.6,23),Vector3(0,0,0),Vector3(.1,.1,.1));
	spawnScenery(&mTeaSaucer,Vector3(108.5,.6,12.5),Vector3(0,0,0),Vector3(.1,.1,.1));
	spawnScenery(&mTeaCup,Vector3(102,1,13),Vector3(PI,0,0),Vector3(.1,.1,.1));
	spawnReadable(&mBook3, L"Book", nullptr,Vector3(101,.8,23),Vector3(0,PI,0),Vector3(1.5,1.5,1.5),forgotten);

	//Livingroom Decor
	spawnSearchable(&mSofa,L"Sofa",nullptr,Vector3(85,0,40),Vector3(0,-2,0),Vector3(4,4,4));
	spawnSearchable(&mSofa,L"Sofa",nullptr,Vector3(85,0,25),Vector3(0,-1,0),Vector3(4,4,4));
	spawnSearchable(&mFireplace,L"Fireplace",nullptr,Vector3(65.8,-3,35),Vector3(0,PI/2,0),Vector3(1,1,1));
	spawnReadable(&mPictureframe,L"Picture",nullptr,Vector3(65.5,3,24),Vector3(0,PI,PI/2),Vector3(1,1,1), pic1);
	spawnReadable(&mPictureframe,L"Picture",nullptr,Vector3(89.3,3,32),Vector3(0,0,PI/2),Vector3(1,1,1), L"A scratched picture");

	//Parlor Decor
	spawnReadable(&mPictureframe,L"Picture",nullptr,Vector3(64.5,3,24),Vector3(0,0,PI/2),Vector3(1,1,1), L"A scratched picture");
	spawnReadable(&mPictureframe,L"Picture",nullptr,Vector3(64.5,3,38),Vector3(0,0,PI/2),Vector3(1,1,1), L"A worn of drawing of the pyramids.");
	spawnReadable(&mPictureframe,L"Picture",nullptr,Vector3(30.3,3,24),Vector3(0,PI,PI/2),Vector3(1,1,1), L"A scratched picture");
	spawnReadable(&mPictureframe,L"Picture",nullptr,Vector3(30.3,3,38),Vector3(0,PI,PI/2),Vector3(1,1,1), L"A family portrait, showing a man, his wife, \n their two children, and the family dog");
}