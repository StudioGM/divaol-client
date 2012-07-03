//
//  SoraFontModifiers.h
//  Sora
//
//  Created by Robert Bu on 7/15/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraFontModifiers_h
#define Sora_SoraFontModifiers_h

#include "SoraModifier.h"
#include "SoraFont.h"

namespace sora {
    
    class SORA_API SoraFontRotationModifier: public SoraModifier<SoraFont> {
    public:
        SoraFontRotationModifier(float begin, float end, float inTime, bool mLineRotation=true, bool mRotateChar=false);
    
        bool   update(float dt);
        void   modify(SoraFont* font);
        void   reset();

        SoraModifier<SoraFont>* clone();
        
    private:
        float mBegin;
        float mEnd;
        float mCurrVal;
        float mInTime;
        float mCurrTime;
        bool mLineRotation;
        bool mRotateChar;
    };
    
    class SORA_API SoraFontScaleModifier: public SoraModifier<SoraFont> {
    public:
        SoraFontScaleModifier(float begin, float end, float inTime);
        
        bool   update(float dt);
        void   modify(SoraFont* font);
        void   reset();
        
        SoraModifier<SoraFont>* clone();
        
    private:
        float mBegin;
        float mEnd;
        float mCurrVal;
        float mInTime;
        float mCurrTime;
    };
} // namespace sora



#endif
