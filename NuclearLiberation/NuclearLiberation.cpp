#include"NuclearLiberation.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif


	NuclearLiberation theApp(hInstance);
	
	theApp.initApp();

	return theApp.run();
}



NuclearLiberation::NuclearLiberation(HINSTANCE hInstance)
: D3DApp(hInstance), mFX(0), mTech(0), mVertexLayout(0),
  mfxWVPVar(0)
{
	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mProj);

	invisibleWallLocation = 0;

	playerBullets = new Bullet[NL::MAX_PLAYER_BULLETS];
	air = new Air[NL::MAX_DROPS];
	power = new Power[NL::MAX_DROPS];
	shield = new Shield[NL::MAX_DROPS];
	walls = new Wall[NL::MAX_WALLS];
	enemyBullets = new Bullet[NL::MAX_ENEMY_BULLETS];
	enemyLight = new EnemyLight[NL::MAX_LIGHT_ENEMIES];
	enemyHeavy = new EnemyHeavy[NL::MAX_HEAVY_ENEMIES];
	enemySplit = new EnemySplit[NL::MAX_SPLIT_ENEMEIS];
	enemyBoat = new EnemyBoat[NL::MAX_BOAT_ENEMEIS];
}

NuclearLiberation::~NuclearLiberation()
{
	if( md3dDevice )
		md3dDevice->ClearState();

	ReleaseCOM(mFX);
	ReleaseCOM(mVertexLayout);

	delete [] playerBullets;
	delete [] air;
	delete [] power;
	delete [] shield;
	delete [] walls;
	delete [] enemyBullets;
	delete [] enemyLight;
	delete [] enemyHeavy;
	delete [] enemySplit;
	delete [] enemyBoat;
}

void NuclearLiberation::initApp()
{
	D3DApp::initApp();

	test = false;

	origin.init(this,1);

	cameraDisplacement = Vector3(0,0,-75);
	cameraTarget = Vector3(0,0,0);

	//Cube cubeG,cubeR,cubeY,cubeW;
	//Line lineX, lineY, lineZ;

	cubeG.init(md3dDevice,GREEN);
	cubeR.init(md3dDevice,RED);
	cubeY.init(md3dDevice,DARKGRAY);
	cubeW.init(md3dDevice,WHITE);
	cubeGLD.init(md3dDevice,GOLD);
	cubeLGRY.init(md3dDevice,LTEGRAY);
	whiteCoin.init(md3dDevice,WHITE);
	greenCoin.init(md3dDevice,GREEN);
	goldQuad.init(md3dDevice,GOLD);
	redCoin.init(md3dDevice,RED);
	cubeSub.init(md3dDevice,D3DXCOLOR(184/255.0,115/255.0,51/255.0,1));
	purpleCube.init(md3dDevice,VIOLENTVIOLET);
	//goldCoin.init(md3dDevice,GOLD);
	cyanCoin.init(md3dDevice,CYAN);

	//Inititilizes the background colors for the level.
	//The magic numbers are x and y locations. All x's are -20
	//to cover the very first portion of the screen
	bgQuad[0].init(md3dDevice,BLACK,DARKBLUE);
	bgQuad[1].init(md3dDevice,DARKBLUE,DARKISHBLUE);
	bgQuad[2].init(md3dDevice,DARKISHBLUE,SURFACEBLUE);
	bgQuad[3].init(md3dDevice,GOLD,VIOLENTVIOLET);

	initBackground();
	

	Controls c;
	c.up = 'W';
	c.down = 'S';
	c.left = 'A';
	c.right = 'D';
	c.fire = ' ';
	player.init(this,&cubeSub,&redCoin,&cyanCoin,1,c);
	player.setScale(Vector3(2,1,1));
	player.setRadius(1);
	
	

	for(int i = 0 ; i < NL::MAX_PLAYER_BULLETS; i++)
	{
		playerBullets[i].init(this,&cubeG,1);
		playerBullets[i].setScale(Vector3(0.5,0.5,0.5));
		playerBullets[i].setRadius(1);
	}

	for(int i = 0 ; i < NL::MAX_DROPS; i++)
	{

		air[i].init(this,&whiteCoin,1);
		air[i].setScale(Vector3(2,2,1));
		air[i].setRadius(2);

		power[i].init(this,&greenCoin,1);
		power[i].setScale(Vector3(1,1,1));
		power[i].setRadius(2);

		shield[i].init(this,&cyanCoin,1);
		shield[i].setScale(Vector3(1,1,1));
		shield[i].setRadius(2);
	}

	for(int i = 0 ; i < NL::MAX_ENEMY_BULLETS; i++)
	{
		enemyBullets[i].init(this,&cubeR,1);
		enemyBullets[i].setScale(Vector3(0.5,0.5,0.5));
		enemyBullets[i].setRadius(0.5);
	}

	for(int i = 0 ; i < NL::MAX_WALLS; i++)
	{
		walls[i].init(this,&cubeY,1);
		//walls[i].setPosition(Vector3(rand()%1000,rand()%1000,0));
		//walls[i].isActive = true;
		walls[i].setScale(Vector3(wallNS::WALL_SCALE,wallNS::WALL_SCALE,1));
	}

	//THIS IS TERRIBLE           vvvvvvvvv     MAKE DIFFERENT CONSTANTS
	for(int i = 0; i < NL::MAX_LIGHT_ENEMIES; i++)
	{
		enemyLight[i].init(this,&cubeR,2);
		enemyLight[i].setScale(Vector3(2,2,2));
		enemyHeavy[i].init(this,&cubeG, 2);
		enemyHeavy[i].setScale(Vector3(2,2,2));
		enemySplit[i].init(this,&purpleCube,2);
		enemySplit[i].setScale(Vector3(2,2,2));
	}

	for(int i = 0 ; i < NL::MAX_BOAT_ENEMEIS;i++)
	{
		enemyBoat[i].init(this,&cubeR,3);
		enemyBoat[i].setScale(Vector3(3,2,1));
	}

	finishLine.init(this,&goldQuad,1);


	quadLtBlue.init(md3dDevice,D3DXCOLOR(224/255.0,1,1,1));
	airBar.init(this,&quadLtBlue,1);
	airBar.isActive = true;

	buildFX();
	buildVertexLayouts();
	menuLoad();
}

