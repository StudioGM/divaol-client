//
//  SoraKeyInfo.cpp
//  Sora
//
//  Created by Robert Bu on 7/30/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraKeyInfo.h"

namespace sora {
    
    bool isKeyPrintable(int key) {
		if((key >= SORA_KEY_0 && key <= SORA_KEY_9) ||
		   (key >= SORA_KEY_A && key <= SORA_KEY_Z) ||
		   (key == SORA_KEY_GRAVE) ||
		   (key == SORA_KEY_MINUS) ||
		   (key == SORA_KEY_EQUALS) ||
		   (key == SORA_KEY_BACKSLASH) ||
		   (key == SORA_KEY_LBRACKET) ||
		   (key == SORA_KEY_RBRACKET) ||
		   (key == SORA_KEY_SEMICOLON) ||
		   (key == SORA_KEY_APOSTROPHE) ||
		   (key == SORA_KEY_COMMA) ||
		   (key == SORA_KEY_PERIOD) ||
		   (key == SORA_KEY_SLASH) ||
		   (key == SORA_KEY_SPACE)
		   )
			return true;
		return false;
	}
	
    char toasciiWithFlag(int key, int flag) {
		char chr = '?';
		if((key >= SORA_KEY_0 && key <= SORA_KEY_9) ||
		   (key >= SORA_KEY_A && key <= SORA_KEY_Z))
            chr = toascii(key);
		else {
			switch(key) {
				case SORA_KEY_GRAVE: chr = '`';  break;
				case SORA_KEY_MINUS: chr = '-'; break;
				case SORA_KEY_EQUALS: chr = '=';  break;
				case SORA_KEY_BACKSLASH: chr = '\\';  break;
				case SORA_KEY_LBRACKET: chr = '[';  break;
				case SORA_KEY_RBRACKET: chr = ']';  break;
				case SORA_KEY_SEMICOLON: chr = ';';  break;
				case SORA_KEY_APOSTROPHE: chr = '\'';  break;
				case SORA_KEY_COMMA: chr = ',';  break;
				case SORA_KEY_PERIOD: chr = '.';  break;
				case SORA_KEY_SLASH: chr = '/';  break;
				case SORA_KEY_SPACE: chr = ' '; break;
			}
		}
		
		if(flag & SORA_INPUT_FLAG_SHIFT) {
			switch(key) {
				case SORA_KEY_1: chr = '!'; break;
				case SORA_KEY_2: chr = '@'; break;
				case SORA_KEY_3: chr = '#'; break;
				case SORA_KEY_4: chr = '$'; break;
				case SORA_KEY_5: chr = '%'; break;
				case SORA_KEY_6: chr = '^'; break;
				case SORA_KEY_7: chr = '&'; break;
				case SORA_KEY_8: chr = '*'; break;
				case SORA_KEY_9: chr = '('; break;
				case SORA_KEY_0: chr = ')'; break;
				case SORA_KEY_GRAVE: chr = '~';  break;
				case SORA_KEY_MINUS: chr = '_'; break;
				case SORA_KEY_EQUALS: chr = '+';  break;
				case SORA_KEY_BACKSLASH: chr = '|';  break;
				case SORA_KEY_LBRACKET: chr = '{';  break;
				case SORA_KEY_RBRACKET: chr = '}';  break;
				case SORA_KEY_SEMICOLON: chr = ':';  break;
				case SORA_KEY_APOSTROPHE: chr = '"';  break;
				case SORA_KEY_COMMA: chr = '<';  break;
				case SORA_KEY_PERIOD: chr = '>';  break;
				case SORA_KEY_SLASH: chr = '?';  break;
			}
		} else 
			if(key >= SORA_KEY_A && key <= SORA_KEY_Z)
				chr = tolower(chr);
		return chr;
	}

    bool SoraKeyEvent::isKeyDown() const {
        return type == SORA_INPUT_KEYDOWN;
    }
    
    bool SoraKeyEvent::isKeyUp() const {
        return type == SORA_INPUT_KEYUP;
    }
    
    int SoraKeyEvent::getKey() const {
        return key;
    }
    
    int SoraKeyEvent::getType() const {
        return type;
    }
    
    bool SoraKeyEvent::isKeyPressed(int k) const {
        return isKeyDown() && k == key;
    }
    
    bool SoraKeyEvent::isKeyReleased(int k) const {
        return isKeyUp() && k == key;
    }
    
    bool SoraKeyEvent::isPrintable() const {
        return isKeyPrintable(key);
    }
    
    bool SoraKeyEvent::isShiftFlag() const {
        return (flags & SORA_INPUT_FLAG_SHIFT) ? true : false;
    }
    
    bool SoraKeyEvent::isCtrlFlag() const {
        return (flags & SORA_INPUT_FLAG_CTRL) ? true : false;
    }
    
    bool SoraKeyEvent::isAltFlag() const {
        return (flags & SORA_INPUT_FLAG_ALT) ? true : false;
    }
    
    bool SoraKeyEvent::isNumber() const {
        return key >= 48 && key <= 57;
    }
    
    bool SoraKeyEvent::isLetter() const {
        return (((key >= 65 && key <= 90)
                 || (key >= 97 && key <= 122)
                 || (key >= 192 && key <= 255))
                && (key != 215) && (key != 247));
    }
    
    bool SoraKeyEvent::isIMEKey() const {
        return isIME;
    }
    
    SoraMouseEvent::SoraMouseEvent(int _key,
                                   int _type,
                                   float _x,
                                   float _y,
                                   int _wheel) {
        key = _key;
        type = _type;
        x = _x;
        y = _y;
        wheel = _wheel;
    }
    
    SoraMouseEvent::SoraMouseEvent() {
        type = None;
    }

    int SoraMouseEvent::getClickCount() const {
        return clickCount;
    }
    
    bool SoraMouseEvent::isMouseLButtonDown() const {
        return leftBtnDown;
    }
    
    bool SoraMouseEvent::isMouseRButtonDown() const {
        return rightBtnDown;
    }
    
    bool SoraMouseEvent::isMouseMButtonDown() const {
        return middleBtnDown;
    }
    
    bool SoraMouseEvent::isMouseLButtonUp() const {
        return !leftBtnDown;
    }
    
    bool SoraMouseEvent::isMouseRButtonUp() const {
        return !rightBtnDown;
    }
    
    bool SoraMouseEvent::isMouseMButtonUp() const {
        return !middleBtnDown;
    }
    
    float SoraMouseEvent::getX() const {
        return x;
    }
    
    float SoraMouseEvent::getY() const {
        return y;
    }
    
    int SoraJoystickEvent::getJoystickId() const {
        return joystickId;
    }
    
    float SoraMouseEvent::getDragDirection() const {
        return dragDirection;
    }
    
} // namespace sora
