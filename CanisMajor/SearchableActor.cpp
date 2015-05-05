#include "SearchableActor.h"
#include "CanisMajor.h"

void SearchableActor::interactWith(Camera* player)
{
	Interactable::interactWith(player);
	if(concealedItem != nullptr)
	{
		Vector3 pos= concealedItem->getPosition();
		Vector3 rot= concealedItem->getRotation();
		concealedItem->create(pos,rot);
		concealedItem = nullptr;
		game->setStoryText(2,L"You found something in the " + name);
		game->playSound(JINGLE, pos);
	}
	else
	{
		game->playSound(EMPTY, position);
		game->setStoryText(2,L"The " + name + L" is empty.");
	}
	if(playOnce) {
		playOnce = false;
		game->playSound(cue,position);
		//game->setStoryText(2,L"The " + name + L" is empty.");
	}
}
void SearchableActor::create(Vector3 pos, Vector3 rot, Vector3 scale, Actor* item)
{
	Actor::create(pos);
	setRotation(rot);
	setScale(scale);
	concealedItem = item;
	if(item!=nullptr)
		item->isVisible = false;
	targeted = false;
}

void SearchableActor::update(float dt) {
	Actor::update(dt);
	targeted = false;

}

void SearchableActor::draw(ID3D10EffectMatrixVariable* fx, Matrix& camera, Matrix& projection, ID3D10EffectTechnique* mTech) {
	game->mfxHighlightBool->SetBool(targeted);
	Actor::draw(fx, camera, projection, mTech);
}