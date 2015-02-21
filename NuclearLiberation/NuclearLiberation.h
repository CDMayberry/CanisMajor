#pragma once

#include "d3dApp.h"
#include "Geometry.h"
#include "Actor.h"
#include "Player.h"
#include "Cube.h"
#include "Wall.h"
#include <d3dx9math.h>

namespace NL{

	const int MAX_PLAYER_BULLETS = 1000;
	const int MAX_WALLS = 1000;
};

class NuclearLiberation : public D3DApp{

public:
	NuclearLiberation(HINSTANCE hInstance);
	~NuclearLiberation();

	void initApp();
	void onResize();
	void updateScene(float dt);
	void drawScene(); 


	void spawnBullet(Vector3 pos, Vector3 vel);

	Vector3 worldSize;

private:
	void buildFX();
	void buildVertexLayouts();

protected:

	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10InputLayout* mVertexLayout;
	ID3D10EffectMatrixVariable* mfxWVPVar;


	D3DXMATRIX mView;
	D3DXMATRIX mProj;

	Vector3 cameraDisplacement;

	

	//geometry
	Cube cube;


	//game objects
	Player player;
	Actor playerBullets[NL::MAX_PLAYER_BULLETS];
	Wall walls[NL::MAX_WALLS];


};