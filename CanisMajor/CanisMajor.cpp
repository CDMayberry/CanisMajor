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
	mfxLightVar(0), mfxLightType(0)
{
	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mProj);

	Controls c;
	c.up = 'W';
	c.down = 'S';
	c.left = 'A';
	c.right = 'D';
	c.use = 'E';
	c.flashlight = 'F';
	c.crouch = VK_CONTROL;
	c.run = VK_SHIFT;
	//Camera Object
	camera.init(this,c);

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

	
	for(int i = 0; i < CM::MAX_LIGHTS; i++) {
		rLights[i].init(2);
		rLights[i].pos = Vector3(10, 10, i*30);
	}

	// Spotlight--position and direction changed every frame to animate.
	fLight.init(3);
	ambient.init(1);
	pLight.init(2);
	pLight.pos = Vector3(10, 10, 30);


	mTelescope.init(md3dDevice,".\\geometry\\telescope.geo");

	mDresser.init(md3dDevice,".\\geometry\\dresser.geo");

	mFlashlight.init(md3dDevice,".\\geometry\\flashlight.geo");
	flashlight.init(this,&mFlashlight,&fLight);

	mFrame.init(md3dDevice,".\\geometry\\pictureframe.geo");

	mBookcase.init(md3dDevice,".\\geometry\\bookcase.geo");

	mChair.init(md3dDevice,".\\geometry\\chair.geo");

	mCradle.init(md3dDevice,".\\geometry\\cradle.geo");

	mMasterbed.init(md3dDevice,".\\geometry\\masterBed.geo");

	mServantbed.init(md3dDevice,".\\geometry\\servantBed.geo");

	mStaircase.init(md3dDevice,".\\geometry\\staircase.geo");

	mTable.init(md3dDevice,".\\geometry\\table.geo");

	mBottle.init(md3dDevice,".\\geometry\\bottle.geo");

	mLock.init(md3dDevice,".\\geometry\\lock.geo");

	mPictureframe.init(md3dDevice,".\\geometry\\pictureframe.geo");

	mRail.init(md3dDevice,".\\geometry\\rail.geo");

	mWallpanel.init(md3dDevice,".\\geometry\\wallpanel.geo");

	mWindow.init(md3dDevice,".\\geometry\\window.geo");

	mCage.init(md3dDevice,".\\geometry\\cage.geo");

	mFixture.init(md3dDevice,".\\geometry\\fixture.geo");

	mDoor.init(md3dDevice,".\\geometry\\door.geo");

	for(int i = 0 ; i < CM::MAX_DOORS; i++)
	{
		doors[i].init(this,&mDoor,1);
		doors[i].collisionType = AABBox;
	}

	mCube.init(md3dDevice,".\\geometry\\cube.geo", DARKBROWN);
	
	mRoofHole.init(md3dDevice,".\\geometry\\roofHole.geo");
	
	mKey.init(md3dDevice,".\\geometry\\key.geo", GOLD);

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
	}
	
	camera.create(Vector3(10,10,10),Vector3(1,0,0));
	camera.setPerspective();

	flashlight.toggle();

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
	switch(state){
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

	flashlight.update(dt);

	camera.update(dt);

	updateStoryText(dt);

	collisions();
}

