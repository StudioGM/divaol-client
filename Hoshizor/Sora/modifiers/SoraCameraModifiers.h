//
//  SoraCameraModifiers.h
//  Sora
//
//  Created by Robert Bu on 7/19/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraCameraModifiers_h
#define Sora_SoraCameraModifiers_h

#include "SoraPlatform.h"
#include "SoraModifier.h"
#include "SoraMath.h"
#include "SoraCamera.h"

#include "SoraObjectModifiers.h"

namespace sora {
    
    class SORA_API SoraCameraZoomModifier: public SoraModifier<Sora2DCamera> {
    public:
        SoraCameraZoomModifier(float starth, float startv, float hz, float hv, float inTime);
        
        bool    update(float dt);
        void    modify(Sora2DCamera* camera);
        void    reset();
        
        SoraModifier<Sora2DCamera>* clone();
        
    private:
        float mTime;
        float mCurrTime;
        SoraVector mCurrZoom;
        SoraVector mEndZoom;
        SoraVector mStartZoom;
    };
    
    class SORA_API SoraCameraRotationModifier: public SoraModifier<Sora2DCamera> {
    public:
        SoraCameraRotationModifier(float start, float end, float inTime);
        
        bool    update(float dt);
        void    modify(Sora2DCamera* camera);
        void    reset();
        
        SoraModifier<Sora2DCamera>* clone();
        
    private:
        float mTime;
        float mCurrTime;
        float mStart;
        float mEnd;
        float mCurrRot;
    };
    
    typedef SoraTransitionModifier<Sora2DCamera> SoraCameraPositionModifier;
    
} // namespace sora

#endif
