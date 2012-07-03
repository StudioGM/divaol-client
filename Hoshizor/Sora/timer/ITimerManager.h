//
//  ITimerManager.h
//  Sora
//
//  Created by Robert Bu on 8/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_ITimerManager_h
#define Sora_ITimerManager_h

#include "SoraSingleton.h"
#include "SoraPlatform.h"
#include "SoraDelegate.h"
#include "function/SoraFunction.h"

namespace sora {
    
    /**
     * TimerManager Interface
     **/
    class SoraSimpleTimer;
    
    typedef SoraAutoPtr<SoraSimpleTimer> SimpleTimerPtr;
    
    class ITimerManager: public SoraDirectSingleton<ITimerManager> {
    public:
        typedef float TimeType;
        typedef SoraFunction<bool(SoraSimpleTimer*, TimeType)> TimerFunc;
        
        virtual bool update(TimeType dt) = 0;
        virtual SimpleTimerPtr createTimer(const TimerFunc& func) = 0;
        virtual void removeTimer(SoraSimpleTimer* timer) = 0;
        
    };
    
} // namespace sora


#endif
