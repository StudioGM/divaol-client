/*
 *  DivaInputManager.h
 *
 *  Created by Hyf042 on 1/29/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_INPUT_MANAGER
#define DIVA_INPUT_MANAGER

#include "DivaCommon.h"
#include "DivaNote.h"
#include "DivaMapInfo.h"
#include "DivaEventManager.h"

namespace divacore
{
	/*
	InputManager, a interlayer between system inputsystem and game core
	*/
	class InputManager : public EventHandler
	{
	public:

		virtual int32 originToKey(int32 key) = 0;
		virtual int32 tagToKey(const std::string &tag) = 0;

		virtual void addKey(int32 key, int32 origin, const std::string &tag = "") = 0;
		virtual void onKeyPressed(SoraKeyEvent& event) = 0;
		virtual void onKeyReleased(SoraKeyEvent& event) = 0;
	};

	typedef InputManager* InputManagerPtr;
}

#endif