/*
 *  SoraOGLInput.cpp
 *  Sora
 *
 *  Created by Robert Bu on 12/12/10.
 *  Copyright 2010 GameMaster Studio(Project Hoshizora). All rights reserved.
 *
 */

#include "SoraOGLInput.h"

#include "glfw/GL/glfw.h"

#include <vector>

#include "SoraOGLKeyPoll.h"
#include "SoraCore.h"

namespace sora {

	SoraOGLInput::SoraOGLInput() {
		glfwSetKeyCallback(glfwKeyCallback);
		glfwSetMouseButtonCallback(glfwMouseCallback);
	}

	void SoraOGLInput::getMousePos(float *x, float *y) {
		int32 ix, iy;
		glfwGetMousePos(&ix, &iy);
		*x = (float)ix; *y = (float)iy; 
	}

	void SoraOGLInput::setMousePos(float x, float y) {
		glfwSetMousePos((int32)x, (int32)y);
	}

	float SoraOGLInput::getMousePosX() {
		int32 ix, iy;
		glfwGetMousePos(&ix, &iy);
		return (float)ix;
	}

	float SoraOGLInput::getMousePosY() {
		int32 ix, iy;
		glfwGetMousePos(&ix, &iy);
		return (float)iy;
	}

	int32 SoraOGLInput::getMouseWheel() {
		return glfwGetMouseWheel();
	}

	bool SoraOGLInput::isMouseOver() {
		return false;
	}

	bool SoraOGLInput::keyDown(int32 key) {
		if(key >= SORA_KEY_LBUTTON && key <= SORA_KEY_MBUTTON)
			return (glfwGetMouseButton(sorakey_to_glfwkey(key)) == GLFW_PRESS);
		return (glfwGetKey(sorakey_to_glfwkey(key))==GLFW_PRESS);
	}

	bool SoraOGLInput::keyUp(int32 key) {
		if(key >= SORA_KEY_LBUTTON && key <= SORA_KEY_MBUTTON)
			return (glfwGetMouseButton(sorakey_to_glfwkey(key)) == GLFW_RELEASE);
		return (glfwGetKey(sorakey_to_glfwkey(key))==GLFW_RELEASE);
	}

	int32 SoraOGLInput::getKeyState(int32 key) {
		if(key >= SORA_KEY_LBUTTON && key <= SORA_KEY_MBUTTON)
			return (glfwGetMouseButton(sorakey_to_glfwkey(key))==GLFW_PRESS)?SORA_INPUT_KEYDOWN:SORA_INPUT_KEYUP;
		return glfwGetKey(sorakey_to_glfwkey(key))==GLFW_PRESS?SORA_INPUT_KEYDOWN:SORA_INPUT_KEYUP;
	}
	
	char* SoraOGLInput::getKeyName(int32 key) {
		return NULL;
	}
	
	bool SoraOGLInput::joyKeyPressed(int32 key) {
		return false;
	}
	
	bool SoraOGLInput::joyKeyDown(int32 key) {
		return false;
	}
	
	bool SoraOGLInput::joyKeyUp(int32 key) {
		return false;
	}
	
	bool SoraOGLInput::joyKeyState(int32 key, unsigned char state) {
		return false;
	}
	
	bool SoraOGLInput::setJoyKey(int32 key) {
		return false;
	}
	
	bool SoraOGLInput::hasJoy() {
		return false;
	}
    
#ifdef SORA_AUTOMATIC_REGISTER
    SORA_STATIC_RUN_CODE(SoraCore::Instance()->registerInput(new SoraOGLInput()));
#endif
	
} // namespace sora