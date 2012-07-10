//
//  animation.cpp
//  Guichan_Sora
//
//  Created by Robert Bu(darkfall) 8/10/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "guichan/animation.hpp"
#include "guichan/widget.hpp"

namespace gcn {
    
    std::string Animation::MessageStart = "anm_start";
    std::string Animation::MessageEnd = "anm_end";
    
    Animation::Animation(int time):
    Modifier(true),
    mTotalTime(time),
    mCurrTime(0) {
        
    }
    
    void Animation::update(Widget* widget) {
        if(!isFinished()) {
            ++mCurrTime;
            if(mCurrTime >= mTotalTime)
                setFinish(true);
        }
    }
    
    void Animation::reset(int time) {
        setFinish(false);
        
        mCurrTime = 0;
        mTotalTime = time;
    }
    
    void Animation::setFinish(bool flag) {
        if(flag) 
            reset(mTotalTime);
        Modifier::setFinish(flag);
    }
    
    int Animation::getTotalTime() const {
        return mTotalTime;
    }
    
    int Animation::getCurrTime() const {
        return mCurrTime;
    }
    
    float Animation::getCurrTimeRatio() const {
        if(!isFinished())
            return (float)mCurrTime / mTotalTime;
        return 1.f;
    }
    
    void Animation::onAdd(Widget* widget) {
        widget->animationBegan(this);
    }
   
    void Animation::onRemove(Widget* widget) {
        widget->animationEnded(this);
    }
    
    void Animation::onRelease(Widget* widget) {
        widget->animationEnded(this);
    }
    
} // namespace gcn