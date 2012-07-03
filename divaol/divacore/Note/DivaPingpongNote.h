/*
 *  DivaPingpongNote.h
 *
 *  Created by Hyf042 on 2/3/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_PINGPONG_NOTE
#define DIVA_PINGPONG_NOTE

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
	class PingpongNote : public Note
	{
		enum{HEAD,TAIL};

		//variables
		int standardNum,cntHit,midNum,limitNum;
		int twoType[2];
		float rhythmHead, rhythmTail, barRate;
		Point tailPosition,notePosition;
		int periodGrid,totalGrid;
		bool bPressStart,bPressOver;

		float length_drawn;
		Point lastTailPosition, nowTailPosition;

		//sprites
		SoraSprite *noteSprite;
		SoraSprite *rhythmSprite;
		SoraSprite *coverSprite;
		SoraSprite *barMetaSprite;

		Rect coverRect;

		void finish(float dt);
	public:
		PingpongNote(MapNote& noteInfo):Note(noteInfo) {}

		bool checkInfo() {return (noteInfo.noteType=="pingpong"&&noteInfo.notePoint.size()==2);}
		void onInitiate() ;
		void onEnter();
		void onLeave() ;
		void onPoint(uint32 cnt, NotePoint point);
		void onRender();
		void onUpdate(double dt, double position);
		void onPressed(StateEvent& event);
		void onReleased(StateEvent& event);
		bool isReceive(KeyEvent& event, float time);
		void finalCalt();
		void recover(int point, int position, float time);

		Point getReceiveScreenPosition() {return Point(noteSprite->getPosition().x,noteSprite->getPosition().y);}
	};
}

#endif