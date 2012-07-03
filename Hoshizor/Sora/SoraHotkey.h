//
//  SoraHotkey.h
//  Sora
//
//  Created by Robert Bu on 7/1/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraHotkey_h
#define Sora_SoraHotkey_h

#include "SoraPlatform.h"
#include "SoraKeyInfo.h"

#include "SoraEvent.h"

namespace sora {
    
    class SoraHotkey;
    
    class SORA_API SoraHotkeyEvent: public SoraEvent {
    public:  
        SoraHotkeyEvent(): mHotkey(NULL) {}
        SoraHotkeyEvent(SoraHotkey* hkey, int32 hid): mHotkey(hkey), mHotkeyId(hid) {}
        
        SoraHotkey* getHotkey() const {
            return mHotkey;
        }
        
        int32 getHotkeyId() const {
            return mHotkeyId;
        }
        
        SORA_EVENT_IDENTIFIER(18446744072805578078ULL);
        
    private:
        SoraHotkey* mHotkey;
        int32 mHotkeyId;
    };
    
    class SORA_API SoraHotkey {
    public:
        SoraHotkey();
        SoraHotkey(int32 key, bool ctrlFlag=false, bool altFlag=false, bool shiftFlag=false);
        
        /**
         *  if this flag equals true,
         *  then when test a hotkey, it will test whether the event is keypress event
         *  otherwise both keypress or keyrelease is ok
         *  you can take this advantage to make something like press to show, release to hide
         *  default true
         **/
        void setRequirePress(bool flag);
        
        bool test();
        bool test(SoraKeyEvent* kev);
        
        void set(int32 key, bool ctrlFlag=false, bool altFlag=false, bool shiftFlag=false);
        
        bool isCtrlFlag() const;
        bool isAltFlag() const;
        bool isShiftFlag() const;
        int32 getKey() const;
        
        SoraHotkey& operator=(const SoraHotkey& rhs);
        
        SoraHotkey(const SoraHotkey& rhs) {
            *this = rhs;
        }
        
    private:
        int32 mKey;
        bool mRequirePress;
        
        bool mCtrlFlag, mAltFlag, mShiftFlag;
    };
    
} // namespace sora

#endif
