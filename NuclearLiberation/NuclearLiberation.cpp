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

	playerBullets = new Bullet[NL::MAX_PLAYER_BULLETS];
	walls = new Wall[NL::MAX_WALLS];
	enemyBullets = new Bullet[NL::MAX_ENEMY_BULLETS];
	enemyLight = new EnemyLight[NL::MAX_LIGHT_ENEMIES];
}

NuclearLiberation::~NuclearLiberation()
{
	if( md3dDevice )
		md3dDevice->ClearState();

	ReleaseCOM(mFX);
	ReleaseCOM(mVertexLayout);

	delete [] playerBullets;
	delete [] walls;
	delete [] enemyBullets;
	delete [] enemyLight;
}

void NuclearLiberation::initApp()
{
	D3DApp::initApp();

	origin.init(this,1);

	worldSize.x = 1000;
	worldSize.y = 1000;

	cameraDisplacement = Vector3(0,0,-75);
	cameraTarget = Vector3(0,0,0);

	//Cube cubeG,cubeR,cubeY,cubeW;
	//Line lineX, lineY, lineZ;

	cubeG.init(md3dDevice,GREEN);
	cubeR.init(md3dDevice,RED);
	cubeY.init(md3dDevice,YELLOW);
	cubeW.init(md3dDevice,WHITE);

	Controls c;
	c.up = 'W';
	c.down = 'S';
	c.left = 'A';
	c.right = 'D';
	c.fire = ' ';
	player.init(this,&cubeW,1,c);
	player.setScale(Vector3(2,1,1));
	player.setRadius(1);

	for(int i = 0 ; i < NL::MAX_PLAYER_BULLETS; i++)
	{
		playerBullets[i].init(this,&cubeG,1);
		playerBullets[i].setScale(Vector3(0.5,0.5,0.5));
		playerBullets[i].setRadius(1);
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

	for(int i = 0; i < NL::MAX_LIGHT_ENEMIES; i++)
	{
		enemyLight[i].init(this,&cubeR,2);
		enemyLight[i].setScale(Vector3(2,2,2));
	}

	buildFX();
	buildVertexLayouts();
	loadLevel1();
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
	player.update(dt);
	for(int i = 0 ; i < NL::MAX_PLAYER_BULLETS; i++)
	{
		playerBullets[i].update(dt);
	}
	for(int i = 0 ; i < NL::MAX_WALLS; i++)
	{
		walls[i].update(dt);
	}
	for(int i = 0; i < NL::MAX_LIGHT_ENEMIES; i++)
	{
		enemyLight[i].update(dt);
	}
	for(int i = 0; i < NL::MAX_ENEMY_BULLETS; i++)
	{
		enemyBullets[i].update(dt);
	}

	// Build the view matrix.

	D3DXVECTOR3 diff = player.getPosition() - cameraTarget;
	float dist = Length(&diff);
	Normalize(&diff,&diff);
	if(dist>NL::MAX_PLAYER_CENTER_DISTANCE)
	{
		diff*=(dist-NL::MAX_PLAYER_CENTER_DISTANCE);
		cameraTarget += diff;
	}
	//if(dist > 1)
	//{
	//	cameraTarget += diff*5*dt;
	//}

	D3DXVECTOR3 pos = cameraTarget+cameraDisplacement;
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&mView, &pos, &cameraTarget, &up);

	collisions();
}

void NuclearLiberation::collisions()
{
	for(int i = 0; i < NL::MAX_ENEMY_BULLETS; i++)
	{
		if(enemyBullets[i].collided(&player))
		{
			loadLevel1();
			break;
		}
	}

	for(int j = 0; j < NL::MAX_LIGHT_ENEMIES; j++)
	{
		for(int i = 0 ; i < NL::MAX_PLAYER_BULLETS; i++)
		{
			if(playerBullets[i].collided(&enemyLight[j]))
			{
				enemyLight[j].isActive = false;
				playerBullets[i].isActive = false;
				break;
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

	origin.draw(mfxWVPVar,mView,mProj,mTech);
	
	player.draw(mfxWVPVar,mView,mProj,mTech);

	for(int i = 0 ; i < NL::MAX_PLAYER_BULLETS; i++)
	{
		playerBullets[i].draw(mfxWVPVar,mView,mProj,mTech);
	}

	for(int i = 0 ; i < NL::MAX_WALLS; i++)
	{
		walls[i].draw(mfxWVPVar,mView,mProj,mTech);
	}

	for(int i = 0; i < NL::MAX_LIGHT_ENEMIES; i++)
	{
		enemyLight[i].draw(mfxWVPVar,mView,mProj,mTech);
	}
	for(int i = 0; i < NL::MAX_ENEMY_BULLETS; i++)
	{
		enemyBullets[i].draw(mfxWVPVar,mView,mProj,mTech);
	}


	mSwapChain->Present(0, 0);
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

void NuclearLiberation::spawnBullet(Vector3 pos, Vector3 vel)
{
	for(int i = 0; i<NL::MAX_PLAYER_BULLETS; i++)
	{
		if(!playerBullets[i].isActive)
		{
			playerBullets[i].create(pos);
			playerBullets[i].setVelocity(vel);
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
	for(int i = 0; i < NL::MAX_ENEMY_BULLETS; i++)
	{
		enemyBullets[i].isActive = false;
	}
}
void NuclearLiberation::loadLevel1()
{
	clearLevel();
	worldSize = Vector3(500,100,0);
	player.setPosition(Vector3(25,50,0));

	for(int i = 50; i < 500; i+=30)
	{
		spawnLightEnemy(Vector3(i+15,30*sin(2*PI*i/50)+50,0));
		spawnLightEnemy(Vector3(i,30*sin(2*PI*i/50)+60,0));
	}

	for(int i = -50; i < 550; i+=wallNS::WALL_SCALE)
	{
		float y = 5*(sin(2*PI*i/150.0)+2)-10;
		for(float j = y; j > -40; j-=wallNS::WALL_SCALE)
		{
			spawnWall(Vector3(i,j,wallNS::WALL_SCALE));
		}
	}
	
}
