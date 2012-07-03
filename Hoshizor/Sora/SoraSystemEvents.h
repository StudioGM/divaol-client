//
//  SoraSystemEvents.h
//  Sora
//
//  Created by Robert Bu on 8/5/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraSystemEvents_h
#define Sora_SoraSystemEvents_h

#include "SoraEvent.h"
#include "SoraPreDeclare.h"
#include "SoraStringId.h"

namespace sora {
    
    enum {
        SYS_EVT_UPDATE = 0,
        SYS_EVT_ON_ENABLE,
        SYS_EVT_ON_DISABLE,
        SYS_EVT_ADD_HANDLER,
        SYS_EVT_DEL_HANDLER,
        SYT_EVT_ON_DEBUG_RENDER,
    };
    
    class SORA_API SoraSystemEvent: public SoraEvent {
    public:
        SoraSystemEvent(): mType(-1) {}
        SoraSystemEvent(int32 type): mType(SYS_EVT_UPDATE) {}
        
        void setDelta(float dt) { 
            mDelta = dt;
        }
        
        float getDelta() const { 
            return mDelta;
        }
        
        void setType(int32 type) {
            mType = type;
        }
        
        int32 getType() const { 
            return mType;
        }
        
        SORA_EVENT_IDENTIFIER(18446744070299896253ULL);
        
    private:
        int32 mType;
        float mDelta;
    };
    
    inline bool IsSystemEvent(SoraEvent* evt) {
        return evt->getEventIdentifier() == 18446744070299896253ULL;
    }
    
    
} // namespace sora

#endif
