#include"CanisMajor.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif


	CanisMajor theApp(hInstance);

	theApp.initApp();

	return theApp.run();
}



CanisMajor::CanisMajor(HINSTANCE hInstance)
	: D3DApp(hInstance), mFX(0), mTech(0), mVertexLayout(0),
	mfxWVPVar(0), mfxWorldVar(0),  mfxEyePosVar(0), 
	mfxLightVar(0), mfxLightBool(0), mfxDiffuseMapVar(0),
	mfxSpecMapVar(0), mfxTexMtxVar(0)
{
	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mProj);

	controls.up = 'W';
	controls.down = 'S';
	controls.left = 'A';
	controls.right = 'D';
	controls.use = VK_LBUTTON;
	controls.altUse = 'E';
	controls.flashlight = 'F';
	controls.crouch = VK_CONTROL;
	controls.run = VK_SHIFT;
	controls.recharge = 'R';
	controls.fire = VK_RBUTTON;

	birminghamMode=false;

	//Camera Object
	camera.init(this,&mCube,controls);

	scenery = new Actor[CM::MAX_SCENERY];
	searchableActors = new SearchableActor[CM::MAX_SEARCHABLE_ACTORS];
	readableActors = new ReadableActor[CM::MAX_READABLE_ACTORS];
}

CanisMajor::~CanisMajor()
{
	if( md3dDevice )
		md3dDevice->ClearState();

	delete [] scenery;
	delete [] searchableActors;
	delete [] readableActors;

	ReleaseCOM(mFX);
	ReleaseCOM(mVertexLayout);

}

DWORD WINAPI threadFunct(LPVOID lpParameter)
{
	static_cast<CanisMajor*>(lpParameter)->threadInit();
	return 0;
}

void CanisMajor::initApp()
{

	D3DApp::initApp();

	buildFX();

	threadComplete = false;
	loadingStatus = 0;
	loadingThread = CreateThread(0,0,threadFunct,this,0,0);

	buildVertexLayouts();
	menuLoad();

}

