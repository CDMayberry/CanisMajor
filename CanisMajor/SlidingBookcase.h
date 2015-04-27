#pragma once
#include "Door.h"

namespace slidingBookcaseNS{
	const Vector3 SLIDING_DISP(0,0,5);
	const float EPS = 0.4;
};

class SlidingBookcase: public virtual Door{
public:
	void update(float dt);
	void create(Vector3 pos, Vector3 rot, Vector3 scale, QuestItem* k, bool open = false);
	virtual std::wstring getUtilText(){
		if(getOpen())return L"";
		else return L"";
	}
private:
	Vector3 closedPos,openPos;
};