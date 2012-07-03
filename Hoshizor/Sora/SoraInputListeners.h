//
//  SoraInputListeners.h
//  Sora
//
//  Created by Robert Bu on 7/31/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraInputListeners_h
#define Sora_SoraInputListeners_h

#include "SoraPlatform.h"
#include "SoraKeyInfo.h"

namespace sora {
    
    class SORA_API SoraInputListener {
    public:
        virtual ~SoraInputListener() {}
        
        virtual void onMouseMoved(SoraMouseEvent& event) { }
        virtual void onMouseDragged(SoraMouseEvent& from, SoraMouseEvent& to) { }
        virtual void onMouseWheelUp(SoraMouseEvent& event) { }
        virtual void onMouseWheelDown(SoraMouseEvent& event) { }
        virtual void onMouseClicked(SoraMouseEvent& event) { }
        virtual void onMouseReleased(SoraMouseEvent& event) { }
                
        virtual void onKeyPressed(SoraKeyEvent& event) { }
        virtual void onKeyReleased(SoraKeyEvent& event) { }

        virtual void onJoystickPressed(SoraJoystickEvent& event) { }
        virtual void onJoystickRelesed(SoraJoystickEvent& event) { }
    };

    
} // namespace sora

#endif
