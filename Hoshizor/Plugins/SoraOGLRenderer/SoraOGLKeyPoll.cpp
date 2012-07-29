/*
 *  SoraOGLKeyPoll.cpp
 *  Sora
 *
 *  Created by Robert Bu on 12/12/10.
 *  Copyright 2010 GameMaster Studio(Project Hoshizora). All rights reserved.
 *
 */
#include "glfw/GL/glfw.h"

#include "SoraOGLKeyPoll.h"
#include "SoraEventManager.h"
#include "SoraInputQueue.h"

#include "SoraLogger.h"

#include <vector>

namespace sora {
	
	int sorakey_to_glfwkey(int key) {
		if((key >= SORA_KEY_0 && key <= SORA_KEY_9) ||
		   (key >= SORA_KEY_A && key <= SORA_KEY_Z))
			return key;
		
		int rkey = key;
		switch(key) {
			case SORA_KEY_F1: rkey = GLFW_KEY_F1; break;
			case SORA_KEY_F2: rkey = GLFW_KEY_F2; break;
			case SORA_KEY_F3: rkey = GLFW_KEY_F3; break;
			case SORA_KEY_F4: rkey = GLFW_KEY_F4; break;
			case SORA_KEY_F5: rkey = GLFW_KEY_F5; break;
			case SORA_KEY_F6: rkey = GLFW_KEY_F6; break;
			case SORA_KEY_F7: rkey = GLFW_KEY_F7; break;
			case SORA_KEY_F8: rkey = GLFW_KEY_F8; break;
			case SORA_KEY_F9: rkey = GLFW_KEY_F9; break;
			case SORA_KEY_F10: rkey = GLFW_KEY_F10; break;
			case SORA_KEY_F11: rkey = GLFW_KEY_F11; break;
			case SORA_KEY_F12: rkey = GLFW_KEY_F12; break;
			
			case SORA_KEY_ESCAPE: rkey = GLFW_KEY_ESC; break;
			case SORA_KEY_BACKSPACE: rkey = GLFW_KEY_BACKSPACE; break;
			case SORA_KEY_TAB: rkey = GLFW_KEY_TAB; break;
			case SORA_KEY_ENTER: rkey = GLFW_KEY_ENTER; break;
			case SORA_KEY_SPACE: rkey = GLFW_KEY_SPACE; break;
			
			case SORA_KEY_SHIFT: rkey = GLFW_KEY_LSHIFT; break;
			case SORA_KEY_CTRL: rkey = GLFW_KEY_LCTRL; break;
			case SORA_KEY_ALT: rkey = GLFW_KEY_LALT; break;
			
			case SORA_KEY_PAUSE: rkey = GLFW_KEY_PAUSE; break;
			case SORA_KEY_CAPSLOCK: rkey = GLFW_KEY_CAPS_LOCK; break;
			case SORA_KEY_NUMLOCK: rkey = GLFW_KEY_KP_NUM_LOCK; break;
			case SORA_KEY_SCROLLLOCK: rkey = GLFW_KEY_SCROLL_LOCK; break;
				
			case SORA_KEY_PGUP: rkey = GLFW_KEY_PAGEUP; break;
			case SORA_KEY_PGDN: rkey = GLFW_KEY_PAGEDOWN; break;
			case SORA_KEY_HOME: rkey = GLFW_KEY_HOME; break;
			case SORA_KEY_END: rkey = GLFW_KEY_END; break;
			case SORA_KEY_INSERT: rkey = GLFW_KEY_INSERT; break;
			case SORA_KEY_DELETE: rkey = GLFW_KEY_DEL; break;
			
			case SORA_KEY_LEFT: rkey = GLFW_KEY_LEFT; break;
			case SORA_KEY_UP: rkey = GLFW_KEY_UP; break;
			case SORA_KEY_RIGHT: rkey = GLFW_KEY_RIGHT; break;
			case SORA_KEY_DOWN: rkey = GLFW_KEY_DOWN; break;
			
			case SORA_KEY_NUMPAD0: rkey = GLFW_KEY_KP_0; break;
			case SORA_KEY_NUMPAD1: rkey = GLFW_KEY_KP_1; break;
			case SORA_KEY_NUMPAD2: rkey = GLFW_KEY_KP_2; break;
			case SORA_KEY_NUMPAD3: rkey = GLFW_KEY_KP_3; break;
			case SORA_KEY_NUMPAD4: rkey = GLFW_KEY_KP_4; break;
			case SORA_KEY_NUMPAD5: rkey = GLFW_KEY_KP_5; break;
			case SORA_KEY_NUMPAD6: rkey = GLFW_KEY_KP_6; break;
			case SORA_KEY_NUMPAD7: rkey = GLFW_KEY_KP_7; break;
			case SORA_KEY_NUMPAD8: rkey = GLFW_KEY_KP_8; break;
			case SORA_KEY_NUMPAD9: rkey = GLFW_KEY_KP_9; break;
			
			case SORA_KEY_GRAVE:		rkey = 0x60; break;
			case SORA_KEY_MINUS:		rkey = 0x2D; break;
			case SORA_KEY_EQUALS:		rkey = 0x3D; break;
			case SORA_KEY_BACKSLASH:	rkey = 0x5C; break;
			case SORA_KEY_LBRACKET:		rkey = 0x5B; break;
			case SORA_KEY_RBRACKET:		rkey = 0x5D; break;
			case SORA_KEY_SEMICOLON:	rkey = 0x3B; break;
			case SORA_KEY_APOSTROPHE:	rkey = 0x27; break;
			case SORA_KEY_COMMA:		rkey = 0x2C; break;
			case SORA_KEY_PERIOD:		rkey = 0x2E; break;
			case SORA_KEY_SLASH:		rkey = 0x2F; break;
			
			case SORA_KEY_LBUTTON: rkey = GLFW_MOUSE_BUTTON_LEFT; break;
			case SORA_KEY_RBUTTON: rkey = GLFW_MOUSE_BUTTON_RIGHT; break;
			case SORA_KEY_MBUTTON: rkey = GLFW_MOUSE_BUTTON_MIDDLE; break;
		}
		return rkey;
	}
	
