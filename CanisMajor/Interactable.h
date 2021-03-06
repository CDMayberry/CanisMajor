#pragma once
#include "GameState.h"
class Camera;

namespace interactableNS
{
	const float INTERACTION_RADIUS_SQ = 36;
	const int HAND_SPRITE = 0;
	const int BOOK_SPRITE = 1;
	const int MAGNIFY_SPRITE = 2;
	const int DOOR_SPRITE = 3;
	const int ARROW_SPRITE = 4;
	const int DOWN_SPRITE = 5;

};

class Interactable
{
public:
	Interactable(){state=nullptr;stateSwitch=nullptr; sprite = -1; playOnce = false;}
	std::wstring name;
	virtual void interactWith(Camera* player){if(stateSwitch!=nullptr)state->*stateSwitch=true;};
	virtual std::wstring getUtilText(){return L"Press LMB to search the " + name;}
	bool targeted, playOnce;
	int sprite;
	void setCue(const char* set, bool play) {cue = set; playOnce = play;}
	void setStateSwitch(GameState* s,bool GameState::*sw){state = s;stateSwitch=sw;}
	virtual void reset(){state=nullptr; stateSwitch=nullptr;targeted=false; playOnce = false;}
protected:
	GameState* state;
	const char* cue;
	bool GameState::*stateSwitch;
};