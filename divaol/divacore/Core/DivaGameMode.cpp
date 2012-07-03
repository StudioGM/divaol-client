#include "DivaGameMode.h"

namespace divacore
{
	StateEvent::StateEvent(NotePtr note, float dtTime, int score, int hp, const std::string &tag, int info, int key)
		:note(note),dtTime(dtTime),dtScore(score),dtHP(hp),tag(tag),info(info),key(key),breakCombo(false),breakNote(false)
	{
		bOwner = note->isOwner();
		uid = note->getID();
		screenPosition = note->getReceiveScreenPosition();
	}
}