//
//  SoraCountDownLatch.h
//  Sora
//
//  Created by Robert Bu on 7/6/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraCountDownLatch_h
#define Sora_SoraCountDownLatch_h

#include "SoraPlatform.h"

#include "SoraMutex.h"
#include "SoraCondition.h"


namespace sora {
    
    class SoraCountDownLatch: SoraUncopyable {
    public:
        explicit SoraCountDownLatch(int32 count);
        
        void wait();
        void countDown();
        int32 getCount() const;
        
    private:
        mutable SoraMutexLock mutex;
        SoraCondition condition;
        int32 count;
    };
    
}

#endif
