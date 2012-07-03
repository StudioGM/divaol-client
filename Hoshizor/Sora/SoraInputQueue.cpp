//
//  SoraInputQueue.cpp
//  Sora
//
//  Created by Robert Bu on 7/2/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraInputQueue.h"
#include "SoraCore.h"
#include "SoraEventManager.h"

#include <stack>
#include <queue>

namespace sora {
    
    static SoraInput* g_input = NULL;
    
    typedef std::vector<SoraKeyEvent> KeyPoll;
    static KeyPoll g_keyPoll;
    
    struct HotkeyNode {
        SoraHotkey mHotkey;
        int32 mId;
        
        SoraEventHandler* mHandler;
        
        HotkeyNode(const SoraHotkey& node, int32 id, SoraEventHandler* handler):
        mHotkey(node),
        mId(id),
        mHandler(handler) {}
    };
    
    typedef std::vector<HotkeyNode> HotkeyCont;
    static HotkeyCont g_hotkeys;
    static int32 g_next_hotkeyid = 0;
    
    float g_mousex = 0.f, g_mousey = 0.f;
    bool g_lbuttondown = false, g_rbuttondown = false, g_mbuttondown = false;
    SoraMouseEvent g_prevEvent;
    
    typedef std::queue<SoraMouseEvent> MouseEventCont;
    MouseEventCont g_mouseEvents;
    
    typedef std::map<int32, SoraInputListener*> InputListenerMap;
    
    InputListenerMap g_inputListeners;
    
    void SoraKeyPoll::setQueueInput(SoraInput* input) {
        g_input = input;
    }
    
    SoraKeyPoll::SoraKeyPoll() {
        SoraCore::Instance()->addFrameListener(this);
    }
    
    SoraKeyPoll::~SoraKeyPoll() {
        SoraCore::Instance()->delFrameListener(this);
    }
    
    void SoraKeyPoll::clearInputQueue() {
        g_keyPoll.clear();
    }
    
    void checkHotkey(SoraKeyEvent* kev) {
        for(int i=0; i<g_hotkeys.size(); ++i) {
            if(g_hotkeys[i].mHotkey.test(kev)) {
                SoraHotkeyEvent hev(&g_hotkeys[i].mHotkey, g_hotkeys[i].mId);
                
                g_hotkeys[i].mHandler->handleEvent(&hev);
                kev->consume();
            }
        }
    }
    
    void SoraKeyPoll::publishInputedKey(int32 key, int32 type, char keyChr) {
        if(!g_input)
            return;
        
        SoraKeyEvent ev;
        
        ev.key = key;
        ev.type = type;
        
        ev.flags = 0;
        if(g_input->keyDown(SORA_KEY_SHIFT))
            ev.flags |= SORA_INPUT_FLAG_SHIFT;
        
        if(g_input->keyDown(SORA_KEY_ALT))
            ev.flags |= SORA_INPUT_FLAG_ALT;
        
        if(g_input->keyDown(SORA_KEY_CTRL))
            ev.flags |= SORA_INPUT_FLAG_CTRL;
        
        if(g_input->keyDown(SORA_KEY_CAPSLOCK))
            ev.flags |= SORA_INPUT_FLAG_CAPSLOCK;
        
        if(g_input->keyDown(SORA_KEY_SCROLLLOCK))
            ev.flags |= SORA_INPUT_FLAG_SCROLLLOCK;
        
        ev.chr = keyChr;
        
        ev.wheel = g_input->getMouseWheel();
        ev.x = g_input->getMousePosX();
        ev.y = g_input->getMousePosY();
        
        checkHotkey(&ev);
        
        if(!ev.isConsumed())
            SORA_EVENT_MANAGER->publishInputEvent(&ev);
        
        // key listeners
        if(!g_inputListeners.empty()) {
            InputListenerMap::iterator itListener = g_inputListeners.begin();
            while(itListener != g_inputListeners.end()) {
                if(ev.isKeyDown())
                    itListener->second->onKeyPressed(ev);
                else 
                    itListener->second->onKeyReleased(ev);
                ++itListener;
            }
        }

        if(!ev.isConsumed())
            g_keyPoll.push_back(ev);
    }
    
    bool SoraKeyPoll::getQueueEvent(SoraKeyEvent& event) {
        if(g_keyPoll.size() == 0)
            return false;
        
        event = g_keyPoll.back();
        g_keyPoll.pop_back();
        
        return true;
    }
    
    int32 SoraKeyPoll::addGlobalHotKey(const SoraHotkey& hotkey, SoraEventHandler* handler) {
        g_hotkeys.push_back(HotkeyNode(hotkey, g_next_hotkeyid, handler));
        ++g_next_hotkeyid;
        return g_next_hotkeyid-1;
    }
    
    void SoraKeyPoll::delGlobalHotkey(int32 hid) {
        for(int i = 0; i<g_hotkeys.size(); ++i)
            if(g_hotkeys[i].mId == hid) {
                g_hotkeys.erase(g_hotkeys.begin()+i);
                break;
            }
    }
    
    void SoraKeyPoll::setGlobalHotkey(int32 hid, const SoraHotkey& hotkey) {
        for(int i = 0; i<g_hotkeys.size(); ++i)
            if(g_hotkeys[i].mId == hid) {
                g_hotkeys[i].mHotkey = hotkey;
                break;
            }
    }
    
    void SoraKeyPoll::clearGlobalHotkeys() {
        g_hotkeys.clear();
        g_next_hotkeyid = 0;
    }
    
