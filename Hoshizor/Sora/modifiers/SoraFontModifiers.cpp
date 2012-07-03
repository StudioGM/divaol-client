//
//  SoraFontModifiers.cpp
//  Sora
//
//  Created by Robert Bu on 7/15/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraFontModifiers.h"

namespace sora {
    
    SoraFontRotationModifier::SoraFontRotationModifier(float begin, float end, float inTime, bool lineRotation, bool rotateChar):
    mBegin(begin),
    mEnd(end),
    mInTime(inTime),
    mCurrTime(0.f),
    mLineRotation(lineRotation),
    mRotateChar(rotateChar) {
        mCurrVal = mBegin;
    }
    
    bool SoraFontRotationModifier::update(float dt) {
        mCurrTime += dt;
        mCurrVal = lerp(mBegin, mEnd, mCurrTime/mInTime);
        if(mCurrTime >= mInTime) {
            return true;
        }
        return false;
    }
    
    void SoraFontRotationModifier::reset() {
        mCurrTime = 0.f;
        mCurrVal = mBegin;
    }
    
    void SoraFontRotationModifier::modify(SoraFont* font) {
        if(!mLineRotation)
            font->setCharRotation(mCurrVal);
        else 
            font->setLineRotation(mCurrVal, mRotateChar);
    }
    
    SoraModifier<SoraFont>* SoraFontRotationModifier::clone() {
        return new SoraFontRotationModifier(mBegin, mEnd, mInTime, mLineRotation, mRotateChar);
    }
    
    SoraFontScaleModifier::SoraFontScaleModifier(float begin, float end, float inTime):
    mBegin(begin),
    mEnd(end),
    mInTime(inTime) {
        mCurrTime = 0.f;
        mCurrVal = begin;
    }
    
    bool SoraFontScaleModifier::update(float dt) {
        mCurrTime += dt;
        mCurrVal = lerp(mBegin, mEnd, mCurrTime/mInTime);
        if(mCurrTime >= mInTime) {
            return true;
        }
        return false;
    }
    
    void SoraFontScaleModifier::reset() {
        mCurrTime = 0.f;
        mCurrVal = mBegin;
    }
    
    void SoraFontScaleModifier::modify(SoraFont* font) {
        font->setScale(mCurrVal);
    }
    
    SoraModifier<SoraFont>* SoraFontScaleModifier::clone() {
        return new SoraFontScaleModifier(mBegin, mEnd, mInTime);
    }
    
} // namespace sora
