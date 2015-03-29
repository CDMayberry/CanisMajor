#pragma once

#include "Actor.h"
#include "Interactable.h"

class Camera;

class SearchableActor: public virtual Actor, public Interactable
{
public:
	SearchableActor():Actor(){concealedItem = nullptr; isActive=false;name=L"!NAME!";}
	virtual void interactWith(Camera* player);
	void create(Vector3 pos, Vector3 rot, Vector3 scale, Actor* item = nullptr);
private:
	Actor* concealedItem;
};