#pragma once

#include "d3dApp.h"
#include "Geometry.h"
#include "Actor.h"
#include "Player.h"
#include "Cube.h"
#include "Quad.h"
#include "Line.h"
#include "Origin.h"
#include "Coin.h"
#include "input.h"
#include <d3dx9math.h>
#include "Camera.h"
#include "Light.h"

namespace NL{
	const int NUM_MENU_ITEMS = 3;//title, play, quit
	const int NUM_SPLASH_MENU_ITEMS = 3;//title, continue, quit
};

enum GameState{
	MENU,
	ATTIC,
	SECOND_FLOOR,
	FIRST_FLOOR,
	BASEMENT,
	SPLASH
};

class CanisMajor : public D3DApp{

public:
	CanisMajor(HINSTANCE hInstance);
	~CanisMajor();

	void initApp();
	void initBackground();
	void onResize();
	void updateScene(float dt);
	void drawScene(); 
	void collisions();

	void menuLoad();
	void menuUpdate(float dt,bool reset = false);
	void menuDraw();

	void loadSplashScreen(bool status);
	void splashDraw();

	void clearLevel();
	void levelsUpdate(float dt);
	void levelsDraw();

	void resetLevel();
	void loadAttic();
	void loadSecondFloor();
	void loadFirstFloor();
	void loadBasement();

	GameState state;


	void onPlayerDeath();

	Vector3 worldSize;
	
	Player player;

	ID3D10Device* getDevice(){return md3dDevice;}

	Geometry mTelescope, mDresser, mFlashlight, mFrame, mBookcase, mChair, mCradle, mMasterbed, 
			mServantbed, mStaircase, mTable, mBottle, mLock, mPictureframe, mRail, mWallpanel, mWindow,
			mCage, mFixture, mDoor, mCube;
	Cube qFloor;
	Actor telescope, dresser,flashlight,frame, bookcase, chair, cradle, masterbed, 
			servantbed, staircase, table, bottle, lock, pictureFrame, rail, wallPanel, window,
			cage, fixture, door, floor, cube;

	Origin origin;


	//EVERTHING PUBLIC BELOW THIS IS FOR TESTING
	Vector3 pos;

	
	Light mLights[3];
	int mLightType; // 0 (parallel), 1 (point), 2 (spot)

	ID3D10EffectMatrixVariable* mfxWVPVar;
	ID3D10EffectMatrixVariable* mfxWorldVar;
	ID3D10EffectVariable* mfxEyePosVar;
	ID3D10EffectVariable* mfxLightVar;
	
	ID3D10EffectScalarVariable* mfxLightType;

private:
	void buildFX();
	void buildVertexLayouts();

	//Origin origin;
	bool test;

protected:

	void spawnAllWallsOnMap();//Justin
	void placeFinishLine();//Justin
	void placeEnemyBoats(int numBoats);//Justin

	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10InputLayout* mVertexLayout;


	D3DXMATRIX mView;
	D3DXMATRIX mProj;

	Vector3 cameraDisplacement;
	Vector3 cameraTarget, cameraPositon, cameraUp;

	// Camera stuff
	Vector3 cameraPos;
	Vector3 lookAt;


	//Camera Object stuff
	Camera camera;

	std::wstring menuText[NL::NUM_MENU_ITEMS];
	int menuChoice;
	
};