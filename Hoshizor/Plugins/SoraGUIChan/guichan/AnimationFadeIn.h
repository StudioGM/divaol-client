//
//  AnimationFadeIn.h
//  Guichan_Sora
//
//  Created by Robert Bu(darkfall) 8/26/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_AnimationFadeIn_h
#define Sora_AnimationFadeIn_h

#include "guichan/animation.hpp"

namespace gcn {
    
    class AnimationFadeIn: public Animation {
    public:
        AnimationFadeIn(int start, int end, int time):
        Animation(time),
        mStart(start),
        mEnd(end) {
            
        }
        
        std::string getName() const {
            return "anm_fadein";
        }
        
        virtual void update(Widget* widget) {
            Animation::update(widget);
            
            widget->setAlpha(mStart + (mEnd-mStart)*getCurrTimeRatio());
        }
        
    private:
        int mStart;
        int mEnd;
    };
    
}



#endif
