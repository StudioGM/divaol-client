/*
 *  DivaStandardCoreFlow.h
 *
 *  Created by Hyf042 on 1/15/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_STANDARD_CORE_FLOW
#define DIVA_STANDARD_CORE_FLOW

#include<queue>
#include "Core/DivaCoreFlow.h"
#include "SoraVlcMoviePlayer/SoraVlcMoviePlayer.h"
#include "SoraSprite.h"
#include "Core/DivaTimeCounter.h"

namespace divacore
{
	using namespace sora;

	class SCF_TimeStamp
	{
	public:
		enum{NOTE_START,NOTE_POINT,EVENT,SYSTEM};

		double time;
		uint8 type;
		uint32 uid;
		uint32 uid2;
		uint32 position;
		
	public:
		SCF_TimeStamp(double time, uint32 position, uint8 type, uint32 uid, uint32 uid2 = 0):
		  time(time),position(position),type(type),uid(uid),uid2(uid2) {}

		SCF_TimeStamp():time(0),position(0),type(0),uid(0),uid2(0) {}

		friend bool operator<(const SCF_TimeStamp &a, const SCF_TimeStamp &b)
		{
			return a.time>b.time;
		}
	};

	/*
	StandardCoreFlow
	manage the game flow with stl::priority_queue
	*/
	class StandardCoreFlow : public CoreFlow
	{
		friend class StandardEditUtility;

		typedef std::priority_queue<SCF_TimeStamp> TIME_QUEUE;
		typedef std::vector<NotePtr> NOTELIST;

		TIME_QUEUE timeQueue;
		MapInfo::NOTES *notesPtr;
		MapInfo::EVENTS *eventsPtr;
		NOTELIST noteList;

		uint8 state;
		double nowTime,nowBPM;
		double totalTime;
		uint32 totalGrid;
		//position
		SCF_TimeStamp lastStamp;
		double nowPosition;

		TimeCounter timeCounter;	

		Task beginTask;
		Task endTask;

		//main sound
		std::string mainSound;
	private:
		void changeBPM(ARGUMENTS &arg);
	public:
		StandardCoreFlow():state(INIT) {clear();}

		void registerEvents();

		virtual void gameReset();
		virtual void gameStop();
		virtual void over() {state = END;}
		virtual void clear();
		virtual void ready();
		virtual void start();
		virtual void destroy();
		virtual void update(float dt);
		virtual void render();
		virtual void pause();
		virtual void resume();
		virtual bool toFail(uint32 uid);
		
		//edit functions
	private:
		float _posToTime(double position);
		double _timeToPos(float time);
		void _locate(double position, float time);
		void _reloadNotes();
		void _reloadEvents();
	public:

		virtual std::string getMainSound() {return mainSound;}
		virtual double getRunTime();
		virtual double getRealTime();
		virtual double getTotalTime();
		virtual double getPosition() {return nowPosition;}
		virtual uint32 getTotalPosition() {return totalGrid;}
		virtual uint8 getState() {return state;}
		virtual double getBPM() {return nowBPM;}

		virtual void onKeyPressed(KeyEvent& event);
		virtual void onKeyReleased(KeyEvent& event);

		void registerBeginCallback(Task &task) {beginTask = task;}
		void registerEndCallback(Task &task) {endTask = task;}

	};

	class StandardEditUtility
	{
		StandardCoreFlow *coreFlow;

		StandardEditUtility() {coreFlow = NULL;}
	public:
		static StandardEditUtility& instance() {static StandardEditUtility instance; return instance;}
		void init();
		void setTime(float time);
		void setPosition(double position);
		void sync() {refreshAll();}
		void refreshAll();
		void reCaltTime();
		void loadResource(MapResourceInfo info);
		void unloadResource(MapResourceInfo info);
		void addEvent(MapEvent event);
		void delEvent(int index);
		void addNote(MapNote note);
		void delNote(int index);
		float posToTime(double position) {return coreFlow->_posToTime(position);}
		double timeToPos(float time) {return coreFlow->_timeToPos(time);}
	};

	typedef CoreFlow* CoreFlowPtr;
}

#endif