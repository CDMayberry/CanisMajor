#include"CanisMajor.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
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
	controls.flashlight = 'F';
	controls.crouch = VK_CONTROL;
	controls.run = VK_SHIFT;
	controls.recharge = 'R';

	//Camera Object
	camera.init(this,&mCube,controls);

	scenery = new Actor[CM::MAX_SCENERY];
	searchableActors = new SearchableActor[CM::MAX_SEARCHABLE_ACTORS];
	readableActors = new ReadableActor[CM::MAX_READABLE_ACTORS];
	numwaypoints = 0;
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
	};

	gui.init(md3dDevice, centers, MAX_GUI, guiNames);

	D3DXVECTOR3 centers[SpriteNS::SPRITES];
	for(int i = 0; i < SpriteNS::SPRITES; i++) {
		Vector3 center(0,0,100);
		centers[i] = center;
	}

	//Make sure to have the exact number of file names
	std::wstring spriteNames[SpriteNS::SPRITES] = 
	{ //Put all file names in here.
		/* L".\\textures\\.dds", */
		L".\\textures\\hand.dds",
		L".\\textures\\book.dds",
	};
	int billboards = 16;
	sprite.init(md3dDevice, centers, billboards, spriteNames);

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

	numwaypoints = 0;

	mTelescope.init(md3dDevice,".\\geometry\\telescope.geo");
	loadingStatus++; //1
	mDresser.init(md3dDevice,".\\geometry\\dresser.geo", L".\\textures\\lightwood.dds");
	loadingStatus++; //2
	mFlashlight.init(md3dDevice,".\\geometry\\flashlight.geo", L".\\textures\\grey.dds");
	flashlight.init(this,&mFlashlight,&fLight);
	loadingStatus++; //3
	mFrame.init(md3dDevice,".\\geometry\\pictureframe.geo");
	loadingStatus++; //4
	mBookcase.init(md3dDevice,".\\geometry\\bookcase.geo", L".\\textures\\medwood.dds");
	loadingStatus++; //5
	mChair.init(md3dDevice,".\\geometry\\chair.geo", L".\\textures\\lightwood.dds");
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
	mTable.init(md3dDevice,".\\geometry\\table.geo", L".\\textures\\lightwood.dds");
	loadingStatus++; //11
	mBottle.init(md3dDevice,".\\geometry\\bottle.geo", L".\\textures\\bottlegreen.dds");
	loadingStatus++; //12
	mLock.init(md3dDevice,".\\geometry\\lock.geo");
	loadingStatus++; //13
	mPictureframe.init(md3dDevice,".\\geometry\\pictureframe.geo");
	loadingStatus++; //14
	mRail.init(md3dDevice,".\\geometry\\rail.geo");
	loadingStatus++; //15
	mWallpanel.init(md3dDevice,".\\geometry\\wallpanel.geo", L".\\textures\\greywood.dds");
	loadingStatus++; //16
	mCage.init(md3dDevice,".\\geometry\\cage.geo");
	loadingStatus++; //17
	mFixture.init(md3dDevice,".\\geometry\\fixture.geo", L".\\textures\\cardboard.dds");
	loadingStatus++; //18
	mDoor.init(md3dDevice,".\\geometry\\door.geo", L".\\textures\\gold.dds");
	loadingStatus++; //19
	mBox.init(md3dDevice,".\\geometry\\cardboardBox.geo", L".\\textures\\cardboard.dds");
	loadingStatus++; //20
	mBook.init(md3dDevice,".\\geometry\\book.geo",L".\\textures\\paper.dds");
	loadingStatus++; //21
#ifndef DEBUG
	mToilet.init(md3dDevice,".\\geometry\\toilet.geo");
#else
	mToilet.init(md3dDevice,".\\geometry\\cardboardBox.geo", L".\\textures\\paper.dds");
#endif
	loadingStatus++; //22
