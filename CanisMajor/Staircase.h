#pragma once

#include "Actor.h"
#include "Interactable.h"

typedef void(CanisMajor::*LLevel)(void);

class Staircase: public virtual Actor, public virtual Interactable
{
public:
	Staircase():Actor(){sprite = 3;}
	void setLLevel(LLevel in, std::wstring n){loadLevel = in;name=n; sprite = 3;}
	void interactWith(Camera* player);
	virtual std::wstring getUtilText(){return L"Press LMB to travel " + name;}

private:
	LLevel loadLevel;

};