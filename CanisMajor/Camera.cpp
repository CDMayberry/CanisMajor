#include "Camera.h"
#include "CanisMajor.h"

using namespace CameraNS;

#define Transform D3DXVec3TransformCoord
Camera::Camera()
{
	speed = 10;
	FoV = 0.25*PI;
	aspectRatio = 480.0f/640.0f;
	nearClippingPlane = NEAR_CLIPPING_DIST;
	farClippingPlane = FAR_CLIPPING_DIST;
	up = Vector3(0.0f, 1.0f, 0.0f);
	position = Vector3(0,0,0);
	direction = Vector3(0.0f, 0.0f, 0.0f);
	yaw = 0;
	roll = 0;
	pitch = 0;
	mPhi = 0;
	mTheta = 0;
	// register handler for relative mouse movement events
	//Windows::Devices::Input::MouseDevice::GetForCurrentView()->MouseMoved += ref new TypedEventHandler<MouseDevice^, MouseEventArgs^>(this, &MoveLookController::OnMouseMoved);
}

Camera::~Camera()
{
	//nothing to deallocate
}

void Camera::init(CanisMajor* g, Controls c)
{
	Actor::init(g,nullptr,1);
	controls = c;
}

void Camera::create(Vector3 pos, Vector3 dir)
{
	position = pos;
	direction = dir;
}

void Camera::setPerspective()
{
	D3DXMatrixPerspectiveFovLH(&mProj, FoV, aspectRatio, nearClippingPlane,farClippingPlane); 
}
void Camera::update(float dt)
{
	Vector3 forward = direction;
	forward.y=0;

	Vector3 right;

	D3DXVec3Cross(&right,&up,&forward);

	Normalize(&right,&right);
	Normalize(&forward,&forward);

	Vector3 input(0,0,0);

	if(GetAsyncKeyState(controls.up))
	{
		input += forward;
	}
	if(GetAsyncKeyState(controls.down))
	{
		input -= forward;
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

	position+=SPEED*input*dt;

	Vector3 currentMouseState(game->input->getMouseX(),game->input->getMouseY(),0);
	Vector3 mouseDiff = currentMouseState - game->getDefaultRelativeMousePosition();

	mouseDiff*=-MOUSE_MOVE_RATE;

	direction.y += mouseDiff.y;

	direction = rotateY(direction,-mouseDiff.x);

	Normalize(&direction,&direction);

	Vector3 lookAt = position + direction;

	flashlight->dir = direction;

	//bool rotated = false;
	//bool pitched = false;
	//float deltaYaw = 0;
	//float _speed = 1;
	//float deltaPitch = 0;

	//Vector3 direction = Vector3(0,0,0);
	//D3DXVECTOR3 rotDirection = Vector3(0,0,0);
	//Matrix yawR;
	//Matrix pitchR;
	//Matrix rollR;
	//
	//Identity(&yawR);
	//Identity(&pitchR);
	//Identity(&rollR);

	//if(GetAsyncKeyState(VK_RIGHT) & 0x8000)
	//{
	//	deltaYaw += _speed*dt;
	//	yaw += deltaYaw;
	//	rotated = true;
	//}
	//if(GetAsyncKeyState(VK_LEFT) & 0x8000)
	//{
	//	rotated = true;
	//	deltaYaw -= _speed*dt;
	//	yaw+= deltaYaw;
	//}
	////if (GetAsyncKeyState(VK_UP) & 0x8000)
	////{
	////	pitched = true;
	////	deltaPitch += _speed*dt;
	////	if (deltaPitch < 1) 
	////		deltaPitch = 1;
	////	pitch += deltaPitch;
	////}
	////if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	////{
	////	pitched = true;
	////	deltaPitch -= _speed*dt;
	////	if (deltaPitch < -1) 
	////		deltaPitch = -1;
	////	pitch += deltaPitch;

	////}
	////RotateY(&yawR, ToRadian(yaw));

	//if(GetAsyncKeyState('A') & 0x8000)
	//		direction.z = 1;
	//if(GetAsyncKeyState('D') & 0x8000)
	//		direction.z = -1;
	//if(GetAsyncKeyState('S') & 0x8000)
	//		direction.x = -1;
	//if(GetAsyncKeyState('W') & 0x8000)
	//		direction.x = 1;
	////if(GetAsyncKeyState(' ') & 0x8000)
	////	direction.y = 1;
	//if(GetAsyncKeyState(VK_CONTROL) & 0x8000)
	//	position.y = -1;
	//else
	//	position.y = 0;
	//

	////Generate rotation matrices
	//D3DXMatrixRotationY(&yawR,yaw);
	//D3DXMatrixRotationX(&pitchR,pitch);
	//
	//
	////Update position
	//direction = direction *dt*speed;
	//setPosition(position+direction);
	//lookAt+=direction;

	////Update LookAt
	//if (rotated)
	//{
	//	Vector3 transformed = Vector3(1,0,0);
	//	Transform(&transformed,&transformed,&yawR);
	//	D3DXVec3Normalize(&transformed,&transformed);
	//	lookAt = transformed;
	//	flashlight->dir = lookAt;
	//	lookAt+=position;
	//	rotated = false;
	//	//this->direction = lookAt;
	//}
	//else{
	//
	//}

	//if(pitched)
	//{
	//	Vector3 transformedX = Vector3(0,1,0);
	//	Transform(&transformedX,&transformedX,&pitchR);
	//	D3DXVec3Normalize(&transformedX,&transformedX);
	//	lookAt = transformedX;
	//	lookAt+=position;
	//	pitched = false;
	//	//this->direction = lookAt;
	//}
	//this->direction = lookAt;
	
	//Generate new matrix
	D3DXMatrixLookAtLH(&mView, &position, &lookAt, &up);
}
