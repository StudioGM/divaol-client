//
//  SoraEventFactory.cpp
//  Sora
//
//  Created by Robert Bu on 8/6/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraEventFactory.h"
#include "SoraSystemEvents.h"

namespace sora {
    
    void SoraEventFactory::destroyEvent(SoraEvent* evt) {
        delete evt;
    }
    
    SoraEvent* SoraEventFactory::createEvent(const std::string& n) {
        if(n.empty()) {
            return new SoraSystemEvent();
        } else {
            return createInstance(n);
        }
    }
    
    SoraEventFactory* SoraEventFactory::Instance() {
        static SoraEventFactory instance;
        return &instance;
    }
    
    SoraEventFactory& SoraEventFactory::RefInstance() {
        return *Instance();
    }
    
    SoraEventFactory::SoraEventFactory() {
        
    }
    
    SoraEventFactory::~SoraEventFactory() {
        
    }
    
} // namespace sora 
