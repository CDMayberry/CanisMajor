#include "Camera.h"

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
	yaw = 0;
	roll = 0;
	pitch = 0;
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
	float deltaYaw = 0;
	float _speed = 100;
	float deltaPitch = 0;

	Vector3 direction = Vector3(0,0,0);
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
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		rotated = true;
		deltaPitch += _speed*dt;
		if (deltaPitch < 1) 
			deltaPitch = 1;
		pitch += deltaPitch;
	}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		rotated = true;
		deltaPitch -= _speed*dt;
		if (deltaPitch < -1) 
			deltaPitch = -1;
		pitch += deltaPitch;

	}

	if(GetAsyncKeyState('A') & 0x8000)
			direction.z = 1;
	if(GetAsyncKeyState('D') & 0x8000)
			direction.z = -1;
	if(GetAsyncKeyState('S') & 0x8000)
			direction.x = -1;
	if(GetAsyncKeyState('W') & 0x8000)
			direction.x = 1;
	
	//Generate rotation matrices

	
	//Update position

	//Update LookAt
	if (rotated)
	{

	rotated = false;
	}
	else{
	
	}

	//Generate new matrix
	D3DXMatrixLookAtLH(&mView, &position, &lookAt, &up);
}
