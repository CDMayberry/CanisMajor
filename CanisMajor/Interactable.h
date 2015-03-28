#pragma once

class Camera;

class Interactable
{
public:
	virtual void interactWith(Camera* player) = 0;
};