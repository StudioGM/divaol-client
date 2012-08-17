/*
 *  DivaCoreGameState.h
 *
 *  Created by Hyf042 on 1/15/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_CORE_PLAY_STATE
#define DIVA_CORE_PLAY_STATE

#include "Core/DivaCoreState.h"
#include "Core/DivaMapLoader.h"
#include "SoraTask.h"
#include "SoraText.h"
#include "SoraResourceFile.h"

namespace divacore
{
	using namespace sora;

	/*
	CorePlayState
	A state for game playing
	*/
	class CorePlayState : public CoreState
	{
		CoreFlow *coreFlow;
		sora::SoraText mText;
		sora::SoraFont* mFont;
		bool bFinish;
		SoraTimestamp *start_time;
		float volume;
		bool bPressed;
		bool bShowDetail;
	public:
		void onInitiate();

		void onEnter();

		void onLeave();

		void onRender();

		void onUpdate(float dt);

		void onDestroy();

		void load(sora::SoraTask* task);

		void onKeyPressed(SoraKeyEvent& event);
		void onKeyReleased(SoraKeyEvent& event);

		void pause(void* args = NULL);
	};

	typedef CorePlayState* CorePlayStatePtr;
}

#endif