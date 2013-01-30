/*
 *  DivaStateManager.h
 *
 *  Created by Hyf042 on 1/29/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_STATE_MANAGER
#define DIVA_STATE_MANAGER

#include "DivaCommon.h"
#include "DivaNote.h"
#include "DivaMapInfo.h"
#include "DivaEventManager.h"

namespace divacore
{
	class Note;
	typedef Note* NotePtr;

	class StateEvent
	{
	public:
		enum{PRESS,FAILURE,EXTRA,POINT};
		int8 type;
		uint32 uid;
		NotePtr note;
		std::string tag;
		int info,key,rank,position;
		Point screenPosition;
		int dtScore,dtHP;
		bool breakCombo, breakNote, bOwner;
		float dtTime;
		StateEvent():uid(0),note(NULL),dtHP(0),dtScore(0),dtTime(0),tag(""),info(0),key(NO_KEY),breakCombo(false),breakNote(false),bOwner(true)  {}
		StateEvent(NotePtr note, float dtTime = 0, int score = 0, int hp = 0, const std::string &tag = "", int info = 0, int key=NO_KEY);
		inline bool isOwner() {return bOwner;}
		void setScreenPosition(Point position) {screenPosition = position;}
		Point getScreenPosition() {return screenPosition;}
	};
	/*
	ScoreManager
	*/
	class GameMode : public EventHandler
	{
	public:
		virtual bool getAlive() {return true;}
		virtual int32 getScore() = 0;
		virtual int32 getHP() = 0;
		virtual float getHPinRatio() = 0;
		virtual int32 getCombo() = 0;
		virtual void setScore(int32 score) {}
		virtual void setHP(int32 hp) {}
		virtual void setCombo(int32 combo) {}

		//Game Mode's Name
		virtual std::string getName() = 0;
		virtual std::string getNetGameMode() = 0;

		//Start About
		virtual void preStart() {}
		virtual void afterStart() {}
		virtual void noteOver() {}
	
		//Evaluate About
		virtual void preEvaluate() {}
		virtual void afterEvaluate() {} 

		//PreHook
		virtual void addHookIn() {}

		//check note create event, if it will be passed, return true
		virtual bool checkNote(NotePtr note) {return true;}

		//check note over event
		virtual void checkNoteOver(NotePtr note) {}

		//check press event, if it will be received, return true
		virtual bool checkPress(StateEvent&) = 0;

		//check note point , if it will be rescued, return true
		virtual bool checkPoint(StateEvent&) {return false;}

		//check failure event, if it will actually fail, return true
		virtual bool checkFailure(StateEvent&) {return false;}

		//check extra event
		virtual bool checkExtra(StateEvent&) {return false;}

		//inform to make changes from event
		virtual void inform(StateEvent&) = 0;

		//render info
		virtual void render() {}
		
		//key event
		virtual void onKeyDown(KeyEvent& event) {}
		virtual void onKeyPressed(KeyEvent& event) {}
		virtual void onKeyReleased(KeyEvent& event) {}
	};

	typedef GameMode* GameModePtr;
}

#endif