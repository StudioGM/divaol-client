//
//  SoraSimpleTimerManager.cpp
//  Sora
//
//  Created by Robert Bu on 8/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraSimpleTimerManager.h"
#include "SoraSimpleTimer.h"

namespace sora {

    SoraSimpleTimerManager::SoraSimpleTimerManager() {
        
    }
    
    SoraSimpleTimerManager::~SoraSimpleTimerManager() {
        
    }
    
    void SoraSimpleTimerManager::addList() {
        TimerList::iterator itTimer = mAddList.begin();
        while(itTimer != mAddList.end()) {
            mTimerList.push_back(*itTimer);
            ++itTimer;
        }
        mAddList.clear();
    }

    bool SoraSimpleTimerManager::update(TimeType dt) {
        addList();

        TimerList::iterator itTimer = mTimerList.begin();
        while (itTimer != mTimerList.end()) {
            if((*itTimer)->isEnabled()) {
                bool b = (*itTimer)->update(dt);
                (*itTimer)->setEnable(b);
            }
            ++itTimer;
        }
        return true;
    }
    
    SimpleTimerPtr SoraSimpleTimerManager::createTimer(const TimerFunc& func) {
        SimpleTimerPtr ptr(new SoraSimpleTimer(func, this));
        mTimerList.push_back(ptr.get());
        return ptr;
    }
    
    void SoraSimpleTimerManager::removeTimer(SoraSimpleTimer* timer) {
        mTimerList.remove(timer);
        mAddList.remove(timer);
        //
    }
} // namespace sora
