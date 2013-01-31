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
		addJoystick(DIVA_KEY_TYPE_4,JoystickState::JOYSTICK_KEY_RIGHT);
		addJoystick(DIVA_KEY_TYPE_5,JoystickState::JOYSTICK_KEY_LEFT);
		addJoystick(DIVA_KEY_TYPE_6,JoystickState::JOYSTICK_KEY_DOWN);
		addJoystick(DIVA_KEY_TYPE_7,JoystickState::JOYSTICK_KEY_UP);
		addJoystick(DIVA_KEY_TYPE_0,JoystickState::JOYSTICK_KEY_1);
		addJoystick(DIVA_KEY_TYPE_1,JoystickState::JOYSTICK_KEY_3);
		addJoystick(DIVA_KEY_TYPE_2,JoystickState::JOYSTICK_KEY_2);
		addJoystick(DIVA_KEY_TYPE_3,JoystickState::JOYSTICK_KEY_0);
		addJoystick(DIVA_KEY_SPACE,JoystickState::JOYSTICK_KEY_4);
		addJoystick(DIVA_KEY_SPACE,JoystickState::JOYSTICK_KEY_5);
		addJoystick(DIVA_KEY_CT,JoystickState::JOYSTICK_KEY_6);
		addJoystick(DIVA_KEY_CT,JoystickState::JOYSTICK_KEY_7);
		addJoystick(DIVA_KEY_RELAY,JoystickState::JOYSTICK_KEY_8);
	}

	void StandardInputManager::clear()
	{
		keyMap = KEY_MAP(MAX_ID,NO_KEY);
		keyState = KEY_MAP(MAX_ID,0);
		keyStrMap.clear();
	}

	void StandardInputManager::init()
	{
		joystick.Init();
		joystick += DxJoystickInputer::Listener(Base::MakeFunction(&StandardInputManager::joystickEventHandler, this));
	}

	void StandardInputManager::update(float dt)
	{
		joystick.Poll();
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
		_event.type = KeyEvent::DOWN;
		_event.origin = event.getKey();

		if(!HOOK_MANAGER_PTR->hook(_event)||!HOOK_MANAGER_PTR->hookInfo())
		{
			Core::Ptr->getCoreFlow()->onKeyDown(_event);
			GAME_MODE_PTR->onKeyDown(_event);
		}

		_event.type = KeyEvent::PRESS;
		if(keyState[event.getKey()] == 0)
		{
			keyState[event.getKey()] = 1;
			if(!HOOK_MANAGER_PTR->hook(_event)||!HOOK_MANAGER_PTR->hookInfo())
			{
				Core::Ptr->getCoreFlow()->onKeyPressed(_event);
				GAME_MODE_PTR->onKeyPressed(_event);
			}
		}
	}
	void StandardInputManager::onKeyReleased(SoraKeyEvent& event) 
	{
		KeyEvent _event;
		_event.key = originToKey(event.getKey());
		_event.type = KeyEvent::RELEASE;
		_event.origin = event.getKey();

		keyState[event.getKey()] = 0;

		if(!HOOK_MANAGER_PTR->hook(_event)||!HOOK_MANAGER_PTR->hookInfo())
		{
			Core::Ptr->getCoreFlow()->onKeyReleased(_event);
			GAME_MODE_PTR->onKeyReleased(_event);
		}
	}
	void StandardInputManager::joystickEventHandler(int key, int type, int device, int dx, int dy)
	{
		if(type != JoystickState::JOYSTICK_TYPE_PRESSED && type != JoystickState::JOYSTICK_TYPE_UP)
			return;

		KeyEvent _event;
		_event.key = originToKey(key+JOYSTICK_OFFSET);
		_event.type = (type == JoystickState::JOYSTICK_TYPE_PRESSED)?(KeyEvent::PRESS):(KeyEvent::RELEASE);
		_event.origin = key;

		if(!HOOK_MANAGER_PTR->hook(_event)||!HOOK_MANAGER_PTR->hookInfo())
		{
			if (type == JoystickState::JOYSTICK_TYPE_PRESSED) {
				Core::Ptr->getCoreFlow()->onKeyPressed(_event);
				GAME_MODE_PTR->onKeyPressed(_event);
			}
			else {
				Core::Ptr->getCoreFlow()->onKeyReleased(_event);
				GAME_MODE_PTR->onKeyReleased(_event);
			}
		}
	}
}