#ifndef DEBUG
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
	dog.init(this,&mDog,1.0f,Vector3(1,2,2));
	//dog.setScale(Vector3(0.1f,5.0f,5.0f));
	dog.setNegalight(&negaLight);
	dog.setEyes(&eyes);
	loadingStatus++; //24
	mRoofHole.init(md3dDevice,".\\geometry\\newRoofHole.geo", L".\\textures\\greywood.dds");
	loadingStatus++; //25
	mKey.init(md3dDevice,".\\geometry\\key2.geo", L".\\textures\\key1D.dds", true);
	loadingStatus++; //26
	mWindowPanel.init(md3dDevice,".\\geometry\\windowpanel.geo", L".\\textures\\greywood.dds");
	loadingStatus++; //27
	mBookStack.init(md3dDevice,".\\geometry\\bookStack.geo");
	loadingStatus++; //28
	mDesk.init(md3dDevice,".\\geometry\\desk.geo");
	loadingStatus++; //29
#ifndef DEBUG
	mSink.init(md3dDevice,".\\geometry\\sink.geo");
#else
	mSink.init(md3dDevice,".\\geometry\\cardboardBox.geo", L".\\textures\\paper.dds");
#endif
	loadingStatus++; //30
#ifndef DEBUG
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
	pedestal.init(this,&mPedastal);
	pedestal.collisionType = AABBox;

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
	camera.create(Vector3(10,10,10),Vector3(1,0,0));
	camera.setPerspective();

	flashlight.toggle();

#ifdef DEBUG
	mRedCube.init(md3dDevice,".\\geometry\\cube.geo", L".\\textures\\metal.dds", true);
	AABBHelper.init(this,&mRedCube,1);
	AABBHelper.isActive = true;
