#pragma once

#include "Actor.h"
#include "Interactable.h"

class Key: public virtual Actor, public virtual Interactable
{
public:
	std::wstring name;
	void interactWith(Camera* player);
};