//COLLISIONS GIVE LOADS OF FALSE POSITIVES
void CanisMajor::collisions()
{

	for(int i = 0; i< CM::MAX_SEARCHABLE_ACTORS; i++)
	{
		if(searchableActors[i].isActive)
		{
			Vector3 diff = camera.getPosition()-searchableActors[i].getPosition();
			if(D3DXVec3LengthSq(&diff) < CM::INTERACTION_RADIUS_SQ)
			{
				camera.setNearbyInteractable(&searchableActors[i]);
				drawUtilText(L"Press E to search the " + searchableActors[i].name + L".");
			}

			if(camera.collided(&searchableActors[i]))
			{
				camera.backUp();
			}
		}
	}

	for(int i = 0 ; i < CM::MAX_DOORS; i++)
	{
		if(doors[i].isActive)
		{
			Vector3 diff = camera.getPosition()-doors[i].getPosition();
			if(D3DXVec3LengthSq(&diff) < CM::INTERACTION_RADIUS_SQ)
			{
				camera.setNearbyInteractable(&doors[i]);
				if(doors[i].getOpen())
					drawUtilText(L"Press E to close door.");
				else
					drawUtilText(L"Press E to open door.");
			}

			if(camera.collided(&doors[i]))
			{
				camera.backUp();
			}
		}
	}

	for(int i = 0 ; i < CM::MAX_KEYS; i++)
	{
		if(camera.collided(&keys[i]))
		{
			camera.setNearbyInteractable(&keys[i]);
			drawUtilText(L"Press E to pick up key.");
		}
	}

	if(!camera.hasFlashlight()&&camera.collided(&flashlight))
	{
		camera.setNearbyInteractable(&flashlight);
		drawUtilText(L"Press E to pick up flashlight.");
	}

	for(int i = 0 ; i < CM::MAX_SCENERY;i++)
	{
		if(camera.collided(&scenery[i]))
		{
			camera.backUp();
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
	//mfxEyePosVar->SetRawValue(&pos, 0, sizeof(D3DXVECTOR3));
	mfxLightVar->SetRawValue(&fLight, 0, sizeof(Light));
	mfxAmbientVar->SetRawValue(&ambient, 0, sizeof(Light));
	mfxPLightsVar->SetRawValue(&rLights, 0, sizeof(Light)*4);
	mfxPLightVar->SetRawValue(&pLight, 0, sizeof(Light));
	mfxLightType->SetBool(flashlight.isOn);
	
	// set the point to the shader technique
	D3D10_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);

	switch(state){
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

	flashlight.draw(mfxWVPVar,mView,mProj,mTech);

	drawUtilText();
	drawStoryText();
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
	mfxPLightsVar = mFX->GetVariableByName("lights");
	mfxPLightVar = mFX->GetVariableByName("pLight");
	mfxAmbientVar = mFX->GetVariableByName("ambient");
	mfxLightType = mFX->GetVariableByName("gLightType")->AsScalar();
}

void CanisMajor::buildVertexLayouts()
{
	// Create the vertex input layout.
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"DIFFUSE",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"SPECULAR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 40, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout
	D3D10_PASS_DESC PassDesc;
	mTech->GetPassByIndex(0)->GetDesc(&PassDesc);
	HR(md3dDevice->CreateInputLayout(vertexDesc, 4, PassDesc.pIAInputSignature,
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
}

void CanisMajor::loadSplashScreen(bool status)
{
	state = GameState::SPLASH;
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
	state = GameState::MENU;
	clearLevel();
	menuUpdate(0,true);

	menuText[0] = L"CANIS MAJOR";
	menuText[1] = L"PLAY";
	menuText[2] = L"QUIT";
}

void CanisMajor::loadAttic()
{
	state = ATTIC;
	setStoryText(10,L"WELCOME TO THE ATTIC");
	int iter = 0;

	camera.setPosition(Vector3(5,0,5));

	flashlight.setPosition(Vector3(10,-2.5,5));
	flashlight.isActive = true;

	spawnScenery(&mCube,Vector3(0,-4,0),Vector3(0,0,0),Vector3(200,1,200));

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
		spawnScenery(&mWallpanel,Vector3(4.5,14, iter*10),Vector3(0,0,2),Vector3(1,1.1,1));
		iter++;
	}

	iter = 0;
	//left upper roof. Missing one panel for hole
	for(int i = 27; i < 33; i++)
	{
		if(i != 28)//WTF
			spawnScenery(&mWallpanel,Vector3(14.5,18.6, iter*10),Vector3(0,0,2),Vector3(1,1.5,1));
		iter++;
	}

	iter = 0;
	//right side of roof
	for(int i = 33; i < 39; i++)
	{
		spawnScenery(&mWallpanel,Vector3(30,17, iter*10),Vector3(0,0,1.1),Vector3(1,2.2,1));
		iter++;
	}

	spawnScenery(&mWallpanel,Vector3(10,5,-5.01),Vector3(0,1.5707963268,0),Vector3(1,4,6));

	spawnScenery(&mWallpanel,Vector3(10,5,55.01),Vector3(0,1.5707963268,0),Vector3(1,4,6));

	spawnScenery(&mRoofHole,Vector3(15,18,10),Vector3(0,0,.41),Vector3(4.5, 3, CM::ROOF_SCALE));

	

	spawnSearchable(&mCube,L"Conspicuous Cube",nullptr,Vector3(10,0,10));
	spawnSearchable(&mCube,L"inconspicuous Cube",nullptr,Vector3(20,0,10),Vector3(0,PI/4,0));

	Key* k = spawnKey(L"GOLD KEY",Vector3(20,0,5));
	Door* d = spawnDoor(Vector3(20,0,30),Vector3(0,-PI/2,0),k);
	spawnSearchable(&mBookcase,L"Bookcase",k,Vector3(20,0,5));
	d->setScale(Vector3(1,3,2));
}

void CanisMajor::loadSecondFloor()
{
	state = SECOND_FLOOR;
}

void CanisMajor::loadFirstFloor()
{
	state = FIRST_FLOOR;
}

void CanisMajor::loadBasement()
{
	state = BASEMENT;
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

Door* CanisMajor::spawnDoor(Vector3 pos, Vector3 rot,Key* k, bool open)
{
	for(int i = 0 ; i < CM::MAX_DOORS; i++)
	{
		if(!doors[i].isActive)
		{
			doors[i].create(pos,rot,k,open);
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