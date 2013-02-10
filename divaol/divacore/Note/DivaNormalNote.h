/*
 *  DivaNormalNote.h
 *
 *  Created by Hyf042 on 1/25/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_NORMAL_NOTE
#define DIVA_NORMAL_NOTE

#include "Core/DivaNote.h"
#include "SoraCore.h"
#include "SoraSprite.h"
#include "SoraImageEffect.h"
#include "Component/DivaStandardFactory.h"
#include "Utility/DivaPath.h"

namespace divacore
{
	/*
	NormalNote
	*/
	class NormalNote : public Note
	{
		//variables
		int totalPosition;
		Point tailPosition,notePosition;
		Point lastTailPosition, nowTailPosition;
		Rect coverRect;

		float mEndTime;

		//sprite
		SoraSprite *noteSprite;
		SoraSprite *coverSprite;
		//SoraSprite *arrowSprite;
		SoraSprite *rhythmSprite;
	public:
		NormalNote(MapNote& noteInfo):Note(noteInfo) {}

		std::string getName() {return "normal_note";}

		//check the note info to say whether it's suitable
		bool checkInfo() {return (noteInfo.noteType=="normal"&&noteInfo.notePoint.size()==1);}
		void onInitiate() ;
		void onEnter();
		void onLeave() ;
		void onPoint(uint32 cnt, NotePoint point);
		void onRender();
		void onUpdate(double dt, double position);
		bool onPressed(StateEvent& event);
		bool isReceive(KeyEvent& event, float time);
		void recover(int point, int position, float time);

		Point getReceiveScreenPosition() {return Point(noteSprite->getPosition().x,noteSprite->getPosition().y);}

		//specific gets
		Point getRhythmPosition() {return Point(rhythmSprite->getPositionX(), rhythmSprite->getPositionY());}
	};
}

#endif