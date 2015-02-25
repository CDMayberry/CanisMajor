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
}

NuclearLiberation::~NuclearLiberation()
{
	if( md3dDevice )
		md3dDevice->ClearState();

	ReleaseCOM(mFX);
	ReleaseCOM(mVertexLayout);

	delete [] playerBullets;
	delete [] walls;
}

void NuclearLiberation::initApp()
{
	D3DApp::initApp();

	worldSize.x = 1000;
	worldSize.y = 1000;

	cameraDisplacement = Vector3(0,0,-50);
	cameraTarget = Vector3(0,0,0);

	cube.init(md3dDevice);
	quad.init(md3dDevice);
	rockA.init(md3dDevice,"rockA.geo");
	bullet.init(md3dDevice,"torpedo.geo");
	sub.init(md3dDevice,"sub.geo");

	Controls c;
	c.up = 'W';
	c.down = 'S';
	c.left = 'A';
	c.right = 'D';
	c.fire = ' ';
	player.init(this,&sub,1,c);
	player.setScale(Vector3(2,2,2));

	for(int i = 0 ; i < NL::MAX_PLAYER_BULLETS; i++)
	{
		playerBullets[i].init(this,&bullet,1);
		playerBullets[i].setScale(Vector3(1,1,1));
	}

	for(int i = 0 ; i < NL::MAX_WALLS; i++)
	{
		walls[i].init(this,&rockA,1);
		walls[i].setPosition(Vector3(rand()%1000,rand()%1000,0));
		walls[i].isActive = true;
		walls[i].setScale(Vector3(2,2,1));
	}

	buildFX();
	buildVertexLayouts();
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

	
	player.draw(mfxWVPVar,mView,mProj,mTech);

	for(int i = 0 ; i < NL::MAX_PLAYER_BULLETS; i++)
	{
		playerBullets[i].draw(mfxWVPVar,mView,mProj,mTech);
	}

	for(int i = 0 ; i < NL::MAX_WALLS; i++)
	{
		walls[i].draw(mfxWVPVar,mView,mProj,mTech);
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