	int glfwkey_to_sorakey(int key) {
		if((key >= SORA_KEY_0 && key <= SORA_KEY_9) ||
		   (key >= SORA_KEY_A && key <= SORA_KEY_Z))
			return key;
		
		int rkey = key;
		switch(key) {
			case GLFW_KEY_F1: rkey = SORA_KEY_F1; break;
			case GLFW_KEY_F2: rkey = SORA_KEY_F2; break;
			case GLFW_KEY_F3: rkey = SORA_KEY_F3; break;
			case GLFW_KEY_F4: rkey = SORA_KEY_F4; break;
			case GLFW_KEY_F5: rkey = SORA_KEY_F5; break;
			case GLFW_KEY_F6: rkey = SORA_KEY_F6; break;
			case GLFW_KEY_F7: rkey = SORA_KEY_F7; break;
			case GLFW_KEY_F8: rkey = SORA_KEY_F8; break;
			case GLFW_KEY_F9: rkey = SORA_KEY_F9; break;
			case GLFW_KEY_F10: rkey = SORA_KEY_F10; break;
			case GLFW_KEY_F11: rkey = SORA_KEY_F11; break;
			case GLFW_KEY_F12: rkey = SORA_KEY_F12; break;
				
			case GLFW_KEY_ESC: rkey = SORA_KEY_ESCAPE; break;
			case GLFW_KEY_BACKSPACE: rkey = SORA_KEY_BACKSPACE; break;
			case GLFW_KEY_TAB: rkey = SORA_KEY_TAB; break;
			case GLFW_KEY_ENTER: rkey = SORA_KEY_ENTER; break;
			case GLFW_KEY_SPACE: rkey = SORA_KEY_SPACE; break;
				
			case GLFW_KEY_LSHIFT: rkey = SORA_KEY_SHIFT; break;
			case GLFW_KEY_LCTRL: rkey = SORA_KEY_CTRL; break;
			case GLFW_KEY_LALT: rkey = SORA_KEY_ALT; break;
				
			case GLFW_KEY_RSHIFT: rkey = SORA_KEY_SHIFT; break;
			case GLFW_KEY_RCTRL: rkey = SORA_KEY_CTRL; break;
			case GLFW_KEY_RALT: rkey = SORA_KEY_ALT; break;
				
			case GLFW_KEY_PAUSE: rkey = SORA_KEY_PAUSE; break;
			case GLFW_KEY_CAPS_LOCK: rkey = SORA_KEY_CAPSLOCK; break;
			case GLFW_KEY_KP_NUM_LOCK: rkey = SORA_KEY_NUMLOCK; break;
			case GLFW_KEY_SCROLL_LOCK: rkey = SORA_KEY_SCROLLLOCK; break;
				
			case GLFW_KEY_PAGEUP: rkey = SORA_KEY_PGUP; break;
			case GLFW_KEY_PAGEDOWN: rkey = SORA_KEY_PGDN; break;
			case GLFW_KEY_HOME: rkey = SORA_KEY_HOME; break;
			case GLFW_KEY_END: rkey = SORA_KEY_END; break;
			case GLFW_KEY_INSERT: rkey = SORA_KEY_INSERT; break;
			case GLFW_KEY_DEL: rkey = SORA_KEY_DELETE; break;
				
			case GLFW_KEY_LEFT: rkey = SORA_KEY_LEFT; break;
			case GLFW_KEY_UP: rkey = SORA_KEY_UP; break;
			case GLFW_KEY_RIGHT: rkey = SORA_KEY_RIGHT; break;
			case GLFW_KEY_DOWN: rkey = SORA_KEY_DOWN; break;
				
			case GLFW_KEY_KP_0: rkey = SORA_KEY_NUMPAD0; break;
			case GLFW_KEY_KP_1: rkey = SORA_KEY_NUMPAD1; break;
			case GLFW_KEY_KP_2: rkey = SORA_KEY_NUMPAD2; break;
			case GLFW_KEY_KP_3: rkey = SORA_KEY_NUMPAD3; break;
			case GLFW_KEY_KP_4: rkey = SORA_KEY_NUMPAD4; break;
			case GLFW_KEY_KP_5: rkey = SORA_KEY_NUMPAD5; break;
			case GLFW_KEY_KP_6: rkey = SORA_KEY_NUMPAD6; break;
			case GLFW_KEY_KP_7: rkey = SORA_KEY_NUMPAD7; break;
			case GLFW_KEY_KP_8: rkey = SORA_KEY_NUMPAD8; break;
			case GLFW_KEY_KP_9: rkey = SORA_KEY_NUMPAD9; break;
				
			case 0x60:		rkey = SORA_KEY_GRAVE; break;
			case 0x2D:		rkey = SORA_KEY_MINUS; break;
			case 0x3D:		rkey = SORA_KEY_EQUALS; break;
			case 0x5C:		rkey = SORA_KEY_BACKSLASH; break;
			case 0x5B:		rkey = SORA_KEY_LBRACKET; break;
			case 0x5D:		rkey = SORA_KEY_RBRACKET; break;
			case 0x3B:		rkey = SORA_KEY_SEMICOLON; break;
			case 0x27:		rkey = SORA_KEY_APOSTROPHE; break;
			case 0x2C:		rkey = SORA_KEY_COMMA; break;
			case 0x2E:		rkey = SORA_KEY_PERIOD; break;
			case 0x2F:		rkey = SORA_KEY_SLASH; break;
				
			case GLFW_MOUSE_BUTTON_LEFT: rkey = SORA_KEY_LBUTTON; break;
			case GLFW_MOUSE_BUTTON_RIGHT: rkey = SORA_KEY_RBUTTON; break;
			case GLFW_MOUSE_BUTTON_MIDDLE: rkey = SORA_KEY_MBUTTON; break;
		}
		return rkey;
	}
    
