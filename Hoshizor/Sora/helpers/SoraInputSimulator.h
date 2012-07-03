/*
 *  SoraInputSimulator.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/6/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_INPUT_SIMULATOR_H_
#define SORA_INPUT_SIMULATOR_H_

#include "SoraPlatform.h"
#include "SoraKeyInfo.h"
#include "SoraPlugin.h"

#include <map>

namespace sora {
	
	class SORA_API SoraInputSimulator {
	public:
		static void simulateKey(int32 key, int32 state);
		static void simulateEvent(SoraKeyEvent* ev);
								  
		static int32 getKeyState(int32 key);
		static void clear();
		
	private:
		struct KeyState {
			int32 key;
			int32 state;
		};
		typedef std::map<int32, KeyState> KEY_STATE_MAP;
		static KEY_STATE_MAP keyStates;
	}; 
	
} // namespace sora

#endif // SORA_INPUT_SIMULATOR_H_