void CanisMajor::threadInit()
{
	sky.init(md3dDevice, this, 5000.0f);

	for(int i = 0; i < MAX_LIGHTS; i++) {
		rLights[i].init();
		rLights[i].pos = Vector3(0, -200, 0);
		lightType[i] = 0;
	}

	D3DXVECTOR3 centers[guiNS::SPRITES];
	for(int i = 0; i < guiNS::SPRITES; i++) {
		Vector3 center(0,0,100);
		centers[i] = center;
	}

	//Make sure to have the exact number of file names
	std::wstring guiNames[guiNS::SPRITES] = 
	{ //Put all file names in here.
		/* L".\\textures\\.dds", */
		L".\\textures\\hand.dds",
		L".\\textures\\book.dds",
		L".\\textures\\maglass.dds",
		L".\\textures\\sDoor.dds",
		L".\\textures\\arrow.dds",
		L".\\textures\\arrowDown.dds",
		L".\\textures\\largeWhite.png"
	};

	gui.init(md3dDevice, centers, MAX_GUI, guiNames);

	D3DXVECTOR3 centers2[SpriteNS::SPRITES];
	for(int i = 0; i < SpriteNS::SPRITES; i++) {
		Vector3 center(0,0,100);
		centers2[i] = center;
	}

	//Make sure to have the exact number of file names
	//std::wstring spriteNames[SpriteNS::SPRITES] = 
	//{ //Put all file names in here.
	//	/* L".\\textures\\.dds", */
	//	L".\\textures\\hand.dds",
	//	L".\\textures\\book.dds",
	//	L".\\textures\\book.dds",
	//	L".\\textures\\book.dds",
	//};
	//int billboards = 16;
	//sprite.init(md3dDevice, centers2, billboards, spriteNames);

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
		Vector3 center(RandF(-50,50),-15,RandF(-50,50));
		center.y = -15;
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

	// Spotlight--position and direction changed every frame to animate.
	fLight.init(2);  //Flashlight
	ambient.init(1); //Ambientlight (DUH)
	negaLight.init(3); //Dog's anti-light
	negaLight.pos = Vector3(-20,0,0);
	eyes.init(5);
	eyes.pos = Vector3(0,-200,0);
	activeLights = 0;

	waiting = false;
	howl = false;

	mTelescope.init(md3dDevice,".\\geometry\\telescope.geo");
	loadingStatus++; //1
	mDresser.init(md3dDevice,".\\geometry\\dresser.geo", L".\\textures\\lightwood.dds");
	loadingStatus++; //2
	mFlashlight.init(md3dDevice,".\\geometry\\flashlight.geo", L".\\textures\\grey.dds");
	flashlight.init(this,&mFlashlight,&fLight);
	loadingStatus++; //3
	mFrame.init(md3dDevice,".\\geometry\\pictureframe.geo");
	loadingStatus++; //4
	mBookcase.init(md3dDevice,".\\geometry\\bookcase.geo", L".\\textures\\wood2.dds",true);
	loadingStatus++; //5
	mChair.init(md3dDevice,".\\geometry\\chair.geo", L".\\textures\\wood3.dds",true);
	loadingStatus++; //6
	mCradle.init(md3dDevice,".\\geometry\\cradle.geo", L".\\textures\\medwood.dds");
	loadingStatus++; //7
	mMasterbed.init(md3dDevice,".\\geometry\\masterBed.geo", L".\\textures\\medwood.dds");
	loadingStatus++; //8
	mServantbed.init(md3dDevice,".\\geometry\\servantBed.geo",L".\\textures\\medwood.dds");
	loadingStatus++; //9
	mStaircase.init(md3dDevice,".\\geometry\\staircase.geo", L".\\textures\\lightwood.dds");
	mStaircase.setCustomAABB(mStaircase.getAABBMin(),mStaircase.getAABBMax()+Vector3(0,10,0));
	loadingStatus++; //10
	mTable.init(md3dDevice,".\\geometry\\table.geo", L".\\textures\\wood.dds",true);
	loadingStatus++; //11
	mBottle.init(md3dDevice,".\\geometry\\bottle.geo", L".\\textures\\bottlegreen.dds");
	loadingStatus++; //12
	mLock.init(md3dDevice,".\\geometry\\lock.geo");
	loadingStatus++; //13
	mPictureframe.init(md3dDevice,".\\geometry\\pictureframe.geo");
	loadingStatus++; //14
	mRail.init(md3dDevice,".\\geometry\\rail.geo");
	loadingStatus++; //15
	mWallpanel.init(md3dDevice,".\\geometry\\wallpanel.geo", L".\\textures\\pine3.dds",true);
	loadingStatus++; //16
	mCage.init(md3dDevice,".\\geometry\\cage.geo");
	loadingStatus++; //17
	mFixture.init(md3dDevice,".\\geometry\\fixture.geo", L".\\textures\\cardboard.dds");
	loadingStatus++; //18
	mDoor.init(md3dDevice,".\\geometry\\door.geo", L".\\textures\\pine1.dds",true);
	loadingStatus++; //19
	mBox.init(md3dDevice,".\\geometry\\cardboardBox.geo", L".\\textures\\cardboard2.dds",true);
	loadingStatus++; //20
	mBook.init(md3dDevice,".\\geometry\\book2.geo",L".\\textures\\book_tex.dds", true,L".\\textures\\book_spec.dds");
	mBook2.init(md3dDevice,".\\geometry\\book3.geo",L".\\textures\\Book1.dds", true);
	mBook3.init(md3dDevice,".\\geometry\\book4.geo",L".\\textures\\Book2.dds", true);
	loadingStatus++; //21
#if !defined(DEBUG) || !defined(DEBUG)
	mToilet.init(md3dDevice,".\\geometry\\toilet.geo");
#else
	mToilet.init(md3dDevice,".\\geometry\\cardboardBox.geo", L".\\textures\\paper.dds");
#endif
	loadingStatus++; //22
#if  !defined(DEBUG) || !defined(DEBUG)
	mDog.init(md3dDevice,".\\geometry\\dog.geo");
#else
	mDog.init(md3dDevice,".\\geometry\\cardboardBox.geo", L".\\textures\\paper.dds");
#endif
	loadingStatus++; //23
	for(int i = 0 ; i < CM::MAX_DOORS; i++)
	{
		doors[i].init(this,&mDoor,1);
		doors[i].collisionType = AABBox;
		doors[i].name = L"door";
	}

	mCube.init(md3dDevice,".\\geometry\\cube.geo", L".\\textures\\metal.dds", true);
	mFloor.init(md3dDevice,".\\geometry\\cube2.geo",L".\\textures\\woodfloor.dds",true);
	dog.init(this,&mDog,3.0f,Vector3(1,2,2));
	//dog.setScale(Vector3(0.1f,5.0f,5.0f));
	dog.setNegalight(&negaLight);
	dog.setEyes(&eyes);
	dog.LoadWaypoints(".\\lvl2.txt",3,2);
	dog.LoadWaypoints(".\\lvl1.txt",3,1);
	loadingStatus++; //24
	mRoofHole.init(md3dDevice,".\\geometry\\newRoofHole.geo", L".\\textures\\pine3.dds",true);
	loadingStatus++; //25
	mKey.init(md3dDevice,".\\geometry\\key2.geo", L".\\textures\\key1D.dds", true);
	loadingStatus++; //26
	mWindowPanel.init(md3dDevice,".\\geometry\\windowpanel.geo", L".\\textures\\pine4.dds",true);
	loadingStatus++; //27
	mBookStack.init(md3dDevice,".\\geometry\\bookStack.geo");
	loadingStatus++; //28
	mDesk.init(md3dDevice,".\\geometry\\desk.geo");
	loadingStatus++; //29
#if !defined(DEBUG) || !defined(DEBUG)
	mSink.init(md3dDevice,".\\geometry\\sink.geo");
#else
	mSink.init(md3dDevice,".\\geometry\\cardboardBox.geo", L".\\textures\\paper.dds");
#endif
	loadingStatus++; //30
#if !defined(DEBUG) || !defined(DEBUG)
	mTub.init(md3dDevice,".\\geometry\\tub.geo");
#else
	mTub.init(md3dDevice,".\\geometry\\cardboardBox.geo", L".\\textures\\paper.dds");
#endif
	loadingStatus++;
	mArrow.init(md3dDevice,".\\geometry\\arrow.geo", L".\\textures\\gold.dds");
	loadingStatus++;
	mRing.init(md3dDevice,".\\geometry\\ring.geo", L".\\textures\\gold.dds");
	loadingStatus++;
	mPedastal.init(md3dDevice,".\\geometry\\pedastal.geo");
	loadingStatus++;
	mStove.init(md3dDevice,".\\geometry\\wood_stove.geo",L".\\textures\\metal.dds");
	loadingStatus++;
	mCounter.init(md3dDevice,".\\geometry\\counter.geo");
	loadingStatus++;
	mSinkCounter.init(md3dDevice,".\\geometry\\counter_sink.geo");
	loadingStatus++;
	mCabinet.init(md3dDevice,".\\geometry\\cabinet.geo",L".\\textures\\wood2.dds");
	loadingStatus++;
	mCabDoor.init(md3dDevice,".\\geometry\\CabDoor.geo",L".\\textures\\wood3.dds");
	loadingStatus++;
	pedestal.init(this,&mPedastal);
	pedestal.collisionType = AABBox;

	mStaff.init(md3dDevice,".\\geometry\\staff.geo",L".\\textures\\gold.dds");
	staff.init(this,&mStaff);
	loadingStatus++;
	mSphere.init(md3dDevice,".\\geometry\\sphere.geo",L".\\textures\\white.bmp");
	loadingStatus++;
	mWineGlass.init(md3dDevice,".\\geometry\\wineGlass.geo");//Wine glass model made by Cold999 of blendswap.com
	loadingStatus++;
	mTeaCup.init(md3dDevice,".\\geometry\\teaCup.geo");//Teacup, spoon and suacer models made by oenvoyage of blendswap.com
	loadingStatus++;
	#if !defined(DEBUG) || !defined(DEBUG)
	mTeaSaucer.init(md3dDevice,".\\geometry\\teaSaucer.geo");//Teacup, spoon and suacer models made by oenvoyage of blendswap.com
#else
	mTeaSaucer.init(md3dDevice,".\\geometry\\cardboardBox.geo", L".\\textures\\paper.dds");
#endif
	
	loadingStatus++;
	for(int i = 0 ; i < CM::NUM_QUEST_ITEMS; i++)
	{
		items[i].init(this,&mCube,1);
	}
	loadingStatus++;
	origin.init(this,1);

	for(int i = 0; i< CM::MAX_SCENERY; i++)
	{
		//default init
		scenery[i].init(this,&mCube);
		scenery[i].collisionType=AABBox;
	}
	loadingStatus++;
	for(int i = 0; i< CM::MAX_SEARCHABLE_ACTORS; i++)
	{
		searchableActors[i].init(this,&mCube,1);
		searchableActors[i].collisionType=AABBox;
	}
	loadingStatus++;
	for(int i = 0; i< CM::MAX_READABLE_ACTORS; i++)
	{
		readableActors[i].init(this,&mCube,1);
		readableActors[i].collisionType=AABBox;
	}
	loadingStatus++;
	for(int i = 0 ; i < CM::MAX_STAIRCASES; i++)
	{
		staircases[i].init(this,&mStaircase,10);
		staircases[i].collisionType=AABBox;
	}
	loadingStatus++;
	for(int i = 0; i < CM::NUM_BOLTS; i++){
		bolts[i].init(this,&mSphere);
		bolts[i].collisionType=SPHERE;
		bolts[i].setScale(Vector3(.5,.5,.5));
	}
	loadingStatus++;
	camera.create(Vector3(10,10,10),Vector3(1,0,0));
	camera.setPerspective();

	slidingBookcase.init(this,&mBookcase,1,CM::BOOKCASE_SCALE);

	flashlight.toggle();

#if defined(DEBUG) || defined(DEBUG)
	mRedCube.init(md3dDevice,".\\geometry\\cube.geo", L".\\textures\\metal.dds", true);
	AABBHelper.init(this,&mRedCube,1);
	AABBHelper.isActive = true;

	camera.setFlashlight(&flashlight);
#endif

	camera.update(0);

	//buildFX();
	buildVertexLayouts();

	loadingStatus++;
	threadComplete = true;
}