	void glfwKeyCallback(int key, int action) {
		int32 type;
		if(action == GLFW_PRESS) type = SORA_INPUT_KEYDOWN;
		else type = SORA_INPUT_KEYUP;

		int32 skey = glfwkey_to_sorakey(key);

		char chr = toasciiWithFlag(skey, (glfwGetKey(GLFW_KEY_LSHIFT) == GLFW_PRESS || glfwGetKey(GLFW_KEY_RSHIFT) == GLFW_PRESS) ? SORA_INPUT_FLAG_SHIFT : 0);
        
        SoraKeyPoll::publishInputedKey(skey, type, chr);
	}
	
	void glfwMouseCallback(int key, int action) {
		int32 type;
		if(action == GLFW_PRESS) type = SORA_INPUT_KEYDOWN;
		else type = SORA_INPUT_KEYUP;
		
        int32 skey;
		switch(key) {
			case GLFW_MOUSE_BUTTON_LEFT: skey = SORA_KEY_LBUTTON; break;
			case GLFW_MOUSE_BUTTON_RIGHT: skey = SORA_KEY_RBUTTON; break;
			case GLFW_MOUSE_BUTTON_MIDDLE: skey = SORA_KEY_MBUTTON; break;
		}
		
		SoraKeyPoll::publishInputedKey(skey, type, 0);
	}

} // namespace sora


