//
//  SoraTimer.h
//  Sora
//
//  Created by Robert Bu on 8/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_Timer_SoraTimer_h
#define Sora_Timer_SoraTimer_h

#include "SoraPlatform.h"
#include "SoraTimestamp.h"
#include "SoraFunction.h"
#include "SoraUncopyable.h"
#include "util/SoraAutoContainer.h"

namespace sora {
    
    class ITimerManager;
    
    class SoraSimpleTimer {
    public:
        typedef float TimeType;
        typedef SoraFunction<bool(SoraSimpleTimer*, TimeType)> TimerFunc;
        
        SoraSimpleTimer(const TimerFunc& func, ITimerManager* owner);
        virtual ~SoraSimpleTimer();
    
        /*
         *  As the timer update depends on SoraCore update
         *  minimum interval depends on current framerate
         *  For FPS = 60, minimum interval = 1000/60 ~= 17ms
         */
        virtual bool start(TimeType interval, uint32 times, TimeType start);
        virtual void stop();
        virtual void pause(bool flag);
        virtual TimeType play();
        
        virtual bool update(TimeType dt);
        
        bool isPaused() const;
        bool isEnabled() const;
        
        
        void setEnable(bool flag);
        
        // time since the timer has started
        TimeType getStartedTime() const;
        // previous delta, maybe larger than defined because of frame update
        TimeType getPrevDelta() const;
                
    private:
        uint32 mTimes;
        bool mEnabled;
        bool mPaused;
        TimerFunc mTimerFunc;
        
        TimeType mInterval;
        TimeType mStartedTime;
        TimeType mPrevDelta;
        TimeType mCurrDelta;
        
        ITimerManager* mOwner;
    };

    
} // namespace

#endif
