/*
 *  DivaCoreFlow.h
 *
 *  Created by Hyf042 on 1/13/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_CORE_FLOW
#define DIVA_CORE_FLOW

#include "DivaCommon.h"
#include "DivaMapInfo.h"
#include "DivaCore.h"
#include "DivaNote.h"
#include "DivaEventManager.h"
#include "DivaTask.h"

namespace divacore
{
	using namespace sora;

	class Core;
	typedef Core* CorePtr;

	/*
	CoreFlow
	manage the flow of core game, involved note and event
	*/
	class CoreFlow : public EventHandler
	{
	protected:
		CorePtr core;
	public:
		enum STATE{INIT,READY,RUN,PAUSE,END};
		const char* const MAIN_SOUND_CHANNEL;

		CoreFlow():MAIN_SOUND_CHANNEL("__MAIN_SOUND__") {}
		void setCore(CorePtr core) {this->core = core;}
		virtual void over() {}
		virtual void clear() {}
		virtual void ready() {}
		virtual void start() {}
		virtual void destory() {}
		virtual void update(float dt) {}
		virtual void render()  {}
		virtual void pause() {}
		virtual void resume() {}
		virtual bool toFail(uint32 uid) {return false;}
		//virtual void setTime(double time) {}
		//virtual void setPosition(int position) {}
		virtual double getRunTime() {return 0;}
		virtual double getRealTime() {return 0;}
		virtual double getTotalTime() {return 0;}
		virtual double getPosition() {return 0;}
		virtual uint32 getTotalPosition() {return 0;}
		virtual uint8 getState() {return 0;}
		virtual double getBPM() {return 0;}
		virtual std::string getMainSound() = 0;
		virtual void onKeyPressed(KeyEvent& event) {}
		virtual void onKeyReleased(KeyEvent& event) {}
		//registerEvent
		virtual void registerBeginCallback(Task &task) {}
		virtual void registerEndCallback(Task &task) {}
	};

	typedef CoreFlow* CoreFlowPtr;
}

#endif