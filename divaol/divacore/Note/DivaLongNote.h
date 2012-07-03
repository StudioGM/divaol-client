/*
 *  DivaLongNote.h
 *
 *  Created by Hyf042 on 1/28/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_LONG_NOTE
#define DIVA_LONG_NOTE

#include "Core/DivaNote.h"
#include "SoraCore.h"
#include "SoraSprite.h"
#include "SoraImageEffect.h"
#include "Component/DivaStandardFactory.h"

namespace divacore
{
	/*
	LongNote
	*/
	class LongNote : public Note
	{
		enum{HEAD,TAIL};
		
		float rhythmHead, rhythmTail, barRate;
		Point tailPosition,notePosition;
		int periodGrid,totalGrid;
		bool bPressStart,bPressOver;

		float length_drawn;
		Point lastTailPosition, nowTailPosition;
		Rect coverRect;

		SoraSprite *noteSprite;
		SoraSprite *rhythmSprite;
		SoraSprite *coverSprite;
		SoraSprite *barMetaSprite;

		void finish(float dt);
	public:
		LongNote(MapNote& noteInfo):Note(noteInfo) {}

		bool checkInfo() {return (noteInfo.noteType=="long"&&noteInfo.notePoint.size()==2);}
		void onInitiate() ;
		void onEnter();
		void onLeave() ;
		void onPoint(uint32 cnt, NotePoint point);
		void onRender();
		void onUpdate(double dt, double position);
		void onPressed(StateEvent& event);
		void onReleased(StateEvent& event);
		bool isReceive(KeyEvent& event, float time);
		void recover(int point, int position, float time);

		Point getReceiveScreenPosition() {return Point(noteSprite->getPosition().x,noteSprite->getPosition().y);}
	};
}

#endif