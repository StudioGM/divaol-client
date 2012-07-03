//
//  SoraObserver.cpp
//  Sora
//
//  Created by Robert Bu on 7/11/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraObserver.h"

namespace sora {
    
    SoraAbstractObserver::SoraAbstractObserver() {
        
    }
    
    SoraAbstractObserver::SoraAbstractObserver(const SoraAbstractObserver& observer) {
        
    }
    
    SoraAbstractObserver::~SoraAbstractObserver() {
        
    }
    
    SoraAbstractObserver& SoraAbstractObserver::operator =(const SoraAbstractObserver& rhs) {
        return *this;
    }
    
} // namespace sora