//
//  SoraCountDownLatch.cpp
//  Sora
//
//  Created by Robert Bu on 7/6/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraCountDownLatch.h"

namespace sora {
    
    SoraCountDownLatch::SoraCountDownLatch(int32 _count):
    mutex(),
    condition(mutex),
    count(_count) {
        
    }
    
    void SoraCountDownLatch::wait() {
        SoraMutexGuard lock(mutex);
        while(count > 0) {
            condition.wait();
        }
    }
    
    void SoraCountDownLatch::countDown() {
        SoraMutexGuard lock(mutex);
        --count;
        if(count == 0)
            condition.notifyAll();
    }
    
    int32 SoraCountDownLatch::getCount() const {
        SoraMutexGuard lock(mutex);
        return count;
    }
    
} // namespace sora
