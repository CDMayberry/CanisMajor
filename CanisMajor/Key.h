#pragma once

#include "Actor.h"
#include "Interactable.h"

class Key: public virtual Actor, public virtual Interactable
{
public:
	void interactWith(Camera* player);
};