void NuclearLiberation::initBackground()
{
	float halfHeight = worldSize.y/(2*(NL::NUM_BKGD_IMGS-1));
	for(int i = 0 ; i < NL::NUM_BKGD_IMGS; i++)
	{
		bgImg[i].init(this,&bgQuad[i],1);
		bgImg[i].setPosition(Vector3(worldSize.x/2,(2*i+1)*halfHeight,5));//set in middle, place behind everything
		bgImg[i].setScale(Vector3(2*worldSize.x,2*halfHeight,1));
		bgImg[i].isActive = true;
	}
}

void NuclearLiberation::onResize()
{
	D3DApp::onResize();

	float aspect = (float)mClientWidth/mClientHeight;
	D3DXMatrixPerspectiveFovLH(&mProj, 0.25f*PI, aspect, 1.0f, 1000.0f);
}

void NuclearLiberation::updateScene(float dt)
{
	D3DApp::updateScene(dt);
	if(GetAsyncKeyState(VK_ESCAPE))
		PostQuitMessage(0);
	switch(state){
	case MENU:
		menuUpdate(dt);
		break;
	default:
		levelsUpdate(dt);
		break;
	}
	D3DXVECTOR3 pos = cameraTarget+cameraDisplacement;
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&mView, &pos, &cameraTarget, &up);
}

void NuclearLiberation::menuUpdate(float dt, bool reset)
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
				loadLevel3();
				break;
			case 2://feeling lucky
				//TODO::SOMETHING
				break;
			case 3://quit
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

	if(menuChoice >= (NL::NUM_MENU_ITEMS)) menuChoice = 1;
	if(menuChoice <= 0) menuChoice = NL::NUM_MENU_ITEMS-2;
}

