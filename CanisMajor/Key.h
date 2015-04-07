#pragma once

#include "Actor.h"
#include "Interactable.h"

class Key: public virtual Actor, public virtual Interactable
{
public:
	void interactWith(Camera* player);
	virtual std::wstring getUtilText(){return L"Press E to take the " + name;}
};