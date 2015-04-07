#pragma once

class Camera;

namespace interactableNS
{
	const float INTERACTION_RADIUS_SQ = 36;
};

class Interactable
{
public:
	std::wstring name;
	virtual void interactWith(Camera* player)=0;
	virtual std::wstring getUtilText(){return L"Press E to search the " + name;}
	bool targeted;

};