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

	bool getOpen(){return isOpen;}
private:
	Key * key;
	float closedRotation;
	float openRotation;
	bool isOpen;
};