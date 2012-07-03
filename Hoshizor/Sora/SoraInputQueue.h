//
//  SoraInputQueue.h
//  Sora
//
//  Created by Robert Bu on 7/2/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraInputQueue_h
#define Sora_SoraInputQueue_h

#include "SoraKeyInfo.h"
#include "SoraHotkey.h"
#include "SoraInput.h"
#include "SoraInputListeners.h"
#include "SoraFrameListener.h"

#include <stack>

namespace sora {
        
    class SORA_API SoraKeyPoll: public SoraFrameListener {
    private:
        SoraKeyPoll();
        ~SoraKeyPoll();
        void onFrameStart();
        void onFrameEnd();
        
        friend class SoraCore;
        
    public:        
        typedef std::stack<SoraKeyEvent> IteratorType;

        static void publishInputedKey(int32 key, int32 type, char keyChr);
        static bool getQueueEvent(SoraKeyEvent& ev);
        static void setQueueInput(SoraInput* input);
        static void clearInputQueue();
        
        static int32 addGlobalHotKey(const SoraHotkey& hotkey, SoraEventHandler* handler);
        static void delGlobalHotkey(int32 hid);
        static void setGlobalHotkey(int32 hid, const SoraHotkey& hotkey);
        static void clearGlobalHotkeys();
        
        static void AddInputListener(SoraInputListener* mouseListener, int prio=0);
        static void DelInputListener(SoraInputListener* joyListener);
        
        static void pollListenerEvents();
    };
}


#endif
