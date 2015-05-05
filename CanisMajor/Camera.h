#ifndef _CAMERA_H                // Prevent multiple definitions if this 
#define _CAMERA_H 

#include "constants.h"
#include "d3dUtil.h"
#include "d3dApp.h"
#include <d3dx9math.h>
#include "input.h"
#include "Light.h"
#include "Actor.h"
#include "Controls.h"
#include "mathUtil.h"
#include "Flashlight.h"
#include "Interactable.h"
#include "QuestItem.h"
#include "Staff.h"

namespace CameraNS
{
	const float INTERACTION_RADIUS_SQ = 64;
	const float NEAR_CLIPPING_DIST = 0.1;
	const float FAR_CLIPPING_DIST = 150;//originally 1000
	const float SPEED = 10;
	const float RUN_SPEED = 30;
	const float CROUCH_SPEED = 5;
	const float MOUSE_MOVE_RATE = 0.001;
	const float DEFAULT_BOB_AMPLITUDE = 0.3;
	const float RUN_BOB_AMPLITUDE = 0.6;
	const float CROUCH_BOB_AMPLITUDE = 0.3;
	const float DEFAULT_HEIGHT = 3;
	const float CROUCH_HEIGHT = 1;
	const float BOB_FREQUENCY = 6;//bobs per 2PI
	const float SQUAT_SPEED = 10;
	const float COLISION_RADIUS = 3;
	const float RUN_TIME = .598;
	const float WALK_TIME = .340;
	const float SHAKE_SPEED = 8;
};

class Camera: public virtual Actor
{
public:
	Camera();
	~Camera();

	void reset();

	//Geo used for AABB collision
	void init(CanisMajor* game,Geometry* geo, Controls c);
	void create(Vector3 pos, Vector3 dir);
	void update(float dt);

	void clearInventory();

	virtual bool collided(Actor *gameObject);

	bool hasFlashlight(){return flashlight!=nullptr;};
	bool hasStaff(){return staff!=nullptr;}

	bool isPicked(Actor* o, float &distance);
	void setNearbyInteractable(Interactable* i, float dist);
	void resetNearbyInteractable();
	Interactable* getNearbyItem(){return nearbyItem;}

	Matrix getViewMatrix() {return mView;}
	Matrix getProjectionMatrix() {return mProj;}
	
	void setPosition(Vector3 pos) {position = pos;prevLoc = pos;}
	Vector3 getPosition() {return position;}
	void setDirection(Vector3 dir) {direction = dir;}
	Vector3 getDirection() {return direction;}
	float getSpeed () {return speed;}
	void setSpeed(float s) {speed = s;}
	float getFoV() {return FoV;}
	void setFoV(float fov){FoV = fov;}
	void setPerspective();
	void setFlashlight(Flashlight* f) {flashlight = f;}
	void setStaff(Staff* f) {staff = f;}
	void addItem(QuestItem* k);
	bool checkItem(QuestItem* k);
	void removeItem(QuestItem* k);

	void backUp(){position=prevLoc;}	

	Controls controls;

private:

	Matrix mView;
	Matrix mProj;
	Vector3 direction;
	float speed;
	float aspectRatio;
	float FoV;
	float nearClippingPlane;
	float farClippingPlane;
	Vector3 up;
	Vector3 right;
	float bobTimer, walkTimer, shakeTimer;
	Flashlight* flashlight;
	Staff* staff;

	float camHeight;
	float flashHeight;
	Interactable* nearbyItem;
	float distToInteractable;
	Vector3 prevLoc;
	QuestItem** items;

};
#endif