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

	
}

CanisMajor::~CanisMajor()
{
	if( md3dDevice )
		md3dDevice->ClearState();

	ReleaseCOM(mFX);
	ReleaseCOM(mVertexLayout);
}

void CanisMajor::initApp()
{
	D3DApp::initApp();


	Controls c;
	c.up = 'W';
	c.down = 'S';
	c.left = 'A';
	c.right = 'D';
	c.use = 'E';
	
	mTelescope.init(md3dDevice,".\\geometry\\telescope.geo");
	telescope.init(this,&mTelescope);
	telescope.setScale(Vector3(3,3,3));
	telescope.create(Vector3(0,-3,0));

	mDresser.init(md3dDevice,".\\geometry\\dresser.geo");
	dresser.init(this,&mDresser);
	dresser.create(Vector3(10,-3,0));

	mFlashlight.init(md3dDevice,".\\geometry\\flashlight.geo");
	flashlight.init(this,&mFlashlight);
	flashlight.create(Vector3(20,-3,0));

	mFrame.init(md3dDevice,".\\geometry\\pictureframe.geo");
	frame.init(this,&mFrame);
	frame.create(Vector3(40,-3,0));

	mBookcase.init(md3dDevice,".\\geometry\\bookcase.geo");
	bookcase.init(this,&mBookcase);
	bookcase.create(Vector3(50,-3,0));

	mChair.init(md3dDevice,".\\geometry\\chair.geo");
	chair.init(this,&mChair);
	chair.create(Vector3(60,-3,0));

	mCradle.init(md3dDevice,".\\geometry\\cradle.geo");
	cradle.init(this,&mCradle);
	cradle.create(Vector3(70,-3,0));

	mMasterbed.init(md3dDevice,".\\geometry\\masterBed.geo");
	masterbed.init(this,&mMasterbed);
	masterbed.create(Vector3(80,-3,0));

	mServantbed.init(md3dDevice,".\\geometry\\servantBed.geo");
	servantbed.init(this,&mServantbed);
	servantbed.create(Vector3(90,-3,0));

	mStaircase.init(md3dDevice,".\\geometry\\staircase.geo");
	staircase.init(this,&mStaircase);
	staircase.create(Vector3(110,-3,0));

	mTable.init(md3dDevice,".\\geometry\\table.geo");
	table.init(this,&mTable);
	table.create(Vector3(120,-3,0));

	mBottle.init(md3dDevice,".\\geometry\\bottle.geo");
	bottle.init(this,&mBottle);
	bottle.create(Vector3(130,-3,0));

	mLock.init(md3dDevice,".\\geometry\\lock.geo");
	lock.init(this,&mLock);
	lock.create(Vector3(135,-3,0));

	mPictureframe.init(md3dDevice,".\\geometry\\pictureframe.geo");
	pictureFrame.init(this,&mPictureframe);
	pictureFrame.create(Vector3(170,-3,0));

	mRail.init(md3dDevice,".\\geometry\\rail.geo");
	rail.init(this,&mRail);
	rail.create(Vector3(140,-3,0));

	mWallpanel.init(md3dDevice,".\\geometry\\wallpanel.geo");
	wallPanel.init(this,&mWallpanel);
	wallPanel.create(Vector3(150,-3,0));

	mWindow.init(md3dDevice,".\\geometry\\window.geo");
	window.init(this,&mWindow);
	window.create(Vector3(160,-3,0));

	mCage.init(md3dDevice,".\\geometry\\cage.geo");
	cage.init(this,&mCage);
	cage.create(Vector3(0,-3,5));

	mFixture.init(md3dDevice,".\\geometry\\fixture.geo");
	fixture.init(this,&mFixture);
	fixture.create(Vector3(190,-3,0));

	mDoor.init(md3dDevice,".\\geometry\\door.geo");
	door.init(this,&mDoor);
	door.create(Vector3(200,-3,0));

	mCube.init(md3dDevice,".\\geometry\\cube.geo", DARKBROWN);
	cube.init(this,&mCube);
	cube.setScale(Vector3(200,1,200));
	cube.create(Vector3(0,-4,0));

	origin.init(this,1);

	//qFloor.init(md3dDevice, BLUE);

	//floor.init(this, &qFloor);
	//floor.setScale(Vector3(50,1,50));
	//floor.create(Vector3(0,-5,0));

	//Camera Object
	camera.init(this,c);
	camera.create(Vector3(10,10,10),Vector3(1,0,0));
	camera.setPerspective();
	// camera
//	camera.setLight(&mLights[2]);

	buildFX();
	buildVertexLayouts();
	menuLoad();

//	mLightType = 0;
	camera.setLight(&fLight);

	for(int i = 0; i < CM::MAX_LIGHTS; i++) {
		rLights[i].init(2);
		rLights[i].pos = Vector3(i*50, 10, -30);
	}

	// Spotlight--position and direction changed every frame to animate.
	fLight.init(3);
	ambient.init(1);
	pLight.init(2);
	pLight.pos = Vector3(10, 10, 30);

	buildFX();
	buildVertexLayouts();
	menuLoad();

	lightOn = false;
	lPress = false;
	lPressing = false;
}