void CanisMajor::onResize()
{
	D3DApp::onResize();
	camera.setPerspective();
}

void CanisMajor::updateScene(float dt)
{
	if(!threadComplete)
		return;
	D3DApp::updateScene(dt);
	if(GetAsyncKeyState(VK_ESCAPE))
		PostQuitMessage(0);
	switch(state.level){
	case MENU:
	case SPLASH:
		menuUpdate(dt);
		break;
	default:
		levelsUpdate(dt);
		break;
	}
}


void CanisMajor::menuUpdate(float dt, bool reset)
{
	if(!threadComplete)
		return;

	static bool isKeyDown = true;

	if(reset)
	{
		menuChoice = 1;
		isKeyDown = true;
	}

	if(GetAsyncKeyState('D')&&GetAsyncKeyState('R')&&GetAsyncKeyState('B')){
		birminghamMode=true;
		menuText[1]=L"BIRMINGHAM MODE";
	}

	if(GetAsyncKeyState(VK_RETURN)||GetAsyncKeyState(' '))
	{ 
		if(!isKeyDown)
		{
			switch(menuChoice)
			{
			case 1://play
				if(state.level==SPLASH)
					menuLoad();
				else {
					audio->playCue(BG);
#if defined(DEBUG) || defined(DEBUG)		//Use this for testing a specific level
					loadFirstFloor();
#else				//Use this to run the full game
					loadAttic();
#endif
				}
				break;
			case 2://quit
				PostQuitMessage(0);
				break;
			}
		}
	}
	else if(GetAsyncKeyState('W')||GetAsyncKeyState(VK_UP))
	{
		if(!isKeyDown)
		{
			menuChoice--;
			isKeyDown = true;
		}
	}
	else if(GetAsyncKeyState('S')||GetAsyncKeyState(VK_DOWN))
	{
		if(!isKeyDown)
		{
			menuChoice++;
			isKeyDown = true;
		}
	}
	else
		isKeyDown = false;

	if(menuChoice > 2) menuChoice = 1;
	if(menuChoice < 1) menuChoice = 2;
}

