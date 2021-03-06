#pragma once

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
#include "QuestItem.h"
#include "Door.h"
#include "SearchableActor.h"
#include "ReadableActor.h"
#include "sharedDefines.h"
#include "Staircase.h"
#include "Dog.h"
#include "GameState.h"
#include <chrono>
#include "Pedestal.h"
#include "Sky.h"
#include "GUI.h"
#include "Sprite.h"
#include "SlidingBookcase.h"

using std::wstring;

namespace CM{
	const int NUM_MENU_ITEMS = 6;//title, play, quit, "", instructions 1/2
	const int NUM_SPLASH_MENU_ITEMS = 3;//title, continue, quit
	const float TEXT_FADEOUT_TIME = 3;
	const int MAX_ROOF = 75;
	const int MAX_SCENERY = 1000;
	const Vector3 WALL_SCALE = Vector3(1,1.6,1);
	const Vector3 WALL_SCALE2 = Vector3(1,1.2,1);
	const float ROOF_SCALE = 3.8f;
	const Vector3 BOOKCASE_SCALE = Vector3(2.5, 5, 2);
	const int NUM_QUEST_ITEMS=100;
	const int MAX_DOORS=100;
	const int MAX_STAIRCASES = 10;
	const float INTERACTION_RADIUS_SQ=36.0f;
	const int MAX_SEARCHABLE_ACTORS = 1000;
	const int MAX_READABLE_ACTORS = 100;
	const Vector3 CHAIR_SCALE = Vector3(2,2,1.7);
	const Vector3 BOX_SCALE = Vector3(2,2,2);
	const Vector3 CABINET_DOOR = Vector3(1,1.45,1.35);
	const Vector3 CABINET_DOOR2 = Vector3(1,1.4,1.5);
	const int NUM_SPRITES = 1000;
	const int NUM_BOLTS = 100;
	const Vector3 WINE_GLASS = Vector3(.25,.25,.25);	
	const int NUM_WALLS = 100;
};



class CanisMajor : public D3DApp{

public:
	CanisMajor(HINSTANCE hInstance);
	~CanisMajor();

	void initApp();
	void threadInit();
	
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
	void setStoryText(float duration,wstring s, D3DXCOLOR c = WHITE);

	void drawNoteText();
	void updateNoteText(float dt);
	void setNoteText(float duration,wstring s, D3DXCOLOR c = WHITE);

	Camera& getCamera() {return camera;}

	GameState state;

	Controls controls;

	void onPlayerDeath();

	Vector3 worldSize;
	
	ID3D10Device* getDevice(){return md3dDevice;}

	Geometry mTelescope, mDresser, mFlashlight, mFrame, mBookcase, mChair, mCradle, mMasterbed, 
			mServantbed, mStaircase, mTable, mBottle, mLock, mPictureframe, mRail, mWallpanel,
			mCage, mFixture, mDoor, mCube, mRoofHole,mKey, mBox, mWindowPanel, mBook, mBook2, mBook3, mBookStack, mDesk,
			mToilet, mSink, mTub, mArrow, mRing, mDog, mPedastal, mFloor,mStove, mCounter, mSinkCounter, 
			mCabinet, mCabDoor,mStaff,mSphere,mWineGlass,mTeaCup,mTeaSaucer,mSofa,mFireplace;


	Origin origin;

	Camera camera;

	Sky sky;

	clock_t start;
	clock_t current;

	bool waiting, reading;


	Flashlight flashlight;
	Staff staff;
	Dog dog;

	Pedestal pedestal;

	SlidingBookcase slidingBookcase;

	void playSound(const char* cue, Vector3 pos);

	//EVERTHING PUBLIC BELOW THIS IS FOR TESTING
	Vector3 pos;

	bool birminghamMode;

#if defined(_DEBUG) || defined(DEBUG)
	Geometry mRedCube;
	Actor AABBHelper;
#endif

	//TEXTURE RELATED ITEMS
	ID3D10ShaderResourceView* mDiffuseMapRV;
	ID3D10ShaderResourceView* mSpecMapRV;

