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

namespace CameraNS
{
	const float NEAR_CLIPPING_DIST = 1;
	const float FAR_CLIPPING_DIST = 100;//originally 1000
	const float SPEED = 10;
	const float MOUSE_MOVE_RATE = 0.001;
};

class Camera: public virtual Actor
{
public:
	Camera();
	~Camera();
	void init(CanisMajor* game, Controls c);
	void create(Vector3 pos, Vector3 dir);

	Matrix getViewMatrix() {return mView;}
	Matrix getProjectionMatrix() {return mProj;}
	void update(float dt);
	void setPosition(Vector3 pos) {position = pos;}
	Vector3 getPosition() {return position;}
	void setDirection(Vector3 dir) {direction = dir;}
	Vector3 getDirection() {return direction;}
	float getSpeed () {return speed;}
	void setSpeed(float s) {speed = s;}
	float getFoV() {return FoV;}
	void setFoV(float fov){FoV = fov;}
	void setPerspective();
	void setPitch(float p) {pitch = p;}
	void setLight(Light* light) {flashlight = light;}
private:
	Matrix mView;
	Matrix mProj;
	Vector3 position;
	Vector3 direction;
	float speed;
	float aspectRatio;
	float FoV;
	float nearClippingPlane;
	float farClippingPlane;
	Vector3 up;
	Vector3 right;
	float yaw;
	float pitch;
	float roll;
	float mPhi;
	float mTheta;
	Light* flashlight;
	Controls controls;
};
#endif