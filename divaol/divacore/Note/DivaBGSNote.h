/*
 *  DivaLongNote.h
 *
 *  Created by Hyf042 on 1/28/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_BGS_NOTE
#define DIVA_BGS_NOTE

#include "Core/DivaNote.h"
#include "SoraCore.h"
#include "SoraSprite.h"
#include "SoraImageEffect.h"
#include "Component/DivaStandardFactory.h"
#include "Utility/DivaPath.h"

namespace divacore
{
	/*
	LongNote
	*/
	class BGSNote : public Note
	{
	public:
		BGSNote(MapNote& noteInfo):Note(noteInfo) {}
		virtual ~BGSNote();

		std::string getName() {return "bgs_note";}

		bool checkInfo() {return (noteInfo.noteType=="bgs"&&noteInfo.notePoint.size()==1);}
		void onInitiate() ;
		void onEnter();
		void onLeave();
		void onPoint(uint32 cnt, NotePoint point);
		void onRender();
		void onUpdate(double dt, double position);
		void onPressed(StateEvent& event);
		void onReleased(StateEvent& event);
		bool isReceive(KeyEvent& event, float time);
		void recover(int point, int position, float time);

		Point getReceiveScreenPosition() {return Point(0, 0);}
	};
}

#endif