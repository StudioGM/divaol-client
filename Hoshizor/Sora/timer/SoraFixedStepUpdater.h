//
//  SoraFixedStepUpdater.h
//  Sora
//
//  Created by Robert Bu on 8/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraFixedStepUpdater_h
#define Sora_SoraFixedStepUpdater_h

#include "SoraPlatform.h"

namespace sora {
    
    /**
     * Object Update in a specific steptime
     **/
    
    template<typename T>
    class SoraFixedStepUpdater {
    public:
        inline SoraFixedStepUpdater(float stepTime):
        mUpdateObj(NULL),
        mStepTime(stepTime),
        mAccumulator(0.f) {
    
        }
        
        inline ~SoraFixedStepUpdater() {}
        
        inline void enable(T* obj) {
            mUpdateObj = obj;
            mAccumulator = 0.f;
        }
        
        inline void disable(T* obj) {
            if(mUpdateObj == obj)
                mUpdateObj = NULL;
        }
        
        inline int32 update(float interval) {
            uint32 result = 0;
            if(mUpdateObj) {
                mAccumulator += interval;
                
                uint32 step = uint32(mAccumulator / mStepTime);
                if(step > 0) {
                    result = mUpdateObj->update(mAccumulator);
                    mAccumulator -= step*stepTime;
                }
            }
            return result;
        }
        
    private:
        T* mUpdateObj;
        const float mStepTime;
        float mAccumulator;
    };
    
} // namespace sora

#endif