void CanisMajor::onResize()
{
	D3DApp::onResize();

	float aspect = (float)mClientWidth/mClientHeight;
	D3DXMatrixPerspectiveFovLH(&mProj, 0.25f*PI, aspect, 1.0f, 1000.0f);
}

void CanisMajor::updateScene(float dt)
{
	//Switch between different light types, demo
	if((GetAsyncKeyState('F') & 0x8000))	{		
		if(!lPress) {
			lightOn = !lightOn;
		}
		lPress = true;
	}
	else
		lPress = false;



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
	
	////cameraTarget = camera.getDirection();
	//cameraTarget = camera.getPosition();
	////cameraTarget = telescope.getPosition();
	//cameraDisplacement = Vector3(50,10,0);
	//pos = cameraTarget+cameraDisplacement;
	//D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	//D3DXMatrixLookAtLH(&mView, &pos, &cameraTarget, &up);

	// The spotlight takes on the camera position and is aimed in the
	// same direction the camera is looking.  In this way, it looks
	// like we are holding a flashlight.
	//mLights[2].pos = camera.getPosition();
	
	//Vector3 flashlight = -cameraDisplacement;
	fLight.pos = camera.getPosition();

	//Vector3 flashlight = -cameraDisplacement+cameraTarget;
	//D3DXVec3Composite(&flashlight, &-cameraDisplacement,&cameraTarget);

	//D3DXVec3Normalize(&mLights[2].dir, &mLights[2].dir); 
	//mLights[2].dir.z = .1;
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
	telescope.update(dt);
	dresser.update(dt);
	flashlight.update(dt);
	frame.update(dt);
	bookcase.update(dt);
	chair.update(dt);
	cradle.update(dt);
	masterbed.update(dt);
	servantbed.update(dt);
	staircase.update(dt);
	table.update(dt);
	bottle.update(dt);
	pictureFrame.update(dt);
	rail.update(dt);
	wallPanel.update(dt);
	window.update(dt);
	lock.update(dt);
	cage.update(dt);
	fixture.update(dt);
	door.update(dt);

	cube.update(dt);
	//floor.update(dt);
	camera.update(dt);
	collisions();
}

//COLLISIONS GIVE LOADS OF FALSE POSITIVES
void CanisMajor::collisions()
{
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
	mfxLightType->SetBool(lightOn);
	
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


	//floor.draw(mfxWVPVar,mView,mProj,mTech);
	cube.draw(mfxWVPVar,mView,mProj,mTech);
	origin.draw(mfxWVPVar,mView,mProj,mTech);
	telescope.draw(mfxWVPVar,mView,mProj,mTech);
	dresser.draw(mfxWVPVar,mView,mProj,mTech);
	flashlight.draw(mfxWVPVar,mView,mProj,mTech);
	frame.draw(mfxWVPVar,mView,mProj,mTech);
	bookcase.draw(mfxWVPVar,mView,mProj,mTech);
	chair.draw(mfxWVPVar,mView,mProj,mTech);
	cradle.draw(mfxWVPVar,mView,mProj,mTech);
	masterbed.draw(mfxWVPVar,mView,mProj,mTech);
	servantbed.draw(mfxWVPVar,mView,mProj,mTech);
	staircase.draw(mfxWVPVar,mView,mProj,mTech);
	table.draw(mfxWVPVar,mView,mProj,mTech);
	lock.draw(mfxWVPVar,mView,mProj,mTech);
	bottle.draw(mfxWVPVar,mView,mProj,mTech);
	pictureFrame.draw(mfxWVPVar,mView,mProj,mTech);
	rail.draw(mfxWVPVar,mView,mProj,mTech);
	wallPanel.draw(mfxWVPVar,mView,mProj,mTech);
	window.draw(mfxWVPVar,mView,mProj,mTech);
	cage.draw(mfxWVPVar,mView,mProj,mTech);
	fixture.draw(mfxWVPVar,mView,mProj,mTech);
	door.draw(mfxWVPVar,mView,mProj,mTech);
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