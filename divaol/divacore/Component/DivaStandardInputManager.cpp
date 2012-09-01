/*
 *  DivaStandardInputManager.cpp
 *
 *  Created by Hyf042 on 1/29/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#include "DivaStandardInputManager.h"
#include "Core/DivaCore.h"

namespace divacore
{
	void StandardInputManager::gameReset()
	{
		LOGGER->msg("Set","StandardInputManager");

		clear();
		addKey(DIVA_KEY_TYPE_0,SORA_KEY_D);
		addKey(DIVA_KEY_TYPE_1,SORA_KEY_A);
		addKey(DIVA_KEY_TYPE_2,SORA_KEY_S);
		addKey(DIVA_KEY_TYPE_3,SORA_KEY_W);
		addKey(DIVA_KEY_TYPE_4,SORA_KEY_RIGHT);
		addKey(DIVA_KEY_TYPE_5,SORA_KEY_LEFT);
		addKey(DIVA_KEY_TYPE_6,SORA_KEY_DOWN);
		addKey(DIVA_KEY_TYPE_7,SORA_KEY_UP);
		addKey(DIVA_KEY_CT,SORA_KEY_SHIFT);
		//addKey(DIVA_KEY_RELAY_WANT_TO_CHANGE,SORA_KEY_1);
		//addKey(DIVA_KEY_RELAY_WANT_TO_PLAY,SORA_KEY_2);
		addKey(DIVA_KEY_RELAY,SORA_KEY_TAB);
		addKey(DIVA_KEY_SPACE,SORA_KEY_SPACE);
	}

	void StandardInputManager::clear()
	{
		keyMap = KEY_MAP(MAX_ID,NO_KEY);
		keyStrMap.clear();
	}

	void StandardInputManager::init()
	{
	}

	void StandardInputManager::addKey(int32 key, int32 origin, const std::string &tag)
	{
		keyMap[origin] = key;
		if(tag!="")
			keyStrMap[tag] = key;
	}
	void StandardInputManager::onKeyPressed(SoraKeyEvent& event) 
	{
		KeyEvent _event;
		_event.key = originToKey(event.getKey());
		_event.type = KeyEvent::PRESS;
		_event.origin = event.getKey();

		if(!HOOK_MANAGER_PTR->hook(_event)||!HOOK_MANAGER_PTR->hookInfo())
		{
			Core::Ptr->getCoreFlow()->onKeyPressed(_event);
			GAME_MODE_PTR->onKeyPressed(_event);
		}
	}
	void StandardInputManager::onKeyReleased(SoraKeyEvent& event) 
	{
		KeyEvent _event;
		_event.key = originToKey(event.getKey());
		_event.type = KeyEvent::RELEASE;
		_event.origin = event.getKey();

		if(!HOOK_MANAGER_PTR->hook(_event)||!HOOK_MANAGER_PTR->hookInfo())
		{
			Core::Ptr->getCoreFlow()->onKeyReleased(_event);
			GAME_MODE_PTR->onKeyReleased(_event);
		}
	}
}
