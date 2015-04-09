#include "Pedestal.h"
#include "CanisMajor.h"

using namespace PedestalNS;

void Pedestal::create(QuestItem* h,QuestItem* a,QuestItem* LR,QuestItem* MR,QuestItem* SR,Vector3 pos, Vector3 rot, Vector3 scale)
{
	Actor::create(pos,rot,scale);
	arrow = a;
	LRing = LR;
	MRing = MR;
	SRing = SR;
	hidden = h;
	hasArrow=hasLR=hasMR=hasSR = false;
	name = L"pedestal";
	hidden->isVisible=false;
	hidden->setPosition(pos+HIDDEN_LOCATION);
}
void Pedestal::update(float dt)
{
	if(isActive)	
	{
		Actor::update(dt);
		Vector3 r;
		if(hasArrow)
			arrow->update(dt);
		if(hasLR)
		{
			r = LRing->getRotation();
			r += Vector3(1,0,0)*dt;
			LRing->setRotation(r);
			LRing->update(dt);
		}
		if(hasMR)
		{
			r = MRing->getRotation();
			r += Vector3(0,1,0)*dt;
			MRing->setRotation(r);
			MRing->update(dt);
		}
		if(hasSR)
		{
			r = SRing->getRotation();
			r += Vector3(0,0,1)*dt;
			SRing->setRotation(r);
			SRing->update(dt);
		}
	}
}
void Pedestal::interactWith(Camera* player)
{
	if(!solved)
	{
		if(player->checkItem(arrow))
		{
			player->removeItem(arrow);
			arrow->isVisible = true;
			arrow->isActive = true;
			arrow->ableToBeTaken = false;
			arrow->setPosition(getPosition()+RING_LOCATION);
			hasArrow = true;
			game->setStoryText(3,L"You place the golden arrow on the pedestal");
			state->arrowPlaced = true;
		}
		else if(player->checkItem(LRing))
		{
			player->removeItem(LRing);
			LRing->isVisible = true;
			LRing->isActive = true;
			LRing->ableToBeTaken = false;
			LRing->setPosition(getPosition()+RING_LOCATION);
			hasLR = true;
			game->setNoteText(3,L"You place the largest ring on the pedestal");
			state->LRPlaced = true;
		}
		else if(player->checkItem(MRing))
		{
			player->removeItem(MRing);
			MRing->isVisible = true;
			MRing->isActive = true;
			MRing->ableToBeTaken = false;
			MRing->setPosition(getPosition()+RING_LOCATION);
			hasMR = true;
			game->setNoteText(3,L"You place the ring on the pedestal");
			state->MRPlaced = true;
		}
		else if(player->checkItem(SRing))
		{
			player->removeItem(SRing);
			SRing->isVisible = true;
			SRing->isActive = true;
			SRing->ableToBeTaken = false;
			SRing->setPosition(getPosition()+RING_LOCATION);
			hasSR = true;
			game->setNoteText(3,L"You place the smallest ring on the pedestal");
			state->SRPlaced = true;
		}
		else
			game->setNoteText(3,L"The inscription of 3 rings revolving around an arrow can be faintly seen.");

		if(hasArrow&&hasLR&&hasMR&&hasSR)
		{
			hidden->isVisible = true;
			solved=true;
		}
	}
}

std::wstring Pedestal::getUtilText()
{
	return L"Press E to examine the " + name;
}

void Pedestal::load(GameState *s)
{
	hasArrow = s->arrowPlaced;
	hasLR = s->LRPlaced;
	hasMR = s->MRPlaced;
	hasSR = s->SRPlaced;
	state=s;
}