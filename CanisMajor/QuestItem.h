#pragma once

#include "Actor.h"
#include "Interactable.h"

class QuestItem: public virtual Actor, public virtual Interactable
{
public:
	QuestItem(){ableToBeTaken = true; sprite = 0;}
	void interactWith(Camera* player);
	virtual std::wstring getUtilText(){return L"Press E to take the " + name;}
	bool ableToBeTaken;
	virtual void reset(){Interactable::reset(); ableToBeTaken=true;}
};