void NuclearLiberation::levelsUpdate(float dt)
{
	
	for(int i = 0 ; i < NL::NUM_BKGD_IMGS; i++)
		bgImg[i].update(dt);

	finishLine.update(dt);

	player.update(dt);

	//update bar
	airBar.setPosition(player.getPosition()+Vector3(-5,0,0));
	airBar.setScale(Vector3(1,2*player.getAir()/playerNS::MAX_AIR,1));
	airBar.update(dt);

	//ADVANCE INVISIBLE WALL
	invisibleWallLocation= min(invisibleWallLocation+NL::MIN_SCROLL_SPEED*dt,worldSize.x-NL::PRECEIVED_SCREEN_WIDTH);
	//TRACK INVISIBLE WALL WITH PLAYER
	invisibleWallLocation = max(player.getPosition().x - NL::PRECEIVED_SCREEN_WIDTH*0.7,invisibleWallLocation);

	for(int i = 0 ; i < NL::MAX_PLAYER_BULLETS; i++)
	{
		playerBullets[i].update(dt);
	}
	for(int i = 0 ; i < NL::MAX_WALLS; i++)
	{
		walls[i].update(dt);
	}
	for(int i = 0; i < NL::MAX_DROPS; i++) {
		air[i].update(dt);
		power[i].update(dt);				//Making it easier unless we require them to be different sizes
		shield[i].update(dt);
	}
	for(int i = 0; i < NL::MAX_LIGHT_ENEMIES; i++)
	{
		enemyLight[i].update(dt);
	}
	for (int i=0;i<NL::MAX_HEAVY_ENEMIES;i++){
		enemyHeavy[i].update(dt);
	}
	for (int i=0;i<NL::MAX_SPLIT_ENEMEIS;i++){
		enemySplit[i].update(dt);
	}
	for(int i = 0; i < NL::MAX_ENEMY_BULLETS; i++)
	{
		enemyBullets[i].update(dt);
	}
	for(int i = 0; i < NL::MAX_BOAT_ENEMEIS; i++)
	{
		enemyBoat[i].update(dt);
	}

	// Build the view matrix.

	D3DXVECTOR3 diff = player.getPosition() - cameraTarget;

	//IF THE PLAYER MOVES TOO FAR UP OR DOWN
	if(diff.y >  NL::PRECEIVED_SCREEN_HEIGHT*0.2)
		cameraTarget.y = player.getPosition().y - NL::PRECEIVED_SCREEN_HEIGHT*0.2;
	if(diff.y < - NL::PRECEIVED_SCREEN_HEIGHT*0.2)
		cameraTarget.y = player.getPosition().y + NL::PRECEIVED_SCREEN_HEIGHT*0.2;

	//MOVE CAMERA WITH INVISIBLE WALL (THIS HANDLES ALL X MOVEMENT)
	if(cameraTarget.x < invisibleWallLocation + NL::PRECEIVED_SCREEN_WIDTH/2)
		cameraTarget.x = invisibleWallLocation + NL::PRECEIVED_SCREEN_WIDTH/2;
	//stop camera before end of world
	if(cameraTarget.x > worldSize.x-NL::PRECEIVED_SCREEN_WIDTH/2)
		cameraTarget.x = worldSize.x-NL::PRECEIVED_SCREEN_WIDTH/2;



	collisions();
}

