//
//  SoraNotification.cpp
//  Sora
//
//  Created by Robert Bu on 7/11/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraNotification.h"

namespace sora {
    
    SoraNotification::SoraNotification() {
        
    }
    
    SoraNotification::~SoraNotification() {
        
    }
    
    std::string SoraNotification::name() const {
        return typeid(*this).name();
    }
    
} // namespace sora
