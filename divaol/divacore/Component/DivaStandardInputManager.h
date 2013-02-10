/*
 *  DivaStandardInputManager.h
 *
 *  Created by Hyf042 on 1/29/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_STANDARD_INPUT_MANAGER
#define DIVA_STANDARD_INPUT_MANAGER

#include "Core/DivaInputManager.h"
#include "Utility/DivaJoystickInputer.h"

namespace divacore
{
	/*
	InputManager
	*/
	class StandardInputManager : public InputManager
	{
		static const int JOYSTICK_OFFSET = 256;
		static const int MAX_ID = 512;
		typedef std::vector<int32> KEY_MAP; 
		typedef std::map<std::string,int32> KEY_STR_MAP;
		KEY_MAP keyMap;
		KEY_MAP keyState;
		KEY_STR_MAP keyStrMap;
		DxJoystickInputer joystick;

	public:
		StandardInputManager() {}

		void init();
		void clear();

		void gameReset();
		void update(float dt);
		
		inline int32 originToKey(int32 key) {return keyMap[key];}
		inline int32 tagToKey(const std::string &tag) {return keyStrMap[tag];}
		void addKey(int32 key, int32 origin, const std::string &tag = "");
		void addJoystick(int32 key, int32 origin, const std::string &tag = "") {addKey(key, origin+JOYSTICK_OFFSET, tag);}

		void onKeyPressed(SoraKeyEvent& event) ;
		void onKeyReleased(SoraKeyEvent& event) ;
		void joystickEventHandler(int key, int type, int device, int dx, int dy);
	};
}

#endif