#include "Camera.h"

#define Transform D3DXVec3TransformCoord
Camera::Camera()
{
	speed = 10;
	FoV = 0.25*PI;
	aspectRatio = 480.0f/640.0f;
	nearClippingPlane = 1.0f;
	farClippingPlane = 1000.0f;
	up = Vector3(0.0f, 1.0f, 0.0f);
	position = Vector3(-10,0,0);
	lookAt = Vector3(0.0f, 0.0f, 0.0f);
	direction = Vector3(0.0f, 0.0f, 0.0f);
	yaw = 0;
	roll = 0;
	pitch = 0;
	// register handler for relative mouse movement events
	//Windows::Devices::Input::MouseDevice::GetForCurrentView()->MouseMoved += ref new TypedEventHandler<MouseDevice^, MouseEventArgs^>(this, &MoveLookController::OnMouseMoved);
}

Camera::~Camera()
{
	//nothing to deallocate
}

void Camera::init(Vector3 position, Vector3 direction, Vector3 _lookAt)
{
}

void Camera::setPerspective()
{
	D3DXMatrixPerspectiveFovLH(&mProj, FoV, aspectRatio, nearClippingPlane,farClippingPlane); 
}
void Camera::update(float dt)
{
	bool rotated = false;
	bool pitched = false;
	float deltaYaw = 0;
	float _speed = 1;
	float deltaPitch = 0;

	Vector3 direction = Vector3(0,0,0);
	D3DXVECTOR3 rotDirection = Vector3(0,0,0);
	Matrix yawR;
	Matrix pitchR;
	Matrix rollR;
	
	Identity(&yawR);
	Identity(&pitchR);
	Identity(&rollR);

	if(GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		deltaYaw += _speed*dt;
		yaw += deltaYaw;
		rotated = true;
	}
	if(GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		rotated = true;
		deltaYaw -= _speed*dt;
		yaw+= deltaYaw;
	}
	//if (GetAsyncKeyState(VK_UP) & 0x8000)
	//{
	//	pitched = true;
	//	deltaPitch += _speed*dt;
	//	if (deltaPitch < 1) 
	//		deltaPitch = 1;
	//	pitch += deltaPitch;
	//}
	//if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	//{
	//	pitched = true;
	//	deltaPitch -= _speed*dt;
	//	if (deltaPitch < -1) 
	//		deltaPitch = -1;
	//	pitch += deltaPitch;

	//}
	//RotateY(&yawR, ToRadian(yaw));

	if(GetAsyncKeyState('A') & 0x8000)
			direction.z = 1;
	if(GetAsyncKeyState('D') & 0x8000)
			direction.z = -1;
	if(GetAsyncKeyState('S') & 0x8000)
			direction.x = -1;
	if(GetAsyncKeyState('W') & 0x8000)
			direction.x = 1;
	if(GetAsyncKeyState(' ') & 0x8000)
		direction.y = 1;
	if(GetAsyncKeyState(VK_CONTROL) & 0x8000)
		direction.y = -1;
	
	//Generate rotation matrices
	D3DXMatrixRotationY(&yawR,yaw);
	D3DXMatrixRotationX(&pitchR,pitch);
	
	
	//Update position
	direction = direction *dt*speed;
	setPosition(position+direction);
	lookAt+=direction;

	//Update LookAt
	if (rotated)
	{
		Vector3 transformed = Vector3(1,0,0);
		Transform(&transformed,&transformed,&yawR);
		D3DXVec3Normalize(&transformed,&transformed);
		lookAt = transformed;
		lookAt+=position;
		rotated = false;
		//this->direction = lookAt;
	}
	else{
	
	}

	if(pitched)
	{
		Vector3 transformedX = Vector3(0,1,0);
		Transform(&transformedX,&transformedX,&pitchR);
		D3DXVec3Normalize(&transformedX,&transformedX);
		lookAt = transformedX;
		lookAt+=position;
		pitched = false;
		//this->direction = lookAt;
	}
	this->direction = lookAt;
	//Generate new matrix
	D3DXMatrixLookAtLH(&mView, &position, &lookAt, &up);
}
