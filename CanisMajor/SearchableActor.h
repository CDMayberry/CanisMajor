#pragma once

#include "Actor.h"
#include "Interactable.h"

class Camera;

class SearchableActor: public virtual Actor, public Interactable
{
public:
	SearchableActor():Actor(){concealedItem = nullptr; isActive=false; sprite = 1;name=L"!NAME!";}
	virtual void interactWith(Camera* player);
	void create(Vector3 pos, Vector3 rot, Vector3 scale, Actor* item = nullptr);
	virtual void update(float dt);
	virtual void draw(ID3D10EffectMatrixVariable* fx, Matrix& camera, Matrix& projection, ID3D10EffectTechnique* mTech);
private:
	Actor* concealedItem;
};