	ID3D10EffectShaderResourceVariable* mfxDiffuseMapVar;
	ID3D10EffectShaderResourceVariable* mfxSpecMapVar;
	ID3D10EffectMatrixVariable* mfxTexMtxVar;
	ID3D10Effect* skyFX;

	GUI gui;
	Sprite sprite;
	Sprite trees;
	
	Light fLight;
	Light ambient;
	Light pLight;
	Light negaLight;
	Light rLights[MAX_LIGHTS];		//Room Lights, point lights
	Light eyes;
	int activeLights;
	int lightType[MAX_LIGHTS];
	

	ID3D10EffectMatrixVariable* mfxWVPVar;
	ID3D10EffectMatrixVariable* mfxWorldVar;
	ID3D10EffectVariable* mfxEyePosVar;
	ID3D10EffectVariable* mfxLightVar;
	ID3D10EffectVariable* mfxPLightsVar[MAX_LIGHTS];
	ID3D10EffectVariable* mfxPLightVar;
	ID3D10EffectVariable* mfxNegaLightVar;
	ID3D10EffectVariable* mfxEyesVar;
	ID3D10EffectVariable* mfxAmbientVar;
	
	ID3D10EffectScalarVariable* mfxLightBool;
	ID3D10EffectScalarVariable* mfxHighlightBool;
	ID3D10EffectScalarVariable* mfxActiveLights;
	ID3D10EffectScalarVariable* mfxLightType[MAX_LIGHTS];

	
	Actor* walls[CM::NUM_WALLS];
	Actor* scenery;
	SearchableActor* searchableActors;
	ReadableActor* readableActors;
	QuestItem items[CM::NUM_QUEST_ITEMS];
	Door doors[CM::MAX_DOORS];
	Staircase staircases[CM::MAX_STAIRCASES];
	Actor* spawnScenery(Geometry* g, Vector3 pos = Vector3(0,0,0), Vector3 rot = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1));
	QuestItem* spawnQuestItem(Geometry* g, wstring name, Vector3 pos, Vector3 rot = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1));
	Door* spawnDoor(Vector3 pos, Vector3 rot=Vector3(0,0,0), Vector3 Scale=Vector3(1,1,1), QuestItem* k = nullptr, bool isOpen = false);
	SearchableActor* spawnSearchable(Geometry* g, std::wstring name, Actor* in= nullptr, Vector3 pos = Vector3(0,0,0), Vector3 rot = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1), const char* cue = "", bool playOnce = false);
	ReadableActor* spawnReadable(Geometry* g, std::wstring name, Actor* in= nullptr, Vector3 pos = Vector3(0,0,0), Vector3 rot = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1), wstring text = L"The note is scratched and unreadable.", float dur = 2.0f, const char* cue = "", bool playOnce = false);
	Light* spawnLight(Vector3 pos, int type = 0);
	Light* spawnLight(Vector3 pos, Vector3 dir, int type = 0);
	Staircase* spawnStaircase(std::wstring name, LLevel func, Vector3 pos, Vector3 rotation = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1));
	Sprite* spawnSprites(const D3DXVECTOR3 centers[], UINT numSprites, std::wstring filenames[], UINT numFiles);
	Sprite* spawnSprite(const D3DXVECTOR3 center, std::wstring filename);
	Sprite sprites[CM::NUM_SPRITES];
	Actor bolts[CM::NUM_BOLTS];
	Actor* spawnBolt(Vector3 pos, Vector3 vel);

#if defined(_DEBUG) || defined(DEBUG)
	void updateDebugAABB(Actor* a);
#endif

private:
	void buildFX();
	void buildVertexLayouts();

	//Origin origin;
	bool test;
	bool howl;

	int loadingStatus;
	HANDLE loadingThread;
	bool threadComplete;

protected:

	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10InputLayout* mVertexLayout;


	D3DXMATRIX mView;
	D3DXMATRIX mProj;

	//Camera Object stuff
	

	std::wstring menuText[CM::NUM_MENU_ITEMS];
	int menuChoice;
	
	wstring storyText;
	float storyTextLifespan;
	float storyTextAge;
	D3DXCOLOR storyTextColor;

	wstring noteText;
	float noteTextLifespan;
	float noteTextAge;
	D3DXCOLOR noteTextColor;

};