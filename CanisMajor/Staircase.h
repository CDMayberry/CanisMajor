#pragma once

#include "Actor.h"
#include "Interactable.h"

typedef void(CanisMajor::*LLevel)(void);

class Staircase: public virtual Actor, public virtual Interactable
{
public:
	void setLLevel(LLevel in, std::wstring n){loadLevel = in;name=n;}
	void interactWith(Camera* player);
	virtual std::wstring getUtilText(){return L"Press E to travel " + name;}

private:
	LLevel loadLevel;

};