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
	controls.use = 'E';
	controls.flashlight = 'F';
	controls.crouch = VK_CONTROL;
	controls.run = VK_SHIFT;
	controls.recharge = 'R';

	//Camera Object
	camera.init(this,&mCube,controls);

	scenery = new Actor[CM::MAX_SCENERY];
	searchableActors = new SearchableActor[CM::MAX_SEARCHABLE_ACTORS];
}

CanisMajor::~CanisMajor()
{
	if( md3dDevice )
		md3dDevice->ClearState();

	delete [] scenery;
	delete [] searchableActors;

	ReleaseCOM(mFX);
	ReleaseCOM(mVertexLayout);
}

void CanisMajor::initApp()
{
	

	D3DApp::initApp();

	for(int i = 0; i < MAX_LIGHTS; i++) {
		rLights[i].init();
		rLights[i].pos = Vector3(0, -200, 0);
		lightType[i] = 0;
	}

	// Spotlight--position and direction changed every frame to animate.
	fLight.init(2);  //Flashlight
	ambient.init(1); //Ambientlight (DUH)
	negaLight.init(3); //Dog's anti-light
	eyes.init(5);
	activeLights = 0;

	waiting = false;
	howl = false;

	numwaypoints = 0;

	mTelescope.init(md3dDevice,".\\geometry\\telescope.geo");

	mDresser.init(md3dDevice,".\\geometry\\dresser.geo");

	mFlashlight.init(md3dDevice,".\\geometry\\flashlight.geo");
	flashlight.init(this,&mFlashlight,&fLight);

	mFrame.init(md3dDevice,".\\geometry\\pictureframe.geo");

	mBookcase.init(md3dDevice,".\\geometry\\bookcase.geo", L".\\textures\\medwood.dds");

	mChair.init(md3dDevice,".\\geometry\\chair.geo", L".\\textures\\lightwood.dds");

	mCradle.init(md3dDevice,".\\geometry\\cradle.geo");

	mMasterbed.init(md3dDevice,".\\geometry\\masterBed.geo");

	mServantbed.init(md3dDevice,".\\geometry\\servantBed.geo");

	mStaircase.init(md3dDevice,".\\geometry\\staircase.geo");

	mTable.init(md3dDevice,".\\geometry\\table.geo", L".\\textures\\lightwood.dds");

	mBottle.init(md3dDevice,".\\geometry\\bottle.geo", L".\\textures\\bottlegreen.dds");

	mLock.init(md3dDevice,".\\geometry\\lock.geo");

	mPictureframe.init(md3dDevice,".\\geometry\\pictureframe.geo");

	mRail.init(md3dDevice,".\\geometry\\rail.geo");

	mWallpanel.init(md3dDevice,".\\geometry\\wallpanel.geo", L".\\textures\\greywood.dds");

	mWindow.init(md3dDevice,".\\geometry\\window.geo");

	mCage.init(md3dDevice,".\\geometry\\cage.geo");

	mFixture.init(md3dDevice,".\\geometry\\fixture.geo");

	mDoor.init(md3dDevice,".\\geometry\\door.geo", L".\\textures\\gold.dds");

	mBox.init(md3dDevice,".\\geometry\\cardboardBox.geo", L".\\textures\\cardboard.dds");

	mBook.init(md3dDevice,".\\geometry\\book.geo");

	mToilet.init(md3dDevice,".\\geometry\\toilet.geo");

	for(int i = 0 ; i < CM::MAX_DOORS; i++)
	{
		doors[i].init(this,&mDoor,1);
		doors[i].collisionType = AABBox;
		doors[i].name = L"door";
	}

	mCube.init(md3dDevice,".\\geometry\\cube.geo", L".\\textures\\metal.dds", true);
	doge.init(this,&mCube,1.0f);
	doge.setScale(Vector3(0.1f,5.0f,5.0f));
	doge.setNegalight(&negaLight);
	doge.setEyes(&eyes);

	mRoofHole.init(md3dDevice,".\\geometry\\newRoofHole.geo", L".\\textures\\greywood.dds");

	mKey.init(md3dDevice,".\\geometry\\key.geo", L".\\textures\\gold.dds");

	mWindowPanel.init(md3dDevice,".\\geometry\\windowpanel.geo");
	mBookStack.init(md3dDevice,".\\geometry\\bookStack.geo");
	mDesk.init(md3dDevice,".\\geometry\\desk.geo");

	mSink.init(md3dDevice,".\\geometry\\sink.geo");

	mTub.init(md3dDevice,".\\geometry\\tub.geo");

	for(int i = 0 ; i < CM::MAX_KEYS; i++)
	{
		keys[i].init(this,&mKey,1);
	}

	origin.init(this,1);

	for(int i = 0; i< CM::MAX_SCENERY; i++)
	{
		//default init
		scenery[i].init(this,&mCube);
		scenery[i].collisionType=AABBox;
	}

	for(int i = 0; i< CM::MAX_SEARCHABLE_ACTORS; i++)
	{
		searchableActors[i].init(this,&mCube,1);
		searchableActors[i].collisionType=AABBox;
	}

	for(int i = 0 ; i < CM::MAX_STAIRCASES; i++)
		staircases[i].init(this,&mStaircase,10);

	camera.create(Vector3(10,10,10),Vector3(1,0,0));
	camera.setPerspective();

	flashlight.toggle();

#ifdef DEBUG
	mRedCube.init(md3dDevice,".\\geometry\\cube.geo", L".\\textures\\metal.dds", true);
	AABBHelper.init(this,&mRedCube,1);
	AABBHelper.isActive = true;
#endif

	

	buildFX();
	buildVertexLayouts();
	menuLoad();

}

