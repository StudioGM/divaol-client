/*
 *  SoraOGLInput.h
 *  Sora
 *
 *  Created by Robert Bu on 12/12/10.
 *  Copyright 2010 GameMaster. All rights reserved.
 *
 */

#ifndef SORA_OGL_INPUT_H_
#define SORA_OGL_INPUT_H_

#include "SoraInput.h"

namespace sora {
	
	class SoraOGLInput: public SoraInput {
	public:
		SoraOGLInput();
		
		void	getMousePos(float *x, float *y);
		void	setMousePos(float x, float y);
		float getMousePosX();
		float getMousePosY();
		int32	getMouseWheel();
		bool	isMouseOver();
		bool	keyDown(int32 key);
		bool	keyUp(int32 key);
		int32	getKeyState(int32 key);
		char*	getKeyName(int32 key);
		bool	getKeyEvent(SoraKeyEvent& ev);
		bool	joyKeyPressed(int32 key);
		bool	joyKeyDown(int32 key);
		bool	joyKeyUp(int32 key);
		bool	joyKeyState(int32 key, unsigned char state);
		bool	setJoyKey(int32 key);
		bool	hasJoy();
		
		void setWindowHandle(SoraHandle handle) {}
	};
	
} // namespace sora

#endif

