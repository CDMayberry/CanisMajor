#pragma once

#include "d3dApp.h"
#include "Geometry.h"
#include "Actor.h"
#include "Player.h"
#include "Cube.h"
#include "Wall.h"
#include "Quad.h"
#include "Bullet.h"
#include "Line.h"
#include "EnemyLight.h"
#include "EnemyHeavy.h"
#include "EnemySplit.h"
#include "Origin.h"
#include <d3dx9math.h>
#include "Background.h"

namespace NL{

	const int MAX_PLAYER_BULLETS = 500;
	const int MAX_ENEMY_BULLETS = 500;
	const int MAX_WALLS = 2000;
	const int MAX_LIGHT_ENEMIES = 500;
	const int MAX_HEAVY_ENEMIES = 500;
	const int MAX_SPLIT_ENEMEIS = 500;
	const float MAX_PLAYER_CENTER_DISTANCE = 15;
	const float PRECEIVED_SCREEN_WIDTH = 70;
	const float PRECEIVED_SCREEN_HEIGHT = 50;
	const float MIN_SCROLL_SPEED = 3;
	const int MAX_BACK = 3;
};

class NuclearLiberation : public D3DApp{

public:
	NuclearLiberation(HINSTANCE hInstance);
	~NuclearLiberation();

	void initApp();
	void onResize();
	void updateScene(float dt);
	void drawScene(); 
	void collisions();

	void spawnBullet(Vector3 pos, Vector3 vel);
	void spawnEnemyBullet(Vector3 pos, Vector3 vel);
	void spawnLightEnemy(Vector3 pos);
	void spawnHeavyEnemy(Vector3 pos);
	void spawnSplitEnemy(Vector3 pos);
	void spawnWall(Vector3 pos);

	void checkEnemySplit();

	Vector3 worldSize;
	
	Player player;

	ID3D10Device* getDevice(){return md3dDevice;}

	float minPlayerPosition;

private:
	void buildFX();
	void buildVertexLayouts();

	Origin origin;
	Background backgd;
	Background backgd1;
	Background backgd2;
	Background backgd3;

protected:

	void clearLevel();
	void loadLevel1();

	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10InputLayout* mVertexLayout;
	ID3D10EffectMatrixVariable* mfxWVPVar;


	D3DXMATRIX mView;
	D3DXMATRIX mProj;

	Vector3 cameraDisplacement;
	Vector3 cameraTarget, cameraPositon, cameraUp;
	
	

	//geometry
	Cube cubeG,cubeR,cubeY,cubeW;
	Line lineX, lineY, lineZ;
	Quad quad;

	//game objects
	Bullet* playerBullets;
	Wall* walls;
	
	Bullet* enemyBullets;
	EnemyLight * enemyLight;
	EnemyHeavy * enemyHeavy;
	EnemySplit * enemySplit;

};