void CanisMajor::levelsUpdate(float dt)
{	
	gui.sprite = -1;
	//if(state.secondFloorSairsUsed)
		//return loadSplashScreen(true);

	for(int i = 0 ; i < CM::MAX_SCENERY; i++)
	{
		scenery[i].update(dt);
	}
	for(int i = 0 ; i < CM::NUM_QUEST_ITEMS; i++)
	{
		items[i].update(dt);
	}
	for(int i = 0 ; i < CM::MAX_DOORS; i++)
	{
		doors[i].update(dt);
	}
	for(int i = 0; i< CM::MAX_SEARCHABLE_ACTORS; i++)
	{
		searchableActors[i].update(dt);
	}
	for(int i = 0; i< CM::MAX_READABLE_ACTORS; i++)
	{
		readableActors[i].update(dt);
	}


	//for initial sound effect with howling door
	if(!howl && doors[0].getOpen()) {
		audio->playCue(BIRM);
		howl = true;
	}

	for(int i = 0 ; i < CM::MAX_STAIRCASES; i++)
		staircases[i].update(dt);

	slidingBookcase.update(dt);

	pedestal.update(dt);

	camera.update(dt);

	flashlight.update(dt);

	staff.update(dt);

	dog.update(dt);

	for(int i = 0 ; i < CM::NUM_BOLTS; i++)
		if(bolts[i].isActive)
			bolts[i].update(dt);

	updateStoryText(dt);
	updateNoteText(dt);

	collisions();

	static float victoryTimer = 0;
	const float VICTORY_TIME = 3;
	if(state.level==FIRST_FLOOR && camera.getPosition().z<0)
	{
		victoryTimer+=dt;
		gui.sprite=6;//large white
		gui.alpha = victoryTimer/VICTORY_TIME/4;
	}
	else
		victoryTimer=0;

	if(victoryTimer > VICTORY_TIME)
		loadSplashScreen(true);

	if(camera.getNearbyItem()!=nullptr)
	{
		drawUtilText(camera.getNearbyItem()->getUtilText());
	}

	//displays the player's current location. Use for mapping/debugging
	#if defined(DEBUG) || defined(DEBUG)
		wstring xzpos = std::to_wstring((int)camera.getPosition().x) + L", "+ std::to_wstring((int)camera.getPosition().z);
		drawUtilText(xzpos);
	#endif
}

//COLLISIONS GIVE LOADS OF FALSE POSITIVES
void CanisMajor::collisions()
{
	camera.resetNearbyInteractable();
	dog.resetNearest();
	float dist;

	for(int i = 0; i< CM::MAX_SEARCHABLE_ACTORS; i++)
	{
		if(camera.isPicked(&searchableActors[i],dist))
		{
			//sprites.sprite = 2;
			camera.setNearbyInteractable(&searchableActors[i],dist);
		}

		if(camera.collided(&searchableActors[i]))
		{
			camera.backUp();
		}
	}

	for(int i = 0; i< CM::MAX_READABLE_ACTORS; i++)
	{
		if(camera.isPicked(&readableActors[i],dist))
		{
			//sprites.sprite = 2;
			camera.setNearbyInteractable(&readableActors[i],dist);
		}

		if(camera.collided(&readableActors[i]))
		{
			camera.backUp();
		}
	}

	for(int i = 0; i<CM::MAX_STAIRCASES;i++)
	{
		if(camera.collided(&staircases[i]))
		{
			camera.backUp();
		}
		if(camera.isPicked(&staircases[i],dist))
		{
			camera.setNearbyInteractable(&staircases[i],dist);
		}
	}

	for(int i = 0 ; i < CM::MAX_DOORS; i++)
	{
		if(doors[i].isActive)
		{
			if(camera.isPicked(&doors[i],dist))
			{
				camera.setNearbyInteractable(&doors[i],dist);
			}

			if(!doors[i].getOpen() && camera.collided(&doors[i]))
			{
				camera.backUp();
			}
		}
	}

	if(camera.isPicked(&slidingBookcase,dist)){
		camera.setNearbyInteractable(&slidingBookcase,dist);
	}
	if(!slidingBookcase.getOpen() && camera.collided(&slidingBookcase))
	{
		camera.backUp();
	}

	for(int i = 0 ; i < CM::NUM_QUEST_ITEMS; i++)
	{
		if(camera.isPicked(&items[i],dist))
		{
			//sprites.sprite = 0;
			camera.setNearbyInteractable(&items[i],dist);
		}
	}

	if(!camera.hasFlashlight()&&camera.isPicked(&flashlight,dist))
	{
		camera.setNearbyInteractable(&flashlight,dist);
	}

	if(!camera.hasStaff()&&camera.isPicked(&staff,dist))
	{
		camera.setNearbyInteractable(&staff,dist);
	}

	for(int i = 0 ; i < CM::MAX_SCENERY;i++)
	{

	/*	if(camera.isPicked(&scenery[i],dist)){
			camera.setNearbyInteractable(nullptr,dist);
		}*/

		if(camera.collided(&scenery[i]))
		{
			camera.backUp();
		}
	}

	if(camera.collided(&pedestal))
		camera.backUp();
	if(camera.isPicked(&pedestal,dist))
		camera.setNearbyInteractable(&pedestal,dist);


	//Dog AI
	bool isPlayer = false;
	for(int i = 0; i<CM::MAX_SCENERY; i++)
	{
		if(dog.isPicked(&scenery[i], dist))
			dog.setNearest(&scenery[i],dist);
	}

	for(int i = 0; i<CM::MAX_DOORS; i++)
	{
		if(!doors[i].getOpen() && dog.isPicked(&doors[i], dist))
			dog.setNearest(&doors[i],dist);
	}

	isPlayer = true;
	if(dog.isPicked(&camera,dist))
		dog.setNearest(&camera,dist);


	//BOLTS
	for(int i = 0; i<CM::NUM_BOLTS;i++)
	{
		if(bolts[i].isActive)
		{
			for(int j = 0; j<CM::MAX_SCENERY;j++)
			{
				if(bolts[i].collided(&scenery[j]))
				{
					bolts[i].isActive = false;
					break;
				}
			}
		}
		if(bolts[i].isActive){
			for(int j = 0 ; j < CM::MAX_DOORS; j++)
			{
				if(!doors[j].getOpen() && bolts[i].collided(&doors[j]))
				{
					bolts[i].isActive = false;
					break;
				}
			}
		}
		if(bolts[i].isActive){
			if(bolts[i].collided(&dog)){
				dog.onDeath();
				bolts[i].isActive=false;
			}
		}
	}
}

