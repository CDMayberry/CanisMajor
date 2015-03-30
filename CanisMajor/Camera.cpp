#include "Camera.h"
#include "CanisMajor.h"

using namespace CameraNS;

#define Transform D3DXVec3TransformCoord
Camera::Camera()
{
	keys = new Key*[CM::MAX_KEYS];
	speed = 10;
	FoV = 0.5*PI;
	aspectRatio = 480.0f/640.0f;
	nearClippingPlane = NEAR_CLIPPING_DIST;
	farClippingPlane = FAR_CLIPPING_DIST;
	up = Vector3(0.0f, 1.0f, 0.0f);
	position = Vector3(0,0,0);
	direction = Vector3(0.0f, 0.0f, 0.0f);
	bobTimer = 0;
	flashlight=nullptr;
	nearbyItem = nullptr;
	// register handler for relative mouse movement events
	//Windows::Devices::Input::MouseDevice::GetForCurrentView()->MouseMoved += ref new TypedEventHandler<MouseDevice^, MouseEventArgs^>(this, &MoveLookController::OnMouseMoved);
}

Camera::~Camera()
{
	delete [] keys;
}

void Camera::init(CanisMajor* game,Geometry* geo, Controls c)
{
	Actor::init(game,geo,COLISION_RADIUS);
	controls = c;
	camHeight = DEFAULT_HEIGHT;
	isActive = true;
	isVisible = false;

	collisionType = AABBox;
	setScale(Vector3(.7,1.5,.7));

	for(int i = 0 ; i < CM::MAX_KEYS; i++)
	{
		keys[i]=nullptr;
	}
}

void Camera::create(Vector3 pos, Vector3 dir)
{
	position = pos;
	direction = dir;
}

void Camera::setPerspective()
{
	aspectRatio = (float)game->mClientWidth/game->mClientHeight;
	D3DXMatrixPerspectiveFovLH(&mProj, FoV, aspectRatio, nearClippingPlane,farClippingPlane); 
}
void Camera::update(float dt)
{
	prevLoc = getPosition();

	Vector3 forward = direction;
	forward.y=0;

	Vector3 right;

	D3DXVec3Cross(&right,&up,&forward);

	Normalize(&right,&right);
	Normalize(&forward,&forward);

	Vector3 input(0,0,0);

	bool walkingBackwards = false;

	if(GetAsyncKeyState(controls.up))
	{
		input += forward;
		if(bobTimer <= 0)
			game->audio->playCue(WALK1);
	}
	if(GetAsyncKeyState(controls.down))
	{
		input -= forward;
		walkingBackwards = true;
	}
	if(GetAsyncKeyState(controls.left))
	{
		input -= right;
	}
	if(GetAsyncKeyState(controls.right))
	{
		input += right;
	}

	Normalize(&input,&input);

	if(walkingBackwards)
	{
		input/=2;
	}

	if(bobTimer < 0 && GetAsyncKeyState(controls.run));

	if(D3DXVec3LengthSq(&input)>0)
	{
		bobTimer+=dt;
		if(bobTimer>PI/BOB_FREQUENCY) bobTimer-=PI/BOB_FREQUENCY;
	}
	else{
		bobTimer= max(bobTimer-10*dt,0);
	}

	if(GetAsyncKeyState(controls.crouch))
	{
		position += CROUCH_SPEED*input*dt;
		camHeight = max(CROUCH_HEIGHT,camHeight-dt*SQUAT_SPEED);
		position.y = camHeight + CROUCH_BOB_AMPLITUDE*sin(BOB_FREQUENCY*bobTimer);

	}
	else 
	{
		camHeight = min(DEFAULT_HEIGHT,camHeight+dt*SQUAT_SPEED);
		if(GetAsyncKeyState(controls.run))
		{
			position+=RUN_SPEED*input*dt;
			position.y = camHeight + RUN_BOB_AMPLITUDE*sin(BOB_FREQUENCY*bobTimer);
		}
		else
		{
			position+=SPEED*input*dt;
			position.y = camHeight + DEFAULT_BOB_AMPLITUDE*sin(BOB_FREQUENCY*bobTimer);
		}
	}

	Vector3 currentMouseState(game->input->getMouseX(),game->input->getMouseY(),0);
	Vector3 mouseDiff = currentMouseState - game->getDefaultRelativeMousePosition();

	mouseDiff*=-MOUSE_MOVE_RATE;

	direction.y += mouseDiff.y;

	direction = rotateY(direction,-mouseDiff.x);

	Normalize(&direction,&direction);

	Vector3 lookAt = position + direction;

	static bool buttonPushed = false;

	if(GetAsyncKeyState(controls.use)&&nearbyItem!=nullptr)
	{
		if(!buttonPushed)
		{
			nearbyItem->interactWith(this);
			buttonPushed = true;
		}
	}
	else if(flashlight!=nullptr){
		if(GetAsyncKeyState(controls.flashlight) )
		{
			if(!buttonPushed)
			{
				flashlight->toggle();
				game->audio->playCue(CLICK);
				buttonPushed = true;
			}
		}
		else
			buttonPushed = false;
		flashlight->setPosition(position-0.3*up+0.9*forward-0.3*right);
		flashlight->setDirection(direction);
	}
	else
	{
		buttonPushed = false;
	}
	
	nearbyItem=nullptr;

	//Generate new matrix
	D3DXMatrixLookAtLH(&mView, &position, &lookAt, &up);
}

bool Camera::collided(Actor *gameObject)
{

	float y = position.y/2;
	position.y-=y;
	bool r = Actor::collided(gameObject);
	position.y+=y;
	return r;
}

void Camera::addKey(Key* k)
{
	for(int i = 0 ; i < CM::MAX_KEYS; i++)
	{
		if(keys[i]==nullptr)
		{
			keys[i] = k;
		}
	}
}
bool Camera::checkKey(Key* k)
{
	for(int i = 0 ; i < CM::MAX_KEYS; i++)
	{
		if(keys[i]==k)
			return true;
	}
	return false;
}
void Camera::removeKey(Key* k)
{
	for(int i = 0 ; i < CM::MAX_KEYS; i++)
	{
		if(keys[i]==k)
		{
			keys[i]=nullptr;
		}
	}
}