void CanisMajor::onResize()
{
	D3DApp::onResize();
	camera.setPerspective();
}

void CanisMajor::updateScene(float dt)
{
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

	for(int i = 0 ; i < CM::MAX_SCENERY; i++)
	{
		scenery[i].update(dt);
	}
	for(int i = 0 ; i < CM::MAX_KEYS; i++)
	{
		keys[i].update(dt);
	}
	for(int i = 0 ; i < CM::MAX_DOORS; i++)
	{
		doors[i].update(dt);
	}
	for(int i = 0; i< CM::MAX_SEARCHABLE_ACTORS; i++)
	{
		searchableActors[i].update(dt);
	}

	if(!howl && doors[0].getOpen()) {
		audio->playCue(HOWL);
		howl = true;
	}

	for(int i = 0 ; i < CM::MAX_STAIRCASES; i++)
		staircases[i].update(dt);

	flashlight.update(dt);

	camera.update(dt);
	doge.update(dt);

	updateStoryText(dt);

	collisions();
	
	if(camera.getNearbyItem()!=nullptr)
	{
		drawUtilText(camera.getNearbyItem()->getUtilText());
	}
}

//COLLISIONS GIVE LOADS OF FALSE POSITIVES
void CanisMajor::collisions()
{
	camera.resetNearbyInteractable();
	float dist;

	for(int i = 0; i< CM::MAX_SEARCHABLE_ACTORS; i++)
	{
		if(camera.isPicked(&searchableActors[i],dist))
		{
			camera.setNearbyInteractable(&searchableActors[i],dist);
		}

		if(camera.collided(&searchableActors[i]))
		{
			camera.backUp();
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

	for(int i = 0 ; i < CM::MAX_KEYS; i++)
	{
		if(camera.isPicked(&keys[i],dist))
		{
			camera.setNearbyInteractable(&keys[i],dist);
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

	for(int i = 0 ; i < CM::MAX_STAIRCASES; i++)
	{
		if(camera.isPicked(&staircases[i],dist))
		{
			camera.setNearbyInteractable(&staircases[i],dist);
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
	mSwapChain->Present(0, 0);
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

	doge.draw(mfxWVPVar,mView,mProj,mTech);

	for(int i = 0 ; i < CM::MAX_SCENERY; i++)
	{
		scenery[i].draw(mfxWVPVar,mView,mProj,mTech);
	}
	for(int i = 0 ; i < CM::MAX_KEYS; i++)
	{
		keys[i].draw(mfxWVPVar,mView,mProj,mTech);
	}
	for(int i = 0 ; i < CM::MAX_DOORS; i++)
	{
		doors[i].draw(mfxWVPVar,mView,mProj,mTech);
	}
	for(int i = 0 ; i < CM::MAX_SEARCHABLE_ACTORS; i++)
	{
		searchableActors[i].draw(mfxWVPVar,mView,mProj,mTech);
	}
	for(int i = 0 ; i < CM::MAX_STAIRCASES; i++)
		staircases[i].draw(mfxWVPVar,mView,mProj,mTech);
	flashlight.draw(mfxWVPVar,mView,mProj,mTech);

#ifdef DEBUG
	AABBHelper.draw(mfxWVPVar,mView,mProj,mTech);
#endif

	drawUtilText();
	drawStoryText();

//#ifdef DEBUG
	RECT r = {mClientWidth/2,mClientHeight/2,mClientWidth/2,mClientHeight/2};
	utilFont->DrawText(0,L"\u25CF",-1,&r,DT_NOCLIP|DT_CENTER|DT_VCENTER,WHITE);
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
	for(int i = 0 ; i < CM::MAX_KEYS; i++)
	{
		keys[i].isActive=false;
	}
	for(int i = 0 ; i < CM::MAX_DOORS; i++)
	{
		doors[i].isActive=false;
	}
	for(int i = 0 ; i < CM::MAX_SEARCHABLE_ACTORS; i++)
	{
		searchableActors[i].isActive = false;
	}
	for(int i = 0; i < MAX_LIGHTS; i++) {
		rLights[i].init();
		lightType[i] = 0;
	}
	for(int i = 0 ; i < CM::MAX_STAIRCASES; i++)
		staircases[i].isActive=false;
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
	state.level = MENU;
	clearLevel();
	menuUpdate(0,true);

	menuText[0] = L"CANIS MAJOR";
	menuText[1] = L"PLAY";
	menuText[2] = L"QUIT";
}

void CanisMajor::loadAttic()
{
	state.level = ATTIC;
	clearLevel();
	setStoryText(10,L"WELCOME TO THE ATTIC");
	int iter = 0;

	audio->playCue(BG);

	camera.setPosition(Vector3(5,0,5));

	flashlight.setPosition(Vector3(10,-2.5,5));
	flashlight.isActive = true;

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

	spawnScenery(&mRoofHole,Vector3(19,13.4,7.1),Vector3(0,0,.41),Vector3(6.2, 3, 5.1));

	spawnScenery(&mWallpanel,Vector3(40,5,32),Vector3(0,0,0),Vector3(1,1.6,.6));

	Key* k = spawnKey(L"GOLD KEY",Vector3(37.3,2,51));

	//Bookcases
	spawnSearchable(&mBookcase,L"Bookcase",k,Vector3(37.3,2.3,53.7),Vector3(0,0,0), CM::BOOKCASE_SCALE);
	spawnSearchable(&mBookcase,L"Bookcase",nullptr,Vector3(31,2.3,53.7),Vector3(0,0,0), CM::BOOKCASE_SCALE);

	//Table and chairs
	spawnSearchable(&mTable,L"Table",nullptr,Vector3(10.5,-2.5,35),Vector3(0,PI,0),Vector3(1.2,1.4,1));
	spawnScenery(&mChair,Vector3(8,-3,34),Vector3(0,0,0), CM::CHAIR_SCALE);
	spawnScenery(&mChair,Vector3(11,-1.4,39),Vector3(PI/2,.9,0), CM::CHAIR_SCALE);
	spawnScenery(&mChair,Vector3(13,2.3,34),Vector3(PI,PI,0), CM::CHAIR_SCALE);

	//Bottle on table
	spawnScenery(&mBottle,Vector3(10,.8,36));

	//Boxes
	spawnScenery(&mBox,Vector3(37.5,-2,20),Vector3(0,0,0), CM::BOX_SCALE);
	spawnScenery(&mCube,Vector3(37.5,-2,15.7),Vector3(0,0,0), CM::BOX_SCALE);
	spawnScenery(&mCube,Vector3(37.5,-2,11.5),Vector3(0,0,0), CM::BOX_SCALE);
	spawnScenery(&mBox,Vector3(37.5, 2.1,13.5),Vector3(0,.5,0), CM::BOX_SCALE);

	//Ligthing fixture. We should put a point light here
	spawnScenery(&mFixture,Vector3(20,3, 54),Vector3(0,PI,0));

	//Stairwell
	//Note, you cannot progress forward because of the axis-aligned bounding box
	//Comment out the last wall panel to move forward.
	spawnStaircase(L"downstairs",&CanisMajor::loadSecondFloor,Vector3(40,-4, 27), Vector3(0,0,0), Vector3(1,1,.6));
	spawnScenery(&mWallpanel,Vector3(45,0,30),Vector3(0,1.5707963268,0),Vector3(1,3,1));
	spawnScenery(&mWallpanel,Vector3(45,0,24.7),Vector3(0,1.5707963268,0),Vector3(1,3,1));
	//spawnScenery(&mWallpanel,Vector3(47,5,26),Vector3(0,0,.8),Vector3(1,2,3));

	//Comedic effect cubes
	spawnSearchable(&mBox,L"Conspicuous Cube",nullptr,Vector3(10,-2,10),Vector3(0,0,0),CM::BOX_SCALE);
	spawnSearchable(&mBox,L"Inconspicuous Cube",nullptr,Vector3(22,-2,6),Vector3(0,PI/2,0),CM::BOX_SCALE);

	spawnDoor(Vector3(39.9,-3.5,29),Vector3(0,0,0),Vector3(1.4,3.5,2.1),k);

	negaLight.pos = Vector3(20, 0, 30);
	pLight.pos = Vector3(20, -212, 20);
	spawnLight(Vector3(20,3.5, 53.5));
	spawnLight(Vector3(10.5, 20, 9.8),Vector3(.8, -1, 0), 4);
}

void CanisMajor::loadSecondFloor()
{
	Key* patKey = spawnKey(L"BALCONY KEY",Vector3(7,1,15));
	state.level = SECOND_FLOOR;
	camera.setPosition(Vector3(50,3,50));
	clearLevel();
	flashlight.setPosition(Vector3(10,-2.5,10));
	flashlight.isActive = true;

	doge.isActive = true;
	doge.setPosition(Vector3(10,0,30));

	if (numwaypoints !=0)
		delete [] dogeWaypoints;
	numwaypoints = 7;
	dogeWaypoints = new Vector3[numwaypoints];
	dogeWaypoints[0] = Vector3(10,-2.5,30);
	dogeWaypoints[1] = Vector3(24,-2.5,40);
	dogeWaypoints[2] = Vector3(25,-2.5,54);
	dogeWaypoints[3] = Vector3(65, -2.5,51);
	dogeWaypoints[4] = Vector3(65,-2.5,5);
	dogeWaypoints[5] = Vector3(25,-2.5,5);
	dogeWaypoints[6] = Vector3(25,-2.5,20);
	doge.SetWaypoints(dogeWaypoints,numwaypoints);
	
	//Floor panels
	spawnScenery(&mCube,Vector3(0,-4,30),Vector3(0,0,0),Vector3(28,1,30));
	spawnScenery(&mCube,Vector3(53,-4,30),Vector3(0,0,0),Vector3(17,1,30));
	spawnScenery(&mCube,Vector3(32,-4,4),Vector3(0,0,0),Vector3(4,1,5));
	spawnScenery(&mCube,Vector3(32,-4,40),Vector3(0,0,0),Vector3(4,1,20));

	//Roof panels
	spawnScenery(&mCube,Vector3(0,10,30),Vector3(0,0,0),Vector3(71,1,30));

	//Balcony railing and floor
	spawnScenery(&mCube,Vector3(80,-4,30),Vector3(0,PI/2,0),Vector3(15,1,10));
	for(int i= 0; i<10;i++)
		spawnScenery(&mRail,Vector3(71.5+(i*2),-3,44),Vector3(0,PI/2,0),Vector3(2,2,1.5));
	for(int i= 0; i<15;i++)
		spawnScenery(&mRail,Vector3(89.5,-3,43-(i*2)),Vector3(0,PI,0),Vector3(2,2,1.5));
	for(int i= 0; i<10;i++)
		spawnScenery(&mRail,Vector3(71.5+(i*2),-3,15),Vector3(0,PI/2,0),Vector3(2,2,1.5));

	//Telescope. It should contain something for a puzzle
	spawnSearchable(&mTelescope,L"Telescope",nullptr,Vector3(85,-2.7,40),Vector3(0,-2*PI/3,0),Vector3(7,7,7));

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
	spawnDoor(Vector3(36.5,-3.5,26.7),Vector3(0,PI,0),Vector3(2,4,2));
	spawnScenery(&mWallpanel,Vector3(36,3,37.5),Vector3(0,0,0), Vector3(1,1.2,1.4));

	//Staircase to attic
	spawnScenery(&mCube,Vector3(37,-4,65),Vector3(0,0,0),Vector3(4,1,5));
	spawnScenery(&mWallpanel,Vector3(33,5,70),Vector3(0,0,0),Vector3(4,3,2));
	spawnScenery(&mWallpanel,Vector3(42,5,70),Vector3(0,0,0),Vector3(4,3,2));
	spawnScenery(&mWallpanel,Vector3(42,5,70),Vector3(0,0,0),Vector3(4,3,2));
	spawnStaircase(L"upstairs",&CanisMajor::loadAttic,Vector3(37,-3, 71), Vector3(0,PI/2,0), Vector3(1,1,.9));
	spawnScenery(&mStaircase,Vector3(37,3, 78), Vector3(0,PI/2,0), Vector3(1,1,.9));
	spawnScenery(&mStaircase,Vector3(37,-3, 71.1), Vector3(0,PI/2,0), Vector3(1,1,.9));

	//Staircase to main floor
	spawnScenery(&mWallpanel, Vector3(28,-4,15),Vector3(0,0,0),Vector3(1,4,1.2));
	spawnDoor(Vector3(28,-3,25.3),Vector3(0,0,0),Vector3(2,4,2.2));
	spawnScenery(&mWallpanel, Vector3(32,-4,9),Vector3(0,PI/2,0),Vector3(1,4,.8));
	spawnScenery(&mWallpanel, Vector3(36,-18,15),Vector3(0,0,0),Vector3(1,3,1.2));
	spawnScenery(&mStaircase, Vector3(32,-9,14),Vector3(0,PI/2,0),Vector3(1,1,1));
	spawnScenery(&mStaircase, Vector3(32,-3,20),Vector3(0,PI/2,0),Vector3(1,1,1));

	//master bed decor
	spawnScenery(&mMasterbed,Vector3(49,-3,39),Vector3(0,1.5707963268,0), Vector3(5,4,5));
	spawnScenery(&mTable,Vector3(42,-3,43),Vector3(0,0,0), Vector3(.8,1.5,.8));
	spawnScenery(&mTable,Vector3(55.5,-3,43),Vector3(0,0,0), Vector3(.8,1.5,.8));
	spawnSearchable(&mDresser,L"Dresser",nullptr,Vector3(40,-3,11),Vector3(0,-PI/2,0), Vector3(2,2,2));
	spawnSearchable(&mDresser,L"Dresser",nullptr,Vector3(50,-3,11),Vector3(0,-PI/2,0), Vector3(2,2,2));

	//Library decor
	//Bookcases
	spawnSearchable(&mBookcase,L"Bookcase",nullptr,Vector3(1,2.3,2.5),Vector3(0,-PI/2,0),CM::BOOKCASE_SCALE);
	spawnSearchable(&mBookcase,L"Bookcase",nullptr,Vector3(1,2.3,6.5),Vector3(0,-PI/2,0),CM::BOOKCASE_SCALE);
	spawnSearchable(&mBookcase,L"Bookcase",nullptr,Vector3(7,2.3,1),Vector3(0,PI,0),CM::BOOKCASE_SCALE);
	spawnSearchable(&mBookcase,L"Bookcase",nullptr,Vector3(11,2.3,1),Vector3(0,PI,0),CM::BOOKCASE_SCALE);
	spawnSearchable(&mBookcase,L"Bookcase",nullptr,Vector3(15,2.3,1),Vector3(0,PI,0),CM::BOOKCASE_SCALE);
	spawnSearchable(&mBookcase,L"Bookcase",nullptr,Vector3(17,2.3,19),Vector3(0,0,0),CM::BOOKCASE_SCALE);

	//Table with stuff on it and chair
	spawnSearchable(&mTable,L"Table",patKey,Vector3(5,-2.4,16),Vector3(0,PI/2,0),Vector3(1.5,1.5,1.5));
	spawnScenery(&mChair,Vector3(4.7,-1.8,11),Vector3(1,-PI/2,0), CM::CHAIR_SCALE);
	spawnScenery(&mBook,Vector3(5.5,.5,17.5),Vector3(0,PI/2,0),Vector3(.8,1,.8));
	spawnScenery(&mBookStack,Vector3(2.5,.5,16.8),Vector3(0,0,0),Vector3(1,1,1));
	spawnScenery(&mBottle,Vector3(5,1,13.5),Vector3(0,0,0),Vector3(1,1,1));
	spawnScenery(&mBottle,Vector3(2,.4,14.5),Vector3(0,0,PI/2),Vector3(1,1,1));

	//Office Decor
	spawnSearchable(&mDesk,L"Desk",nullptr,Vector3(2,-1.5,47),Vector3(0,PI,0),Vector3(1,1,1));
	spawnScenery(&mChair,Vector3(2.5,-2.6,46),Vector3(0,PI,.6), CM::CHAIR_SCALE);
	spawnScenery(&mBookStack,Vector3(1,1,48.5),Vector3(0,0,0),Vector3(1,1,1));
	spawnScenery(&mBook,Vector3(2.5,1,45),Vector3(0,PI/2,0),Vector3(.5,.75,.5));
	spawnScenery(&mTable,Vector3(6,-1,57),Vector3(0,0,0),Vector3(2,1,1.2));
	spawnScenery(&mChair,Vector3(4,-3,53),Vector3(0,-PI/2,0), CM::CHAIR_SCALE);
	spawnScenery(&mBookStack,Vector3(2,1,55.5),Vector3(0,0,0),Vector3(1,1,1));
	spawnScenery(&mBook,Vector3(2.5,1.8,56),Vector3(PI,PI/2,0),Vector3(.5,.75,.5));
	spawnScenery(&mBook,Vector3(6,1.2,55),Vector3(0,PI,0),Vector3(1.5,1.5,1.5));
	spawnSearchable(&mBookcase,L"Bookcase",nullptr,Vector3(17,2.3,59),Vector3(0,0,0),CM::BOOKCASE_SCALE);
	spawnSearchable(&mBookcase,L"Bookcase",nullptr,Vector3(19,2.3,43),Vector3(0,PI/2,0),CM::BOOKCASE_SCALE);

	//Bathroom Decor
	spawnScenery(&mToilet, Vector3(25,-1,28.5),Vector3(0,PI/2,0),Vector3(1.5,1.5,1.5));
	spawnScenery(&mSink, Vector3(25,-2.5,33),Vector3(0,PI/2,0),Vector3(1.5,1.7,1.5));
	spawnScenery(&mTub,Vector3(33.5,-1.2,40),Vector3(0,PI/2,0), Vector3(1.5,2,1.5));
	
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

Key* CanisMajor::spawnKey(wstring name, Vector3 pos, Vector3 rot)
{
	for(int i = 0 ; i < CM::MAX_KEYS; i++)
	{
		if(!keys[i].isActive)
		{
			keys[i].create(pos);
			keys[i].name = name;
			keys[i].setRotation(rot);
			return &keys[i];
		}
	}
	return nullptr;
}

Door* CanisMajor::spawnDoor(Vector3 pos, Vector3 rot,Vector3 scale, Key* k, bool open)
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