#endif



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

	if(GetAsyncKeyState(VK_RETURN)||GetAsyncKeyState(' '))
	{ 
		if(!isKeyDown)
		{
			switch(menuChoice)
			{
			case 1://play
				if(state.level==SPLASH)
					menuLoad();
				else
					loadAttic();
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
	if(state.secondFloorSairsUsed)
		return loadSplashScreen(true);

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
		audio->playCue(HOWL, doors[0].getPosition());
		howl = true;
	}

	for(int i = 0 ; i < CM::MAX_STAIRCASES; i++)
		staircases[i].update(dt);

	pedestal.update(dt);

	camera.update(dt);

	flashlight.update(dt);

	dog.update(dt);

	updateStoryText(dt);
	updateNoteText(dt);

	collisions();

	if(camera.getNearbyItem()!=nullptr)
	{
		drawUtilText(camera.getNearbyItem()->getUtilText());
	}

	//displays the player's current location. Use for mapping/debugging
	#ifdef DEBUG
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

	for(int i = 0 ; i < CM::MAX_SCENERY;i++)
	{
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
	flashlight.draw(mfxWVPVar,mView,mProj,mTech);
	if(state.level==SECOND_FLOOR)
		pedestal.draw(mfxWVPVar,mView,mProj,mTech);

#ifdef DEBUG
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
	//#ifdef DEBUG
	//RECT r = {mClientWidth/2,mClientHeight/2,mClientWidth/2,mClientHeight/2};
	//utilFont->DrawText(0,L"\u25CF",-1,&r,DT_NOCLIP|DT_CENTER|DT_VCENTER,WHITE);
	//#endif



}

void CanisMajor::buildFX()
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
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
	for(int i = 0; i < MAX_LIGHTS; i++) {
		rLights[i].init();
		lightType[i] = 0;
	}
	activeLights = 0;
	pedestal.isActive = false;
	pedestal.reset();
	flashlight.isActive = false;

	if (numwaypoints !=0)//clear waypoints
		delete [] dogWaypoints;
	numwaypoints = 0;
	dog.SetWaypoints(dogWaypoints,numwaypoints,1);//update dog's waypoint count
	dog.isActive = false;//disable dog
	negaLight.pos = Vector3(200,200,200);
	eyes.pos = Vector3(200,200,200);
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

	menuText[0] = L"CANIS MAJOR";
	menuText[1] = L"PLAY";
	menuText[2] = L"QUIT";
	menuText[3] = L"";
	menuText[4] = L"WASD-move     SHIFT-run     CTRL-crouch";
	menuText[5] = L"E-use     F-flashlight on/off     R-recharge";
}

void CanisMajor::loadAttic()
{
	state.level = ATTIC;
	clearLevel();
	int iter = 0;
	if(!state.gameStarted)
	{
		camera.setPosition(Vector3(5,0,5));
		camera.setDirection(Vector3(1,0,0));
		setStoryText(10,L"Where am I?");
		audio->playCue(BG,camera.getPosition());
		state.gameStarted = true;	
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
	spawnReadable(&mBook, L"Note", nullptr, Vector3(10,.3,36),Vector3(0,0,0),Vector3(.5,.5,.5), L"We couldn't get out...\nSomeone help us");

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

void CanisMajor::loadSecondFloor()
{
	clearLevel();
	setStoryText(10,L"this is a big house...");

	QuestItem* patKey = nullptr;
	if(!state.balconyKeyTaken)
	{
		patKey=spawnQuestItem(&mKey,L"BALCONY KEY",Vector3(7,1,15));
		patKey->setStateSwitch(&state,&GameState::balconyKeyTaken);
	}
	QuestItem* mainKey = nullptr;
	if(!state.secondFloorMainKeyTaken)
	{
		mainKey = spawnQuestItem(&mKey,L"KEY",Vector3(81,1,20));
		mainKey->setStateSwitch(&state,&GameState::secondFloorMainKeyTaken);
	}

	QuestItem *r1 = spawnQuestItem(&mRing,L"Large Ring",Vector3(18,0,17),Vector3(0,0,0),Vector3(1.2,1,1.2));
	QuestItem *r2 = spawnQuestItem(&mRing,L"Medium Ring",Vector3(25,1.5,32),Vector3(0,0,0),Vector3(1,1,1));
	QuestItem *r3 = spawnQuestItem(&mRing,L"Small Ring",Vector3(2,2,47),Vector3(0,0,0),Vector3(.8,1,.8));
	QuestItem *a = spawnQuestItem(&mArrow,L"Arrow",Vector3(40,1.5,14),Vector3(0,0,0),Vector3(1,1,1));
	pedestal.create(mainKey,a,r1,r2,r3,Vector3(82,-.5,20),Vector3(0,0,0),Vector3(1,1,1));
	pedestal.load(&state);

	state.level = SECOND_FLOOR;
	camera.setPosition(Vector3(37,3,65));

	flashlight.setPosition(Vector3(10,-2.5,10));
	flashlight.isActive = true;

	dog.isActive = true;
	dog.setPosition(Vector3(35,0,2));

	if (numwaypoints !=0)
		delete [] dogWaypoints;
	numwaypoints = 7;
	dogWaypoints = new Vector3[numwaypoints];
	dogWaypoints[0] = Vector3(10,-2.5,30);
	dogWaypoints[1] = Vector3(24,-2.5,40);
	dogWaypoints[2] = Vector3(25,-2.5,53);
	dogWaypoints[3] = Vector3(65, -2.5,53);
	dogWaypoints[4] = Vector3(65,-2.5,5);
	dogWaypoints[5] = Vector3(25,-2.5,5);
	dogWaypoints[6] = Vector3(25,-2.5,20);
	dog.SetWaypoints(dogWaypoints,numwaypoints,3);
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
		spawnScenery(&mRail,Vector3(71.5+(i*2),-3,45),Vector3(0,PI/2,0),Vector3(2,2,1.6));
	for(int i= 0; i<15;i++)
		spawnScenery(&mRail,Vector3(89.5,-3,43-(i*2)),Vector3(0,PI,0),Vector3(2,2,1.6));
	for(int i= 0; i<10;i++)
		spawnScenery(&mRail,Vector3(71.5+(i*2),-3,15),Vector3(0,PI/2,0),Vector3(2,2,1.6));

	//Telescope. It should contain something for a puzzle
	spawnReadable(&mTelescope,L"Telescope",nullptr,Vector3(85,-2.7,40),Vector3(0,-2*PI/3,0),Vector3(7,7,7), L"Canis Major can be seen rising just over the horizon");

	//Left outer wall
	spawnScenery(&mWallpanel,Vector3(0,3,10),Vector3(0,0,0), Vector3(1,1.2,2));
	spawnScenery(&mWindowPanel,Vector3(0,3,25),Vector3(PI,0,0),CM::WALL_SCALE2);
	spawnScenery(&mWindowPanel,Vector3(0,3,35),Vector3(PI,0,0),CM::WALL_SCALE2);
	spawnScenery(&mWallpanel,Vector3(0,3,50),Vector3(0,0,0), Vector3(1,1.2,2));

	//Far wall with door to attic
	spawnScenery(&mWallpanel,Vector3(10,3,60),Vector3(0,1.5707963268,0), Vector3(1,1.2,2));
	spawnScenery(&mWindowPanel,Vector3(25,3,60),Vector3(0,1.5707963268,0), CM::WALL_SCALE2);
	spawnScenery(&mWallpanel,Vector3(32.5,3,60),Vector3(0,1.5707963268,0), Vector3(1,1.2,.5));
	spawnDoor(Vector3(35,-2.6,60),Vector3(0,-PI/2,0),Vector3(2,5,2));
	spawnScenery(&mWallpanel,Vector3(41.5,3,60),Vector3(0,1.5707963268,0), Vector3(1,1.2,.5));
	spawnScenery(&mWindowPanel,Vector3(49,3,60),Vector3(0,1.5707963268,0), CM::WALL_SCALE2);
	spawnScenery(&mWallpanel,Vector3(62,3,60),Vector3(0,1.5707963268,0), Vector3(1,1.2,1.6));

	//Right outer wall
	spawnScenery(&mWallpanel,Vector3(70,3,45),Vector3(0,0,0), Vector3(1,1.2,3));
	spawnDoor(Vector3(70,-3.5,30),Vector3(0,0,0),Vector3(2,4,3),patKey);
	spawnScenery(&mWallpanel,Vector3(70,3,12),Vector3(0,0,0), Vector3(1,1.2,2.5));

	//Close outer wall
	spawnScenery(&mWallpanel,Vector3(10,3,0),Vector3(0,1.5707963268,0), Vector3(1,1.2,2));
	spawnScenery(&mWindowPanel,Vector3(25,3,0),Vector3(0,1.5707963268,0), CM::WALL_SCALE2);
	spawnScenery(&mWallpanel,Vector3(62.5,3,0),Vector3(0,1.5707963268,0), Vector3(1,1.2,1.5));
	spawnScenery(&mWindowPanel,Vector3(50,3,0),Vector3(0,1.5707963268,0), CM::WALL_SCALE2);
	spawnScenery(&mWallpanel,Vector3(37.5,3,0),Vector3(0,1.5707963268,0), Vector3(1,1.2,1.5));

	//Library walls
	spawnScenery(&mWallpanel,Vector3(5,3,20),Vector3(0,1.5707963268,0), CM::WALL_SCALE2);
	spawnDoor(Vector3(13.9,-3,20),Vector3(0,-3*PI/2,0),Vector3(2,5,2));
	spawnScenery(&mWallpanel,Vector3(17,3,20),Vector3(0,1.5707963268,0), Vector3(1,1.2,.65));
	spawnScenery(&mWallpanel,Vector3(20,3,2.5),Vector3(0,0,0), Vector3(1,1.2,.5));
	spawnDoor(Vector3(20,-3,5),Vector3(0,PI,0),Vector3(2,5,2));
	spawnScenery(&mWallpanel,Vector3(20,3,14.4),Vector3(0,0,0), Vector3(1,1.2,1.1));
	spawnScenery(&mFixture,Vector3(1,5,10),Vector3(0,PI/2,0));
	spawnLight(Vector3(1.3,5.3,10));


	//Office walls
	spawnScenery(&mWallpanel,Vector3(5,3,40),Vector3(0,1.5707963268,0), CM::WALL_SCALE2);
	spawnDoor(Vector3(10,-3,40),Vector3(0,-PI/2,0),Vector3(2,5,2));
	spawnScenery(&mWallpanel,Vector3(17,3,40),Vector3(0,1.5707963268,0), Vector3(1,1.2,.65));
	spawnScenery(&mWallpanel,Vector3(20,3,57.5),Vector3(0,0,0), Vector3(1,1.2,.5));
	spawnDoor(Vector3(20,-3,51),Vector3(0,PI,0),Vector3(2,5,2));
	spawnScenery(&mWallpanel,Vector3(20,3,45.6),Vector3(0,0,0), Vector3(1,1.2,1.1));

	//MasterBed walls
	spawnScenery(&mWallpanel,Vector3(44.5,3,45.5),Vector3(0,1.5707963268,0), Vector3(1,1.2,3.1));
	spawnScenery(&mWallpanel,Vector3(60,3,37),Vector3(0,0,0), Vector3(1,1.2,1.75));
	spawnDoor(Vector3(60,-3.5,30),Vector3(0,0,0),Vector3(2,4,3));
	spawnScenery(&mWallpanel,Vector3(60,3,16.6),Vector3(0,0,0), Vector3(1,1.2,1.5));
	spawnDoor(Vector3(55.9,-3.5,9.2),Vector3(0,-PI/2,0),Vector3(2,4,2));
	spawnScenery(&mWallpanel,Vector3(46,3,9),Vector3(0,PI/2,0), Vector3(1,1.2,2));
	spawnScenery(&mWallpanel,Vector3(36,3,18),Vector3(0,0,0), Vector3(1,1.2,1.75));
	spawnScenery(&mWallpanel,Vector3(29,3,25.5),Vector3(0,PI/2,0), Vector3(1,1.2,1.4));
	spawnScenery(&mWallpanel,Vector3(22,3,29.5),Vector3(0,0,0), Vector3(1,1.2,.8));
	spawnScenery(&mWallpanel,Vector3(25.5,3,33.5),Vector3(0,PI/2,0), Vector3(1,1.2,.7));
	spawnScenery(&mWallpanel,Vector3(29,3,39.5),Vector3(0,0,0), Vector3(1,1.2,1.2));
	spawnDoor(Vector3(36,-3.5,26.7),Vector3(0,PI,0),Vector3(2,4,2));
	spawnScenery(&mWallpanel,Vector3(36,3,37.95),Vector3(0,0,0), Vector3(1,1.2,1.45));

	//Staircase to attic
	spawnScenery(&mCube,Vector3(37,-4,65),Vector3(0,0,0),Vector3(4,1,5));
	spawnScenery(&mWallpanel,Vector3(33,5,70),Vector3(0,0,0),Vector3(4,3,2));
	spawnScenery(&mWallpanel,Vector3(42,5,70),Vector3(0,0,0),Vector3(4,3,2));
	spawnScenery(&mWallpanel,Vector3(42,5,70),Vector3(0,0,0),Vector3(4,3,2));
	//spawnStaircase(L"upstairs",&CanisMajor::loadAttic,Vector3(37,3, 76), Vector3(0,PI/2,0), Vector3(1,1,.9));
	spawnScenery(&mStaircase,Vector3(37,3, 76), Vector3(0,PI/2,0), Vector3(1,1,.9));
	spawnScenery(&mStaircase,Vector3(37,11, 79), Vector3(0,PI/2,0), Vector3(1,4,.9));
	spawnScenery(&mWallpanel,Vector3(37,10, 70), Vector3(0,0,PI/2), Vector3(1,1,3));

	//Staircase to main floor
	spawnScenery(&mWallpanel, Vector3(28,-4,15),Vector3(0,0,0),Vector3(1,4,1.2));
	spawnDoor(Vector3(28,-3,25.3),Vector3(0,0,0),Vector3(2,4,2.2),mainKey);
	spawnScenery(&mWallpanel, Vector3(32,-4,9),Vector3(0,PI/2,0),Vector3(1,4,.8));
	spawnScenery(&mWallpanel, Vector3(36,-18,15),Vector3(0,0,0),Vector3(1,3,1.2));
	spawnScenery(&mStaircase, Vector3(32,-9,14),Vector3(0,PI/2,0),Vector3(1,1,1));
	Staircase *s = spawnStaircase(L"downstairs",&CanisMajor::loadFirstFloor, Vector3(32,-3,20),Vector3(0,PI/2,0),Vector3(1,1,1));
	s->setStateSwitch(&state,&GameState::secondFloorSairsUsed);

	//master bed decor
	spawnScenery(&mMasterbed,Vector3(49,-3,39),Vector3(0,1.5707963268,0), Vector3(5,4,5));
	spawnScenery(&mTable,Vector3(42,-3,43),Vector3(0,0,0), Vector3(.8,1.5,.8));
	spawnScenery(&mTable,Vector3(55.5,-3,43),Vector3(0,0,0), Vector3(.8,1.5,.8));
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
	spawnReadable(&mBook, L"Book", nullptr,Vector3(5.0,.5,17.5),Vector3(0,PI/2,0),Vector3(.8,1,.8), L"August 20th:\nBloody hell, where have those children hidden my artifacts???");
	spawnScenery(&mBookStack,Vector3(2.5,.5,16.8),Vector3(0,0,0),Vector3(1,1,1));
	spawnScenery(&mBottle,Vector3(5,1,13.5),Vector3(0,0,0),Vector3(1,1,1));
	spawnScenery(&mBottle,Vector3(2,.4,14.5),Vector3(0,0,PI/2),Vector3(1,1,1));

	//Office Decor
	spawnSearchable(&mDesk,L"Desk",r3,Vector3(2,-3,47),Vector3(0,PI,0),Vector3(1,1,1));
	spawnScenery(&mChair,Vector3(2.5,-2.6,46),Vector3(0,PI,.6), CM::CHAIR_SCALE);
	spawnScenery(&mBookStack,Vector3(1,1,48.5),Vector3(0,0,0),Vector3(1,1,1));
	spawnReadable(&mBook, L"Book", nullptr,Vector3(3.2,.65,45),Vector3(PI/8,PI/2,0),Vector3(.5,.75,.5), L"Scrawled in child's writing:\nDolan loves it when we hide daddy's things!\n We even hid one in his dresser.");
	spawnScenery(&mFixture,Vector3(1,5,51),Vector3(0,PI/2,0));
	spawnLight(Vector3(1.3,5.3,51));
	spawnScenery(&mTable,Vector3(6,-1.5,57),Vector3(0,0,0),Vector3(2,1,1.2));
	spawnScenery(&mChair,Vector3(4,-3,53),Vector3(0,-PI/2,0), CM::CHAIR_SCALE);
	spawnScenery(&mBookStack,Vector3(2,.7,55.5),Vector3(0,0,0),Vector3(1,1,1));
	spawnReadable(&mBook, L"Book", nullptr,Vector3(2.5,1.5,56),Vector3(PI,PI/2,0),Vector3(.5,.75,.5));
	spawnReadable(&mBook, L"Book", nullptr,Vector3(6,.3,55),Vector3(0,PI,0),Vector3(1.5,1.5,1.5));
	spawnSearchable(&mBookcase,L"Bookcase",nullptr,Vector3(17,2.3,59),Vector3(0,0,0),CM::BOOKCASE_SCALE);
	spawnSearchable(&mBookcase,L"Bookcase",nullptr,Vector3(19,2.3,43),Vector3(0,PI/2,0),CM::BOOKCASE_SCALE);

	//Bathroom Decor
	spawnScenery(&mToilet, Vector3(25,-2.75,28.5),Vector3(0,PI/2,0),Vector3(1.5,1.5,1.5));
	spawnSearchable(&mSink, L"Sink",r2,Vector3(25,-2.75,32.5),Vector3(0,PI/2,0),Vector3(1.5,1.7,1.5));
	spawnSearchable(&mTub,L"Bath Tub",nullptr,Vector3(33.5,-1.2,40),Vector3(0,PI/2,0), Vector3(1.5,2,1.5));

}

void CanisMajor::loadFirstFloor()
{
	state.level = FIRST_FLOOR;
}

void CanisMajor::loadBasement()
{
	state.level = BASEMENT;
}

void CanisMajor::onPlayerDeath()
{
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
void CanisMajor::setStoryText(float durration,wstring s, D3DXCOLOR c)
{
	storyText = s;
	storyTextLifespan = durration;
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
	if(noteTextAge<noteTextLifespan)
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


SearchableActor* CanisMajor::spawnSearchable(Geometry* g, std::wstring name, Actor* in, Vector3 pos, Vector3 rot, Vector3 scale)
{
	for(int i = 0 ; i < CM::MAX_SEARCHABLE_ACTORS; i++)
	{
		if(!searchableActors[i].isActive)
		{
			searchableActors[i].create(pos,rot,scale,in);
			searchableActors[i].setGeometry(g);
			searchableActors[i].name = name;
			return &searchableActors[i];
		}
	}
	return nullptr;
}

ReadableActor* CanisMajor::spawnReadable(Geometry* g, std::wstring name, Actor* in, Vector3 pos, Vector3 rot, Vector3 scale,  wstring text)
{
	for(int i = 0 ; i < CM::MAX_READABLE_ACTORS; i++)
	{
		if(!readableActors[i].isActive)
		{
			readableActors[i].create(pos,rot,scale,in);
			readableActors[i].setGeometry(g);
			readableActors[i].setText(text);
			readableActors[i].name = name;
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

#ifdef DEBUG
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
	audio->playCue(cue,pos);
}
