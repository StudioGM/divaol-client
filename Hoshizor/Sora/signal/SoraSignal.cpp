//
//  SoraSignal.cpp
//  Sora
//
//  Created by Robert Bu on 8/2/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraSignal.h"

namespace sora {
    
    namespace signal {
        SignalImpl::~SignalImpl() {
            IteratorType it = mConnections.begin();
            while(it != mConnections.end()) {
                it->second.setControl(false);
                ++it;
            }
            mConnections.clear();
        }
    }
    
} // namespace sora
