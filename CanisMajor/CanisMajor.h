#pragma once

//#define DEBUG

#include "d3dApp.h"
#include "Geometry.h"
#include "Actor.h"
#include "Cube.h"
#include "Quad.h"
#include "Line.h"
#include "Origin.h"
#include "Coin.h"
#include "input.h"
#include <d3dx9math.h>
#include "Camera.h"
#include "Light.h"
#include "Flashlight.h"
#include "Key.h"
#include "Door.h"
#include "SearchableActor.h"

using std::wstring;

namespace CM{
	const int NUM_MENU_ITEMS = 3;//title, play, quit
	const int NUM_SPLASH_MENU_ITEMS = 3;//title, continue, quit
	const int MAX_LIGHTS = 4;
	const float TEXT_FADEOUT_TIME = 3;
	const int MAX_WALLS = 750;
	const int MAX_ROOF = 75;
	const int MAX_SCENERY = 1000;
	const Vector3 WALL_SCALE = Vector3(1,1.6,1);
	const float ROOF_SCALE = 3.8;
	const Vector3 BOOKCASE_SCALE = Vector3(2.5, 5, 1.4);
	const int MAX_KEYS=10;
	const int MAX_DOORS=100;
	const float INTERACTION_RADIUS_SQ=36;
	const int MAX_SEARCHABLE_ACTORS = 1000;
	const Vector3 CHAIR_SCALE = Vector3(2,2,1.7);
	const Vector3 BOX_SCALE = Vector3(2,2,2);
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

	void drawUtilText(wstring s=L"");//calling with s defined sets the string, calling without prints the string
	void drawStoryText();
	void updateStoryText(float dt);
	void setStoryText(float durration,wstring s, D3DXCOLOR c = WHITE);

	Camera& getCamera() {return camera;}

	GameState state;

	Controls controls;

	void onPlayerDeath();

	Vector3 worldSize;
	
	ID3D10Device* getDevice(){return md3dDevice;}

	Geometry mTelescope, mDresser, mFlashlight, mFrame, mBookcase, mChair, mCradle, mMasterbed, 
			mServantbed, mStaircase, mTable, mBottle, mLock, mPictureframe, mRail, mWallpanel, mWindow,
			mCage, mFixture, mDoor, mCube, mRoofHole,mKey, mBox;


	Origin origin;

	Flashlight flashlight;


	//EVERTHING PUBLIC BELOW THIS IS FOR TESTING
	Vector3 pos;

#ifdef DEBUG
	Geometry mRedCube;
	Actor AABBHelper;
#endif

	
	Light fLight;
	Light ambient;
	Light pLight;
	Light negaLight;
	Light rLights[CM::MAX_LIGHTS];		//Room Lights, point lights

	ID3D10EffectMatrixVariable* mfxWVPVar;
	ID3D10EffectMatrixVariable* mfxWorldVar;
	ID3D10EffectVariable* mfxEyePosVar;
	ID3D10EffectVariable* mfxLightVar;
	ID3D10EffectVariable* mfxPLightsVar;
	ID3D10EffectVariable* mfxPLightVar;
	ID3D10EffectVariable* mfxNegaLightVar;
	ID3D10EffectVariable* mfxAmbientVar;
	
	ID3D10EffectScalarVariable* mfxLightType;


	Actor* scenery;
	SearchableActor* searchableActors;
	Key keys[CM::MAX_KEYS];
	Door doors[CM::MAX_DOORS];
	Actor* spawnScenery(Geometry* g, Vector3 pos = Vector3(0,0,0), Vector3 rot = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1));
	Key* spawnKey(wstring name, Vector3 pos, Vector3 rot = Vector3(0,0,0));
	Door* spawnDoor(Vector3 pos, Vector3 rot=Vector3(0,0,0),Key* k = nullptr, bool isOpen = false);
	SearchableActor* spawnSearchable(Geometry* g, std::wstring name, Actor* in= nullptr, Vector3 pos = Vector3(0,0,0), Vector3 rot = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1));
private:

#ifdef DEBUG
	void updateDebugAABB(Actor* a);
#endif

	void buildFX();
	void buildVertexLayouts();

	//Origin origin;
	bool test;
	bool howl;

protected:

	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10InputLayout* mVertexLayout;


	D3DXMATRIX mView;
	D3DXMATRIX mProj;

	//Camera Object stuff
	Camera camera;

	std::wstring menuText[CM::NUM_MENU_ITEMS];
	int menuChoice;
	
	wstring storyText;
	float storyTextLifespan;
	float storyTextAge;
	D3DXCOLOR storyTextColor;

};