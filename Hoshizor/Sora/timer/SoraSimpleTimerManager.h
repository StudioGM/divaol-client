//
//  SoraTimerManager.h
//  Sora
//
//  Created by Robert Bu on 8/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraTimerManager_h
#define Sora_SoraTimerManager_h

#include "ITimerManager.h"
#include "SoraSimpleTimer.h"
#include <list>

namespace sora {
    
    class SoraSimpleTimerManager: public ITimerManager {
    public:
        typedef std::list<SoraSimpleTimer*> TimerList;
        
        SoraSimpleTimerManager();
        ~SoraSimpleTimerManager();
        
        bool update(TimeType dt);
        SimpleTimerPtr createTimer(const TimerFunc& func);
        void removeTimer(SoraSimpleTimer* timer);
        
    private:
        void addList();
        
        TimerList mTimerList;
        TimerList mAddList;
    };
    
    template<typename T>
    static SimpleTimerPtr CreateSimpleTimer(const T& func) {
        return SoraSimpleTimerManager::RefInstance().createTimer(func);
    }
    
} // namespace sora


#endif