    void pollMouseInput() {
        if(!g_input)
			return;

		float curMouseX, curMouseY;
        int mouseWheel;
        bool leftBtn, rightBtn, middleBtn;
        
		g_input->getMousePos(&curMouseX, &curMouseY);
        mouseWheel = g_input->getMouseWheel();
        
        leftBtn = g_input->getKeyState(SORA_KEY_LBUTTON)==SORA_INPUT_KEYDOWN;
        rightBtn = g_input->getKeyState(SORA_KEY_RBUTTON)==SORA_INPUT_KEYDOWN;
        middleBtn = g_input->getKeyState(SORA_KEY_MBUTTON)==SORA_INPUT_KEYDOWN;
        
        if(g_mousex != curMouseX ||
           g_mousey != curMouseY) {
            g_mouseEvents.push(SoraMouseEvent(0,
                                              SoraMouseEvent::Moved,
                                              curMouseX,
                                              curMouseY,
                                              mouseWheel));
            g_mousex = curMouseX;
            g_mousey = curMouseY;
        }
        
        if(mouseWheel > 0) {
            g_mouseEvents.push(SoraMouseEvent(0,
                                              SoraMouseEvent::WheelUp,
                                              curMouseX,
                                              curMouseY,
                                              mouseWheel));
        }
        if(mouseWheel < 0) {
            g_mouseEvents.push(SoraMouseEvent(0,
                                              SoraMouseEvent::WheelDown,
                                              curMouseX,
                                              curMouseY,
                                              mouseWheel));
        }
        
        if(!g_lbuttondown && leftBtn) {
            g_mouseEvents.push(SoraMouseEvent(SORA_KEY_LBUTTON,
                                              SoraMouseEvent::Pressed,
                                              curMouseX,
                                              curMouseY,
                                              mouseWheel));
        }
        
        if(!g_rbuttondown && rightBtn) {
            g_mouseEvents.push(SoraMouseEvent(SORA_KEY_RBUTTON,
                                              SoraMouseEvent::Pressed,
                                              curMouseX,
                                              curMouseY,
                                              mouseWheel));
            
        }
        
        if(!g_mbuttondown && middleBtn) {
            g_mouseEvents.push(SoraMouseEvent(SORA_KEY_MBUTTON,
                                              SoraMouseEvent::Pressed,
                                              curMouseX,
                                              curMouseY,
                                              mouseWheel));
            
        }
        
        if(g_lbuttondown && !leftBtn) {
            g_mouseEvents.push(SoraMouseEvent(SORA_KEY_LBUTTON,
                                              SoraMouseEvent::Released,
                                              curMouseX,
                                              curMouseY,
                                              mouseWheel));
        }
        
        if(g_rbuttondown && !rightBtn) {
            g_mouseEvents.push(SoraMouseEvent(SORA_KEY_RBUTTON,
                                              SoraMouseEvent::Released,
                                              curMouseX,
                                              curMouseY,
                                              mouseWheel));
            
        }
        
        if(g_mbuttondown && !middleBtn) {
            g_mouseEvents.push(SoraMouseEvent(SORA_KEY_MBUTTON,
                                              SoraMouseEvent::Released,
                                              curMouseX,
                                              curMouseY,
                                              mouseWheel));
            
        }
                               
        g_lbuttondown = leftBtn;
        g_rbuttondown = rightBtn;
        g_mbuttondown = middleBtn;
    }

    void SoraKeyPoll::AddInputListener(SoraInputListener* mouseListener, int prio) {
        g_inputListeners.insert(std::make_pair(prio, mouseListener));
		if(!g_inputListeners.empty()) {
			InputListenerMap::iterator itListener = g_inputListeners.begin();
			while(itListener != g_inputListeners.end()) {
                ++itListener;
            }
		}
    }
    
    void SoraKeyPoll::DelInputListener(SoraInputListener* mouseListener) {
        InputListenerMap::iterator itListener = g_inputListeners.begin();
        while(itListener != g_inputListeners.end()) {
            if(itListener->second == mouseListener) {
                g_inputListeners.erase(itListener);
                break;
            }
            ++itListener;
        }
    }
    
    void SoraKeyPoll::onFrameStart() {
        
    }
    
    void SoraKeyPoll::onFrameEnd() {
        pollMouseInput();
        pollListenerEvents();
        clearInputQueue();
    }
    
    void SoraKeyPoll::pollListenerEvents() {
        if(!g_inputListeners.empty() && !g_mouseEvents.empty()) {
            while(!g_mouseEvents.empty()) {
                SoraMouseEvent mevent = g_mouseEvents.front();
                g_mouseEvents.pop();
                
                InputListenerMap::iterator itListener = g_inputListeners.begin();
                while(itListener != g_inputListeners.end()) {
                    switch(mevent.type) {
                        case SoraMouseEvent::Moved:
                            if(!g_lbuttondown)
                                itListener->second->onMouseMoved(mevent);
                            else {
                                mevent.type = SoraMouseEvent::Dragged;
                                itListener->second->onMouseDragged(g_prevEvent, mevent);
                                
                                g_prevEvent = mevent;
                            }
                            break;
                        case SoraMouseEvent::Pressed:
                            itListener->second->onMouseClicked(mevent); 
                            g_prevEvent = mevent;
                            break;
                        
                        case SoraMouseEvent::Released:
                            itListener->second->onMouseReleased(mevent); 
                            break;
                        
                        case SoraMouseEvent::WheelUp:
                            itListener->second->onMouseWheelUp(mevent); 
                            break;
                        
                        case SoraMouseEvent::WheelDown:
                            itListener->second->onMouseWheelDown(mevent); 
                            break;
                    }
                    ++itListener;
                }
            }
        }
        // to do joystick event
    }
}