//COLLISIONS GIVE LOADS OF FALSE POSITIVES
void NuclearLiberation::collisions()
{
	for(int i = 0; i < NL::MAX_ENEMY_BULLETS; i++)
	{
		if(enemyBullets[i].collided(&player))
		{
			player.takeDamage();//will call onPlayerDeath if dead
			enemyBullets[i].isActive=false;
			break;
		}
	}

	for(int i = 0; i < NL::MAX_DROPS; i++) {
		if(air[i].collided(&player))
		{
			air[i].setHealth(0);
			break;
		}

	}

	for(int i = 0; i < NL::MAX_DROPS; i++) {
		if(shield[i].collided(&player))
		{
			shield[i].setHealth(0);
			break;
		}

	}

	for(int i = 0; i < NL::MAX_DROPS; i++) {
		if(power[i].collided(&player))
		{
			power[i].setHealth(0);
			break;
		}

	}

	for (int i = 0; i<NL::MAX_PLAYER_BULLETS;i++){
		if(playerBullets[i].isActive){//don't bother checking if the bullet isn't active
			for (int j = 0;j< NL::MAX_HEAVY_ENEMIES;j++){
				if(enemyHeavy[j].collided(&playerBullets[i])){
					enemyHeavy[j].setHealth(enemyHeavy[j].getHealth() - bulletNS::DAMAGE);
					playerBullets[i].isActive = false;
					break;
				}
			}
			for (int j = 0;j<NL::MAX_LIGHT_ENEMIES;j++){
				if (enemyLight[j].collided(&playerBullets[i])){
					enemyLight[j].setHealth(enemyLight[j].getHealth() - bulletNS::DAMAGE);
					playerBullets[i].isActive = false;
					break;
				}
			}
			for (int j = 0;j<NL::MAX_BOAT_ENEMEIS;j++){
				if (enemyBoat[j].collided(&playerBullets[i])){
					enemyBoat[j].setHealth(enemyBoat[j].getHealth() - bulletNS::DAMAGE);
					playerBullets[i].isActive = false;
					break;
				}
			}
			for (int j = 0;j<NL::MAX_SPLIT_ENEMEIS;j++){
				if (enemySplit[j].collided(&playerBullets[i])){
					enemySplit[j].setHealth(enemySplit[j].getHealth() - bulletNS::DAMAGE);
					if(enemySplit[j].getHealth() <= 75 && enemySplit[j].getGen() ==1){
						enemySplit[j].setGen(2);
						spawnSplitEnemy(enemySplit[j].getPosition()+D3DXVECTOR3(3,3,3), 2);
					}
					else if(enemySplit[j].getHealth() <= 50 && enemySplit[j].getGen() ==2){
						enemySplit[j].setGen(3);
						spawnSplitEnemy(enemySplit[j].getPosition()+D3DXVECTOR3(3,3,3), 3);
					}
					else if(enemySplit[j].getHealth() <= 25)
						enemySplit[j].isActive = false;
					playerBullets[i].isActive = false;
					break;
				}
			}
		}
	}
}

void NuclearLiberation::drawScene()
{
	D3DApp::drawScene();

	// Restore default states, input layout and primitive topology 
	// because mFont->DrawText changes them.  Note that we can 
	// restore the default states by passing null.
	md3dDevice->OMSetDepthStencilState(0, 0);
	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	md3dDevice->OMSetBlendState(0, blendFactors, 0xffffffff);
    md3dDevice->IASetInputLayout(mVertexLayout);

	
	// set the point to the shader technique
	D3D10_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);

	switch(state){
	case MENU:
		menuDraw();
		break;
	default:
		levelsDraw();
		break;
	}
	mSwapChain->Present(0, 0);
}

