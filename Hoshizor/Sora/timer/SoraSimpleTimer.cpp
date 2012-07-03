//
//  SoraTimer.cpp
//  Sora
//
//  Created by Robert Bu on 8/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraSimpleTimer.h"
#include "ITimerManager.h"

namespace sora {
    
    template<> 
    std::list<SoraSimpleTimer*> AutoListElement2<SoraSimpleTimer>::members;
 
    SoraSimpleTimer::SoraSimpleTimer(const TimerFunc& func, ITimerManager* owner):
    mTimes(0),
    mEnabled(false),
    mPaused(false),
    mTimerFunc(func),
    mInterval(0.f),
    mStartedTime(0.f),
    mPrevDelta(0.f),
    mCurrDelta(0.f),
    mOwner(owner) {
        assert(mOwner);

    }

    SoraSimpleTimer::~SoraSimpleTimer() {
        mOwner->removeTimer(this);
    }
    
    bool SoraSimpleTimer::start(TimeType interval, uint32 times, TimeType start) {
        sora_assert(mTimerFunc);
        
        if(mEnabled)
            return false;
        
        mCurrDelta = 0.f;
        mPrevDelta = interval;
        mInterval = interval;
        mTimes = times;
        mStartedTime = start;
        
        mEnabled = true;
        return true;
    }
    
    void SoraSimpleTimer::stop() {
        mEnabled = false;
    }
    
    void SoraSimpleTimer::pause(bool flag) {
        mPaused = flag;
    }
    
    SoraSimpleTimer::TimeType SoraSimpleTimer::play() {
        mTimerFunc(this, mCurrDelta);
        return mCurrDelta;
    }
    
    bool SoraSimpleTimer::update(TimeType dt) {
        if(mPaused)
            return false;
        
        TimeType timePass = dt + mCurrDelta;
        uint32 step = static_cast<uint32>(timePass / mInterval);
        
        mStartedTime += dt;
        
        if(step > 0) {
            if(mTimes == 0) {
                if(!mTimerFunc(this, mPrevDelta))
                    return false;
            } else {
                if(!mTimerFunc(this, mPrevDelta))
                    return false;
                mTimes -= 1;
                if(mTimes <= 0) {
                    mEnabled = false;
                    return false;
                }
            }
            mPrevDelta = timePass;

            timePass -= step * mInterval;
        }
        mCurrDelta = timePass;
        return true;
    }
    
    void SoraSimpleTimer::setEnable(bool flag) {
        mEnabled = flag;
    }
    
    bool SoraSimpleTimer::isPaused() const {
        return mPaused;
    }
    
    bool SoraSimpleTimer::isEnabled() const {
        return mEnabled;
    }
    
    SoraSimpleTimer::TimeType SoraSimpleTimer::getStartedTime() const {
        return mStartedTime;
    }
    
    SoraSimpleTimer::TimeType SoraSimpleTimer::getPrevDelta() const {
        return mPrevDelta;
    }
    
} // namespace sora