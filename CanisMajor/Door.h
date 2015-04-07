#pragma once

#include "Actor.h"
#include "Interactable.h"
#include "Key.h"

namespace doorNS{
	const float SWING_SPEED = PI;
};

class Door: public virtual Actor, public virtual Interactable
{
public:
	void interactWith(Camera* player);
	void create(Vector3 pos, Vector3 rot, Vector3 scale, Key* k, bool open = false);
	void update(float dt);
	virtual std::wstring getUtilText(){
		if(isOpen)return L"Press E to close the " + name;
		else return L"Press E to open the "+name;
	}
	bool getOpen(){return isOpen;}
private:
	Key * key;
	float closedRotation;
	float openRotation;
	bool isOpen;
};