void NuclearLiberation::menuDraw()
{
	for(int i = 0; i < NL::NUM_MENU_ITEMS; i++)
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

void NuclearLiberation::levelsDraw()
{
	
	origin.draw(mfxWVPVar,mView,mProj,mTech);
	
	player.draw(mfxWVPVar,mView,mProj,mTech);

	airBar.draw(mfxWVPVar,mView,mProj,mTech);

	for(int i = 0 ; i < NL::NUM_BKGD_IMGS; i++)
		bgImg[i].draw(mfxWVPVar,mView,mProj,mTech);

	for(int i = 0 ; i < NL::MAX_PLAYER_BULLETS; i++)
	{
		playerBullets[i].draw(mfxWVPVar,mView,mProj,mTech);
	}

	for(int i = 0 ; i < NL::MAX_WALLS; i++)
	{
		walls[i].draw(mfxWVPVar,mView,mProj,mTech);
	}
	for(int i = 0 ; i < NL::MAX_DROPS; i++)
	{
		air[i].draw(mfxWVPVar,mView,mProj,mTech);
	}
	for(int i = 0 ; i < NL::MAX_DROPS; i++)
	{
		power[i].draw(mfxWVPVar,mView,mProj,mTech);
	}

	for(int i = 0 ; i < NL::MAX_DROPS; i++)
	{
		shield[i].draw(mfxWVPVar,mView,mProj,mTech);
	}

	for(int i = 0; i < NL::MAX_LIGHT_ENEMIES; i++)
	{
		enemyLight[i].draw(mfxWVPVar,mView,mProj,mTech);
	}
	for (int i=0;i < NL::MAX_HEAVY_ENEMIES;i++){
		enemyHeavy[i].draw(mfxWVPVar,mView,mProj, mTech);
	}
	for (int i=0;i< NL::MAX_SPLIT_ENEMEIS;i++){
		enemySplit[i].draw(mfxWVPVar, mView, mProj, mTech);
	}
	for(int i = 0; i < NL::MAX_ENEMY_BULLETS; i++)
	{
		enemyBullets[i].draw(mfxWVPVar,mView,mProj,mTech);
	}
	for(int i = 0; i < NL::MAX_BOAT_ENEMEIS; i++)
	{
		enemyBoat[i].draw(mfxWVPVar,mView,mProj,mTech);
	}
	finishLine.draw(mfxWVPVar,mView,mProj,mTech);
}

void NuclearLiberation::buildFX()
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
 
	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	hr = D3DX10CreateEffectFromFile(L"color.fx", 0, 0, 
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

	mTech = mFX->GetTechniqueByName("ColorTech");
	
	mfxWVPVar = mFX->GetVariableByName("gWVP")->AsMatrix();
}

void NuclearLiberation::buildVertexLayouts()
{
	// Create the vertex input layout.
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout
    D3D10_PASS_DESC PassDesc;
    mTech->GetPassByIndex(0)->GetDesc(&PassDesc);
    HR(md3dDevice->CreateInputLayout(vertexDesc, 2, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &mVertexLayout));
}

void NuclearLiberation::spawnBullet(Vector3 pos, Vector3 vel, float scale)
{
	for(int i = 0; i<NL::MAX_PLAYER_BULLETS; i++)
	{
		if(!playerBullets[i].isActive)
		{
			playerBullets[i].create(pos);
			playerBullets[i].setVelocity(vel);
			playerBullets[i].setScale(Vector3(scale,scale,scale));
			break;
		}
	}
}

void NuclearLiberation::spawnEnemyBullet(Vector3 pos, Vector3 vel)
{
	for(int i = 0; i<NL::MAX_ENEMY_BULLETS; i++)
	{
		if(!enemyBullets[i].isActive)
		{
			enemyBullets[i].create(pos);
			enemyBullets[i].setVelocity(vel);
			break;
		}
	}
}

void NuclearLiberation::spawnAir(Vector3 pos, Vector3 vel)
{
	for(int i = 0; i<NL::MAX_DROPS; i++)
	{
		if(!air[i].isActive)
		{
			air[i].create(pos);
			air[i].setVelocity(vel);
			break;
		}
	}
}

void NuclearLiberation::spawnPower(Vector3 pos, Vector3 vel)
{
	for(int i = 0; i<NL::MAX_DROPS; i++)
	{
		if(!power[i].isActive)
		{
			power[i].create(pos);
			power[i].setVelocity(vel);
			break;
		}
	}
}

void NuclearLiberation::spawnShield(Vector3 pos, Vector3 vel)
{
	for(int i = 0; i<NL::MAX_DROPS; i++)
	{
		if(!shield[i].isActive)
		{
			shield[i].create(pos);
			shield[i].setVelocity(vel);
			break;
		}
	}
}

void NuclearLiberation::spawnLightEnemy(Vector3 pos)
{
	for(int i = 0; i<NL::MAX_LIGHT_ENEMIES; i++)
	{
		if(!enemyLight[i].isActive)
		{
			enemyLight[i].create(pos);
			break;
		}
	}
}

void NuclearLiberation::spawnHeavyEnemy(Vector3 pos)
{
	for(int i = 0; i<NL::MAX_HEAVY_ENEMIES; i++)
	{
		if(!enemyHeavy[i].isActive)
		{
			enemyHeavy[i].create(pos);
			break;
		}
	}
}

void NuclearLiberation::spawnSplitEnemy(Vector3 pos, int gen){
	for (int i=0;i<NL::MAX_SPLIT_ENEMEIS;i++){
		if (!enemySplit[i].isActive){
			enemySplit[i].create(pos);
			enemySplit[i].setGen(gen);
			break;
		}
	}
}

void NuclearLiberation::spawnWall(Vector3 pos)
{
	for(int i = 0; i<NL::MAX_WALLS; i++)
	{
		if(!walls[i].isActive)
		{
			walls[i].create(pos);
			break;
		}
	}
}

void NuclearLiberation::spawnEnemyBoat(float pos)
{
	for(int i = 0; i<NL::MAX_BOAT_ENEMEIS; i++)
	{
		if(!enemyBoat[i].isActive)
		{
			enemyBoat[i].create(Vector3(pos,0,0));//enemy boat auto places the y coord and removes if blocked
			break;
		}
	}
}

void NuclearLiberation::clearLevel()
{
	for(int i = 0 ; i < NL::MAX_PLAYER_BULLETS; i++)
	{
		playerBullets[i].isActive = false;
	}
	for(int i = 0 ; i < NL::MAX_WALLS; i++)
	{
		walls[i].isActive = false;
	}
	for(int i = 0; i < NL::MAX_LIGHT_ENEMIES; i++)
	{
		enemyLight[i].isActive = false;
	}
	for(int i = 0; i< NL::MAX_HEAVY_ENEMIES;i++){
		enemyHeavy[i].isActive = false;
	}
	for (int i=0;i<NL::MAX_SPLIT_ENEMEIS;i++){
		enemySplit[i].isActive = false;
	}
	for(int i = 0; i < NL::MAX_BOAT_ENEMEIS; i++)
	{
		enemyBoat[i].isActive = false;
	}
	for(int i = 0; i < NL::MAX_ENEMY_BULLETS; i++)
	{
		enemyBullets[i].isActive = false;
	}
	for(int i = 0 ; i < NL::MAX_DROPS; i++)
		air[i].isActive=power[i].isActive=shield[i].isActive=false;
}

void NuclearLiberation::menuLoad()
{
	state = GameState::MENU;
	clearLevel();
	menuUpdate(0,true);

	menuText[0] = L"NUCLEAR LIBERATION";
	menuText[1] = L"PLAY";
	menuText[2] = L"FEELING LUCKY";
	menuText[3] = L"QUIT";
	menuText[4] = L"";
	menuText[5] = L"WASD - MOVE       SPACE - SHOOT";
	menuText[6] = L"WATCH YOUR AIR LEVEL";
}

void NuclearLiberation::loadLevel1()
{
	audio->stopCue(PEXP);
	state = GameState::L1;
	clearLevel();
	worldSize = Vector3(700,250,0);
	player.setPosition(Vector3(25,100,0));
	invisibleWallLocation = 0;
	cameraTarget = player.getPosition();
	player.refillAir();
	initBackground();
	player.isActive = true;
	placeFinishLine();
	placeEnemyBoats(20);
	int which = 0;
	for(int i = 50; i < 500; i+=100) {
		switch(which%3) {
		case 0:
			spawnLightEnemy(Vector3(i+15,30*sin(2*PI*i/50)+50,0));
			break;
		case 1:
			spawnHeavyEnemy(Vector3(i+15,30*cos(2*PI*i/50)+50,0));
			break;
		case 2:
			spawnSplitEnemy(Vector3(i+10, 30*tan(2*PI*i/50)+50,0), 1);
			break;
		}
		
		which++;
	}
	
	spawnAllWallsOnMap();
}

void NuclearLiberation::loadLevel2()
{
		audio->stopCue(PEXP);
	state = GameState::L2;
	clearLevel();
	worldSize = Vector3(700,250,0);
	player.setPosition(Vector3(25,100,0));
	invisibleWallLocation = 0;
	cameraTarget = player.getPosition();
	player.refillAir();
	initBackground();
	player.isActive = true;
	placeFinishLine();
	int which = 0;
	for(int i = 50; i < 500; i+=100)
	{
		switch(which%3)
		{
		case 0:
			spawnLightEnemy(Vector3(i+15,30*sin(2*PI*i/50)+50,0));
			break;
		case 1:
			spawnHeavyEnemy(Vector3(i+15,30*cos(2*PI*i/50)+50,0));
			break;
		case 2:
			spawnSplitEnemy(Vector3(i+10, 30*tan(2*PI*i/50)+50,0), 1);
			break;
		}
		
		which++;
	}
	
	spawnAllWallsOnMap();
}

void NuclearLiberation::loadLevel3()
{
	audio->stopCue(PEXP);
	state = GameState::L3;
	clearLevel();
	worldSize = Vector3(1000,400,0);
	player.setPosition(Vector3(25,100,0));
	invisibleWallLocation = 0;
	cameraTarget = player.getPosition();
	player.refillAir();
	initBackground();
	player.isActive = true;
	placeFinishLine();
	int which = 0;
	for(int i = 50; i < 500; i+=100)
	{
		switch(which%3)
		{
		case 0:
			spawnLightEnemy(Vector3(i+15,30*sin(2*PI*i/50)+50,0));
			break;
		case 1:
			spawnHeavyEnemy(Vector3(i+15,30*cos(2*PI*i/50)+50,0));
			break;
		case 2:
			spawnSplitEnemy(Vector3(i+10, 30*tan(2*PI*i/50)+50,0), 1);
			break;
		}

		which++;
	}

	spawnAllWallsOnMap();
}

void NuclearLiberation::onPlayerDeath()
{
	audio->playCue(PEXP);
	player.resetAll();
	menuLoad();
}

float NuclearLiberation::getFloor(float x)
{
	switch (state)
	{
	case MENU:
		return 0;
		break;
	case L1:
		if(x >= 150 && x < 275)
			return 30+(x-150);
		else if(x >= 275 && x < 400)
			return 155-(x-275);
		else
			return 5*(sin(2*PI*x/150.0)+2)+30;
		break;
	case L2:
		if(x >= 325 && x < 450)
			return 35+(x-325);
		else if(x >= 450 && x < 575)
			return 160 -(x-450);
		return 5*(sin(2*PI*x/150.0)+2)+30;
		break;
	case L3:
		if(x >=0 && x < 200)
			return 30 + x;
		else if(x >=200 && x<400)
			return 230 -(x-200);
		else if(x >= 425 && x < 550)
			return 30+1.5*(x -425);
		else if(x >= 550 && x < 675)
			return 217.5 -1.5*(x-550);
		else if(x >= 850)
			return 30 + (x-850);
		else
			return 5*(sin(2*PI*x/150.0)+2)+30;
		break;
	default:
		return 0;
		break;
	}
}

float NuclearLiberation::getCeiling(float x)
{
	switch (state)
	{
	case MENU:
		return worldSize.y;
		break;
	case L1:
		if(x >= 100 && x < 200)
			return worldSize.y-x+100;
		else if(x >= 200 && x < 300)
			return worldSize.y-100+(x-200);
		else
			return worldSize.y;
		break;
	case L2:
		if(x >= 0 && x < 175)
			return worldSize.y-x;
		else if(x >= 175 && x < 350)
			return worldSize.y-175+(x-175);
		else if(x >=600)
			return worldSize.y -(x-600);
		else
			return worldSize.y;
		break;
	case L3:
		if(x >= 150 && x < 400)
			return worldSize.y -(x-150);
		else if(x >= 400 && x < 650)
			return worldSize.y - 250 + (x -400);
		else if(x >= 850)
			return worldSize.y -(x-850);
		else
			return worldSize.y;
		break;
	default:
		return worldSize.y;
		break;
	}
}

void NuclearLiberation::spawnAllWallsOnMap()
{
	//20's are for bleed on edges of screen
	for(float i = -20;i<worldSize.x+20;i+=wallNS::WALL_SCALE)
	{
		for(float j = getCeiling(i)+wallNS::WALL_SCALE; j < worldSize.y; j+= wallNS::WALL_SCALE)
			spawnWall(Vector3(i,j,wallNS::WALL_SCALE));
		for(float j = getFloor(i)-wallNS::WALL_SCALE; j > 0; j-= wallNS::WALL_SCALE)
			spawnWall(Vector3(i,j,wallNS::WALL_SCALE));
	}
}

void NuclearLiberation::placeFinishLine()
{
	finishLine.setScale(Vector3(5,worldSize.y,1));
	finishLine.setPosition(Vector3(worldSize.x,worldSize.y/2,10));
	finishLine.isActive = true;
}

void NuclearLiberation::placeEnemyBoats(int numBoats)
{
	//need buffers from first and last
	float boatDisp = (worldSize.x - 20)/numBoats;

	for(float i = 10 ; i < worldSize.x; i+=boatDisp)
	{
		//remeber, enemy boat auto deletes itself if blocked
		spawnEnemyBoat(i);
	}
}
