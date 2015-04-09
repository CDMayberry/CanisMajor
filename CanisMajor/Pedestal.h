#pragma once
#include "Actor.h"
#include "Interactable.h"
#include "QuestItem.h"
#include "Camera.h"
#include "GameState.h"

namespace PedestalNS{
	const Vector3 RING_LOCATION(0,3.5,0);
	const Vector3 HIDDEN_LOCATION(2,2,0);
};

class Pedestal: public virtual Actor, public virtual Interactable
{
public:

	virtual void create(QuestItem* hidden, QuestItem* a,QuestItem* LR,QuestItem* MR,QuestItem* SR,Vector3 pos, Vector3 rot, Vector3 scale);
	virtual void update(float dt);
	void interactWith(Camera* player);
	virtual std::wstring getUtilText();
	void load(GameState *state);

private:
	QuestItem *arrow, *LRing, *MRing, *SRing, *hidden;
	bool hasArrow, hasLR, hasMR, hasSR;
};