void CanisMajor::drawScene()
{
	D3DApp::drawScene();

	// Restore default states, input layout and primitive topology 
	// because mFont->DrawText changes them.  Note that we can 
	// restore the default states by passing null.
	md3dDevice->OMSetDepthStencilState(0, 0);
	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	md3dDevice->OMSetBlendState(0, blendFactors, 0xffffffff);
	md3dDevice->IASetInputLayout(mVertexLayout);

	mfxEyePosVar->SetRawValue(&camera.getPosition(), 0, sizeof(D3DXVECTOR3));
	mfxEyesVar->SetRawValue(&eyes, 0, sizeof(Light));
	mfxLightVar->SetRawValue(&fLight, 0, sizeof(Light));
	mfxAmbientVar->SetRawValue(&ambient, 0, sizeof(Light));
	for(int i = 0; i < MAX_LIGHTS; i++) { //Individually setting lights.
		mfxPLightsVar[i]->SetRawValue(&rLights[i], 0, sizeof(Light));
	}
	for(int i = 0; i < MAX_LIGHTS; i++) { //Individually setting lights.
		mfxLightType[i]->SetInt(lightType[i]);
	}
	mfxActiveLights->SetInt(activeLights);

	//mfxPLightsVar->SetRawValue(&rLights, 0, sizeof(Light));
	mfxPLightVar->SetRawValue(&pLight, 0, sizeof(Light));
	mfxNegaLightVar->SetRawValue(&negaLight, 0, sizeof(Light));
	mfxLightBool->SetBool(flashlight.isOn);

	// set the point to the shader technique
	D3D10_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);

	if(!threadComplete)
	{
		RECT r; //its a point because DT_NOCLIP
		r.right = r.left = mClientWidth*0.5;
		r.top = r.bottom = mClientHeight*0.2;
		mFont->DrawText(0,L"LOADING",-1,&r,DT_NOCLIP|DT_CENTER,WHITE);
		r.top = r.bottom = mClientHeight*0.3;
		mFont->DrawText(0,std::to_wstring(loadingStatus).c_str(),-1,&r,DT_NOCLIP|DT_CENTER,RED);
	}
	else
	{
		switch(state.level){
		case MENU:
			menuDraw();
			break;
		case SPLASH:
			splashDraw();
			break;
		default:
			levelsDraw();
			break;
		}
	}
	mSwapChain->Present(1, 0);
}

void CanisMajor::splashDraw()
{
	for(int i = 0; i < CM::NUM_SPLASH_MENU_ITEMS; i++)
	{
		RECT r; //its a point because DT_NOCLIP
		if(i==0)
		{
			r.right = r.left = mClientWidth*0.5;
			r.top = r.bottom = mClientHeight*0.2;

		}
		else
		{
			r.right = r.left = mClientWidth*0.5;
			r.top = r.bottom = mClientHeight*0.4 + (i-1)*mClientHeight*0.07;
		}
		if(menuChoice == i)
			mFont->DrawText(0,menuText[i].c_str(),-1,&r,DT_NOCLIP|DT_CENTER,RED);
		else
			mFont->DrawText(0,menuText[i].c_str(),-1,&r,DT_NOCLIP|DT_CENTER,WHITE);
	}
}

void CanisMajor::menuDraw()
{
	for(int i = 0; i < CM::NUM_MENU_ITEMS; i++)
	{
		RECT r; //its a point because DT_NOCLIP
		if(i==0)
		{
			r.right = r.left = mClientWidth*0.5;
			r.top = r.bottom = mClientHeight*0.2;

		}
		else
		{
			r.right = r.left = mClientWidth*0.5;
			r.top = r.bottom = mClientHeight*0.4 + (i-1)*mClientHeight*0.07;
		}
		if(menuChoice == i)
			mFont->DrawText(0,menuText[i].c_str(),-1,&r,DT_NOCLIP|DT_CENTER,RED);
		else
			mFont->DrawText(0,menuText[i].c_str(),-1,&r,DT_NOCLIP|DT_CENTER,WHITE);
	}
}

