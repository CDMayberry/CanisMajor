#pragma once

#include "Actor.h"
#include "Interactable.h"

using std::wstring;

//namespace NOTE {
//	wstring note1 = L"Bloody hell, where have those children hidden my artifacts???";
//};

class Camera;

class ReadableActor: public virtual Actor, public Interactable
{
public:
	ReadableActor():Actor(){concealedItem = nullptr; isActive=false;name=L"Book"; sprite = 1;myText = L"The note is scratched and unreadable."; duration = 0.0f;}
	virtual void interactWith(Camera* player);
	void create(Vector3 pos, Vector3 rot, Vector3 scale, Actor* item = nullptr, float duration = 0.0f);
	virtual void update(float dt);
	void setText(wstring t) {myText = t;}
	void setDuration(float dur) {duration = dur;}
	virtual std::wstring getUtilText(){return L"Press LMB to read the " + name;}
	virtual void draw(ID3D10EffectMatrixVariable* fx, Matrix& camera, Matrix& projection, ID3D10EffectTechnique* mTech);
private:
	Actor* concealedItem;
	float duration;
	wstring myText;
};