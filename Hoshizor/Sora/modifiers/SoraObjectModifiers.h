//
//  SoraObjectModifiers.h
//  Sora
//
//  Created by Robert Bu on 7/15/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraObjectModifiers_h
#define Sora_SoraObjectModifiers_h

#include "SoraObject.h"
#include "SoraModifier.h"
#include "SoraMath.h"

namespace sora {
    
    template<typename T>
    class SORA_API SoraTransitionModifier: public SoraModifier<T> {
    public:
        SoraTransitionModifier(float startx, float starty, float endx, float endy, float inTime):
        mBegin(startx, starty),
        mEnd(endx, endy),
        mInTime(inTime) {
            mCurrValue = mBegin;
            mCurrTime = 0.f;
        }
        SoraTransitionModifier(const SoraVector& start, const SoraVector& end, float inTime):
        mBegin(start),
        mEnd(end),
        mInTime(inTime) {
            mCurrValue = mBegin;
            mCurrTime = 0.f;
        }
        
        bool update(float dt) {
            mCurrTime += dt;
            mCurrValue = lerp(mBegin, mEnd, mCurrTime/mInTime);
            if(mCurrTime >= mInTime) {
                return true;
            }
            return false;
        }
        
        void modify(T* obj) {
            obj->setPosition(mCurrValue.x, mCurrValue.y);
        }
    
        void reset() {
            mCurrValue = mBegin;
            mCurrTime = 0.f;
        }
        
        SoraModifier<T>* clone()  {
            return new SoraTransitionModifier<T>(mBegin, mEnd, mInTime);
        }
        
    private:
        SoraVector mBegin;
        SoraVector mEnd;
        SoraVector mCurrValue;
        float mInTime;
        float mCurrTime;
    };
    
    typedef SoraTransitionModifier<SoraMovable> SoraMovablePositionModifier;

    typedef SoraTransitionModifier<SoraObject> SoraObjectPositionModifier;
    
    
} // namespace sora



#endif