void CanisMajor::levelsDraw()
{
	//Get Camera viewMatrix
	mView = camera.getViewMatrix();
	mProj = camera.getProjectionMatrix();
	
	for(int i = 0; i < CM::NUM_SPRITES; i++)
		sprites[i].draw(camera.getPosition(),mView*mProj);

	trees.draw(camera.getPosition(), mView*mProj);

	//Billboards screw with these settings, gotta reset them.
	md3dDevice->OMSetDepthStencilState(0, 0);
	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	md3dDevice->OMSetBlendState(0, blendFactors, 0xffffffff);
	md3dDevice->IASetInputLayout(mVertexLayout);

	sky.draw(mView, mProj);

	dog.draw(mfxWVPVar,mView,mProj,mTech);

	for(int i = 0 ; i < CM::MAX_SCENERY; i++)
	{
		scenery[i].draw(mfxWVPVar,mView,mProj,mTech);
	}
	for(int i = 0 ; i < CM::NUM_QUEST_ITEMS; i++)
	{
		items[i].draw(mfxWVPVar,mView,mProj,mTech);
	}
	for(int i = 0 ; i < CM::MAX_DOORS; i++)
	{
		doors[i].draw(mfxWVPVar,mView,mProj,mTech);
	}
	for(int i = 0 ; i < CM::MAX_SEARCHABLE_ACTORS; i++)
	{
		searchableActors[i].draw(mfxWVPVar,mView,mProj,mTech);
	}
	for(int i = 0 ; i < CM::MAX_READABLE_ACTORS; i++)
	{
		readableActors[i].draw(mfxWVPVar,mView,mProj,mTech);
	}
	for(int i = 0 ; i < CM::MAX_STAIRCASES; i++)
		staircases[i].draw(mfxWVPVar,mView,mProj,mTech);
	for(int i = 0 ; i < CM::NUM_BOLTS; i++)
		bolts[i].draw(mfxWVPVar,mView,mProj,mTech);
	flashlight.draw(mfxWVPVar,mView,mProj,mTech);
	staff.draw(mfxWVPVar,mView,mProj,mTech);
	if(state.level==SECOND_FLOOR)
		pedestal.draw(mfxWVPVar,mView,mProj,mTech);

	slidingBookcase.draw(mfxWVPVar,mView,mProj,mTech);

#if defined(DEBUG) || defined(DEBUG)
	AABBHelper.draw(mfxWVPVar,mView,mProj,mTech);
#endif

	drawUtilText();
	drawStoryText();
	drawNoteText();

	if(gui.sprite >= 0)
		gui.draw(mProj);
	else {
		RECT r = {mClientWidth/2,mClientHeight/2,mClientWidth/2,mClientHeight/2};
		utilFont->DrawText(0,L"\u25CF",-1,&r,DT_NOCLIP|DT_CENTER|DT_VCENTER,WHITE);
	}

	
	//#ifdefDEBUG || DEBUG
	//RECT r = {mClientWidth/2,mClientHeight/2,mClientWidth/2,mClientHeight/2};
	//utilFont->DrawText(0,L"\u25CF",-1,&r,DT_NOCLIP|DT_CENTER|DT_VCENTER,WHITE);
	//#endif
}

void CanisMajor::buildFX()
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	hr = D3DX10CreateEffectFromFile(L"lighting.fx", 0, 0, 
		"fx_4_0", shaderFlags, 0, md3dDevice, 0, 0, &mFX, &compilationErrors, 0);
	if(FAILED(hr))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			ReleaseCOM(compilationErrors);
		}
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX10CreateEffectFromFile", true);
	} 

	hr = D3DX10CreateEffectFromFile(L"sky.fx", 0, 0, 
		"fx_4_0", shaderFlags, 0, md3dDevice, 0, 0, &skyFX, &compilationErrors, 0);
	if(FAILED(hr))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			ReleaseCOM(compilationErrors);
		}
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX10CreateEffectFromFile", true);
	} 

	mTech = mFX->GetTechniqueByName("LightTech");

	mfxWVPVar    = mFX->GetVariableByName("gWVP")->AsMatrix();
	mfxWorldVar  = mFX->GetVariableByName("gWorld")->AsMatrix();
	mfxEyePosVar = mFX->GetVariableByName("gEyePosW");
	mfxLightVar  = mFX->GetVariableByName("gLight");

	for(int i = 0; i < MAX_LIGHTS; i++) {
		mfxPLightsVar[i] = mFX->GetVariableByName("lights")->GetElement(i);
	}
	for(int i = 0; i < MAX_LIGHTS; i++) {
		mfxLightType[i] = mFX->GetVariableByName("type")->GetElement(i)->AsScalar();
	}

	mfxActiveLights = mFX->GetVariableByName("activeLights")->AsScalar();
	mfxPLightVar = mFX->GetVariableByName("pLight");
	mfxNegaLightVar = mFX->GetVariableByName("negaLight");
	mfxEyesVar = mFX->GetVariableByName("eyes");
	mfxAmbientVar = mFX->GetVariableByName("ambient");
	mfxLightBool = mFX->GetVariableByName("gLightType")->AsScalar();
	mfxHighlightBool = mFX->GetVariableByName("highlight")->AsScalar();

	//TEXTURES
	mfxDiffuseMapVar = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxSpecMapVar    = mFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxTexMtxVar     = mFX->GetVariableByName("gTexMtx")->AsMatrix();
}

void CanisMajor::buildVertexLayouts()
{
	// Create the vertex input layout.
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24,
		D3D10_INPUT_PER_VERTEX_DATA, 0},
	};


	// Create the input layout
	D3D10_PASS_DESC PassDesc;
	mTech->GetPassByIndex(0)->GetDesc(&PassDesc);
	HR(md3dDevice->CreateInputLayout(vertexDesc, 3, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &mVertexLayout));
}

