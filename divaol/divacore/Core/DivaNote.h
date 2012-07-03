/*
 *  DivaNote.h
 *
 *  Created by Hyf042 on 1/13/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_NOTE
#define DIVA_NOTE

#include "DivaCommon.h"
#include "DivaMapInfo.h"
#include "DivaConfig.h"
#include "DivaGameMode.h"

namespace divacore
{
	using namespace sora;
	class StateEvent;
	/*
	Note
	a note in core game
	*/
	class Note
	{
	protected:
		ConfigPtr config;
		MapNote &noteInfo;
		int keyPosition;
		int uid,nextPoint,receivePoint;
		int state;
		bool bOver,bOwner;
		std::string tailTag;
		
	protected:
		inline void setState(int state) {this->state=state;}
		inline void setReceiveNote(int uid) {receivePoint = uid;}
		inline void over() {bOver = true;}
	public:
		virtual Point getReceiveScreenPosition() = 0;
		inline int getReceiveNote() {return receivePoint;}
		inline const NotePoint& getReceivePoint() {return noteInfo.notePoint[receivePoint];}
		inline void setConfig(ConfigPtr config) {this->config = config;}
		inline void setOwner(bool isOwner) {bOwner=isOwner;}
		inline bool isOwner() {return bOwner;}
	public:
		enum{START=-0x80,FAILED,END};

		Note(MapNote &noteInfo):receivePoint(0),noteInfo(noteInfo),bOver(false),bOwner(true),tailTag("") {setState(START);}

		virtual void toFail() {over();}
		virtual bool checkInfo() {return true;}
		virtual void onInitiate() {}
		virtual void onEnter() {}
		virtual void onLeave() {}
		virtual void onPoint(uint32 cnt, NotePoint point) {}
		virtual void onRender() {}
		virtual void onUpdate(double dt, double position) {}
		virtual void onPressed(StateEvent& event) {}
		virtual void onReleased(StateEvent& event) {}
		virtual bool isReceive(KeyEvent& event, float time) {return false;}
		virtual bool isOver() {return bOver;}

		inline std::string getTailTag() {return tailTag;}
		inline void setTailTag(const std::string &tag) {tailTag=tag;}
		int getState() {return state;}
		void setKeyPosition(int position) {keyPosition = position;}
		int getKeyPosition() {return keyPosition;}
		void setID(int id) {uid = id;}
		int getID() {return uid;}
		inline void setNextPoint(int nextPoint) {
			if(nextPoint<noteInfo.notePoint.size())
				this->nextPoint = nextPoint;
			else 
				this->nextPoint=END;
		}
		inline int getNextPoint() {return nextPoint;}
		virtual void recover(int point, int position, float time) = 0;
	};

	typedef Note* NotePtr;

	class NoteManager
	{
		std::vector<NotePtr> notes;
	public:
	};
}

#endif