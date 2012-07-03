//
//  SoraGlobalMessageRouter.cpp
//  Sora
//
//  Created by Robert Bu on 8/24/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraGlobalMessageRouter.h"

namespace sora {
    
    SoraGlobalMessageRouter* SoraGlobalMessageRouter::Instance() {
        static SoraGlobalMessageRouter instance;
        return &instance;
    }
    
    void SoraGlobalMessageRouter::sendMessage(SoraMessageEvent* evt) {
        MessageRouterMap::iterator it = mMessageRouters.find(evt->getMessage());
        if(it != mMessageRouters.end()) {
            it->second.handleEvent(evt);
        }
        
        if(!evt->isConsumed()) {
            mRouter.handleEvent(evt);
        }
    }
    
    void SoraGlobalMessageRouter::handleEvent(SoraEvent* evt) {
#ifdef SORA_USE_RTTI
        if(isClassClass<SoraEvent, SoraMessageEvent>(evt))
#else
        if(evt->getEventIdentifier() == SoraMessageEvent::GetEventIdentifier())
#endif
            sendMessage(static_cast<SoraMessageEvent*>(evt));
    }
    
    
    
} // namespace sora