void CanisMajor::clearLevel()
{
	camera.clearInventory();
	for(int i=0;i<CM::NUM_SPRITES;i++)
		sprites[i].isInit=false;
	for(int i = 0 ; i < CM::MAX_SCENERY; i++)
	{
		scenery[i].isActive=false;
	}
	for(int i = 0 ; i < CM::NUM_QUEST_ITEMS; i++)
	{
		items[i].isActive=false;
		items[i].reset();
	}
	for(int i = 0 ; i < CM::MAX_DOORS; i++)
	{
		doors[i].isActive=false;
		doors[i].reset();
	}
	for(int i = 0 ; i < CM::MAX_SEARCHABLE_ACTORS; i++)
	{
		searchableActors[i].isActive = false;
		searchableActors[i].reset();
	}
	for(int i = 0 ; i < CM::MAX_READABLE_ACTORS; i++)
	{
		readableActors[i].isActive = false;
		readableActors[i].reset();
	}
	for(int i = 0 ; i < CM::MAX_STAIRCASES; i++)
	{
		staircases[i].isActive=false;
		staircases[i].reset();
	}
	for(int i = 0 ; i < CM::NUM_BOLTS; i++)
	{
		bolts[i].isActive=false;
	}
	for(int i = 0; i < MAX_LIGHTS; i++) {
		rLights[i].init();
		lightType[i] = 0;
	}
	activeLights = 0;
	pedestal.isActive = false;
	pedestal.reset();
	flashlight.isActive = false;
	staff.isActive=false;
	dog.isActive = false;//disable dog
	negaLight.pos = Vector3(200,200,200);
	eyes.pos = Vector3(200,200,200);
	slidingBookcase.isActive=false;
}

void CanisMajor::loadSplashScreen(bool status)
{
	state.level = SPLASH;
	clearLevel();
	menuUpdate(0,true);

	if(status)
		menuText[0] = L"VICTORY";
	else
		menuText[0] = L"DEFEAT";
	menuText[1] = L"CONTINUE";
	menuText[2] = L"QUIT";
}

void CanisMajor::menuLoad()
{
	audio->stopCue(BG);
	state.reset();//sets menu state
	camera.reset();
	clearLevel();
	menuUpdate(0,true);

	birminghamMode = false;

	menuText[0] = L"CANIS MAJOR";
	menuText[1] = L"PLAY";
	menuText[2] = L"QUIT";
	menuText[3] = L"";
	menuText[4] = L"WASD-move     SHIFT-run     CTRL-crouch";
	menuText[5] = L"E-use     F-flashlight on/off     R-recharge";
}

void CanisMajor::loadBasement()
{
	state.level = BASEMENT;
}

void CanisMajor::onPlayerDeath()
{
	if(!birminghamMode)
		loadSplashScreen(false);
}

//calling with s defined sets, calling without clears
void CanisMajor::drawUtilText(wstring s)
{
	static wstring text = L"";
	//set new string
	if(s!=L"")
	{
		text = s;
	}
	else
	{
		RECT r;
		r.top = mClientHeight-30;
		r.bottom = mClientHeight-30;
		r.left = mClientWidth*0.5;
		r.right = mClientWidth*0.5;
		mFont->DrawText(0,text.c_str(),-1,&r,DT_NOCLIP|DT_CENTER|DT_VCENTER,WHITE);
		text = L"";
	}
}
void CanisMajor::drawStoryText()
{
	if(storyTextAge<storyTextLifespan)
	{
		float alpha = 1;
		if(storyTextLifespan-storyTextAge < CM::TEXT_FADEOUT_TIME)
		{
			alpha = (storyTextLifespan-storyTextAge) / CM::TEXT_FADEOUT_TIME;
		}
		storyTextColor.a = alpha;
		RECT r = {10,25,0,0};
		mFont->DrawText(0,storyText.c_str(),-1,&r,DT_NOCLIP,storyTextColor);
	}
}

void CanisMajor::updateStoryText(float dt)
{
	if(storyTextAge<storyTextLifespan)
	{
		storyTextAge+=dt;
	}
}
void CanisMajor::setStoryText(float duration,wstring s, D3DXCOLOR c)
{
	storyText = s;
	storyTextLifespan = duration;
	storyTextAge = 0;
	storyTextColor = c;
}

void CanisMajor::drawNoteText()
{
	if(noteTextAge<noteTextLifespan)
	{
		float alpha = 1;
		if(noteTextLifespan-noteTextAge < CM::TEXT_FADEOUT_TIME)
		{
			alpha = (noteTextLifespan-noteTextAge) / CM::TEXT_FADEOUT_TIME;
		}
		noteTextColor.a = alpha;
		RECT r;
		r.left = r.right = mClientWidth/2;
		r.top = r.bottom = mClientHeight/2 + mClientHeight/8;
		nFont->DrawText(0,noteText.c_str(),-1,&r,DT_NOCLIP|DT_CENTER|DT_VCENTER,noteTextColor);
	}
}
void CanisMajor::updateNoteText(float dt)
{
	if(noteTextAge<noteTextLifespan && !reading)
	{
		noteTextAge+=dt;
	}
}
void CanisMajor::setNoteText(float duration,wstring s, D3DXCOLOR c)
{
	noteText = s;
	noteTextLifespan = duration;
	noteTextAge = 0;
	noteTextColor = c;
}


Actor* CanisMajor::spawnScenery(Geometry* g, Vector3 pos, Vector3 rot, Vector3 scale)
{
	for(int i = 0 ; i < CM::MAX_SCENERY; i++)
	{
		if(!scenery[i].isActive)
		{
			scenery[i].isActive = true;
			scenery[i].setGeometry(g);
			scenery[i].setPosition(pos);
			scenery[i].setRotation(rot);
			scenery[i].setScale(scale);
			return &scenery[i];
		}
	}
	return nullptr;
}

