#include "ReadableActor.h"
#include "CanisMajor.h"

void ReadableActor::interactWith(Camera* player)
{
	Interactable::interactWith(player);
	game->reading = true;
	if(concealedItem != nullptr)
	{
		Vector3 pos= concealedItem->getPosition();
		Vector3 rot= concealedItem->getRotation();
		concealedItem->create(pos,rot);
		concealedItem = nullptr;
		game->setNoteText(duration, myText);
	}
	else
	{
		game->setNoteText(duration, myText);
		game->playSound(EMPTY, position);
	}
	if(playOnce) {
		playOnce = false;
		//game->setNoteText(duration, myText);
		game->playSound(cue,position);
	}
}
void ReadableActor::create(Vector3 pos, Vector3 rot, Vector3 scale, Actor* item, float duration)
{
	Actor::create(pos);
	setRotation(rot);
	setScale(scale);
	setDuration(duration);
	concealedItem = item;
	if(item!=nullptr)
		item->isVisible = false;
	targeted = false;
}

void ReadableActor::update(float dt) {
	Actor::update(dt);
	targeted = false;

}

void ReadableActor::draw(ID3D10EffectMatrixVariable* fx, Matrix& camera, Matrix& projection, ID3D10EffectTechnique* mTech) {
	game->mfxHighlightBool->SetBool(targeted);
	Actor::draw(fx, camera, projection, mTech);
}