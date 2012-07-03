//
//  SoraDeltaWatch.h
//  Sora
//
//  Created by Robert Bu on 7/31/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraDeltaWatch_h
#define Sora_SoraDeltaWatch_h

#include "SoraPlatform.h"
#include "SoraCore.h"

#include <deque>

namespace sora {
    
    /**
     *  A special watch for smooth delta 
     *  It will calculate the delta based on the averge of FRAME_COUNT - 1's delta
     *  For smoother animation update
     **/
    template<int FrameCount = 10>
    class SORA_API SoraDeltaWatch {
    public:
        SoraDeltaWatch(): 
        mCurrDelta(0.f) {
        
        }
        
        void update();
        
        float getDelta() const;
        
    private:
        typedef std::deque<float> FrameQueue;
        FrameQueue mFrames;
        
        float mCurrDelta;
    };
    
    template<int FrameCount>
    void SoraDeltaWatch<FrameCount>::update() {
        float delta = SoraCore::Ptr->getDelta();
        
        mFrames.push_back(delta);
        if(mFrames.size() >= FrameCount)
            mFrames.pop_front();
        
        if(mCurrDelta == 0.f)
            mCurrDelta = delta;
        else {
            mCurrDelta = (mCurrDelta * (mFrames.size() - 1) + delta) / (mFrames.size() + 1);
        }
    }
    
    template<int FrameCount>
    float SoraDeltaWatch<FrameCount>::getDelta() const {
        return mCurrDelta;
    }
    
    
} // namespace sora

#endif