QuestItem* CanisMajor::spawnQuestItem(Geometry* g, wstring name, Vector3 pos, Vector3 rot, Vector3 scale)
{
	for(int i = 0 ; i < CM::NUM_QUEST_ITEMS; i++)
	{
		if(!items[i].isActive)
		{
			items[i].create(pos);
			items[i].name = name;
			items[i].setRotation(rot);
			items[i].setScale(scale);
			items[i].setGeometry(g);
			return &items[i];
		}
	}
	return nullptr;
}

Door* CanisMajor::spawnDoor(Vector3 pos, Vector3 rot,Vector3 scale, QuestItem* k, bool open)
{
	pos.y+=6;
	for(int i = 0 ; i < CM::MAX_DOORS; i++)
	{
		if(!doors[i].isActive)
		{
			doors[i].create(pos,rot,scale,k,open);
			return &doors[i];
		}
	}
	return nullptr;
}


SearchableActor* CanisMajor::spawnSearchable(Geometry* g, std::wstring name, Actor* in, Vector3 pos, Vector3 rot, Vector3 scale, const char* cue, bool playOnce)
{
	for(int i = 0 ; i < CM::MAX_SEARCHABLE_ACTORS; i++)
	{
		if(!searchableActors[i].isActive)
		{
			searchableActors[i].create(pos,rot,scale,in);
			searchableActors[i].setGeometry(g);
			searchableActors[i].name = name;
			searchableActors[i].setCue(cue,playOnce);
			return &searchableActors[i];
		}
	}
	return nullptr;
}

ReadableActor* CanisMajor::spawnReadable(Geometry* g, std::wstring name, Actor* in, Vector3 pos, Vector3 rot, Vector3 scale,  wstring text, float dur, const char* cue, bool playOnce)
{
	for(int i = 0 ; i < CM::MAX_READABLE_ACTORS; i++)
	{
		if(!readableActors[i].isActive)
		{
			readableActors[i].create(pos,rot,scale,in,dur);
			readableActors[i].setGeometry(g);
			readableActors[i].setText(text);
			readableActors[i].name = name;
			readableActors[i].setCue(cue,playOnce);
			return &readableActors[i];
		}
	}
	return nullptr;
}


Light* CanisMajor::spawnLight(Vector3 pos, int type) {

	if(activeLights < MAX_LIGHTS) {
		lightType[activeLights] = rLights[activeLights].init(type);
		rLights[activeLights].pos = pos;
		activeLights++;
		return &rLights[activeLights-1];
	}

	return nullptr;
}

Light* CanisMajor::spawnLight(Vector3 pos, Vector3 dir, int type) { 
	if(activeLights < MAX_LIGHTS) {
		lightType[activeLights] = rLights[activeLights].init(type);
		rLights[activeLights].pos = pos;
		rLights[activeLights].dir = dir;
		activeLights++;
		return &rLights[activeLights-1];
	}

	return nullptr;
}
Staircase* CanisMajor::spawnStaircase(std::wstring name, LLevel func, Vector3 pos, Vector3 rotation, Vector3 scale)
{
	for(int i = 0 ; i < CM::MAX_STAIRCASES; i++)
	{
		if(!staircases[i].isActive)
		{
			staircases[i].create(pos,rotation,scale);
			staircases[i].setLLevel(func,name);
			return &staircases[i];
		}
	}
	return nullptr;
}

Actor* CanisMajor::spawnBolt(Vector3 pos, Vector3 vel)
{
	for(int i = 0 ; i < CM::NUM_BOLTS; i++)
	{
		if(!bolts[i].isActive)
		{
			bolts[i].create(pos);
			bolts[i].setVelocity(vel);
			return &bolts[i];
		}
	}
	return nullptr;
}

Sprite* CanisMajor::spawnSprites(const D3DXVECTOR3 centers[], UINT numSprites, std::wstring filenames[], UINT numFiles)
{

	for(int i = 0 ; i < CM::NUM_SPRITES; i++)
	{
		if(!sprites[i].isInit)
		{
			sprites[i].init(md3dDevice,centers,numSprites,filenames,numFiles);
			return &sprites[i];
		}
	}
	return nullptr;
}

Sprite* CanisMajor::spawnSprite(const D3DXVECTOR3 center, std::wstring filename)
{
	Vector3 centers[1];
	centers[0] = center;
	wstring texts[1];
	texts[0]=filename;
	return spawnSprites(centers,1,texts,1);
}


#if defined(DEBUG) || defined(DEBUG)
void CanisMajor::updateDebugAABB(Actor* a)
{
	Vector3 min = a->getGeometry()->getAABBMin(), max=a->getGeometry()->getAABBMax();
	min.x *= a->getScale().x;
	min.y *= a->getScale().y;
	min.z *= a->getScale().z;

	max.x *= a->getScale().x;
	max.y *= a->getScale().y;
	max.z *= a->getScale().z;

	min=rotate(min,a->getRotation());
	max=rotate(max,a->getRotation());

	min += a->getPosition();
	max += a->getPosition();

	Vector3 pos = (max+min)/2;
	Vector3 scale = (max-min)/2;

	AABBHelper.setPosition(pos);
	AABBHelper.setScale(scale);
	AABBHelper.update(0);

}
#endif

void CanisMajor::playSound(const char* cue, Vector3 pos)
{
	audio->playCue(cue);
}
