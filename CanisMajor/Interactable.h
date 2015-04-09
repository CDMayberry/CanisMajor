#pragma once
#include "GameState.h"
class Camera;

namespace interactableNS
{
	const float INTERACTION_RADIUS_SQ = 36;
};

class Interactable
{
public:
	Interactable(){state=nullptr;stateSwitch=nullptr;}
	std::wstring name;
	virtual void interactWith(Camera* player){if(stateSwitch!=nullptr)state->*stateSwitch=true;};
	virtual std::wstring getUtilText(){return L"Press E to search the " + name;}
	bool targeted;
	void setStateSwitch(GameState* s,bool GameState::*sw){state = s;stateSwitch=sw;}

private:
	GameState* state;
	bool GameState::*stateSwitch;
};