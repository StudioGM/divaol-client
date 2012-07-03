//
//  SoraShaderParamModifier.cpp
//  Sora
//
//  Created by Ruiwei Bu on 10/9/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraShaderParamModifier.h"
#include "SoraShader.h"
#include "math/SoraMathCommon.h"

namespace sora {
    
    SoraShaderParamModifier::SoraShaderParamModifier(const std::string& name, float* from, float* to, uint32 size, float inTime) {
        sora_assert(from);
        sora_assert(to);
        sora_assert(size > 0);
        
        mSize = size;
        mName = name;
        
        mFrom = sora_malloc_t(float, size);
        mTo = sora_malloc_t(float, size);
        mCurr = sora_malloc_t(float, size);
        
        memcpy(mFrom, from, sizeof(float)*size);
        memcpy(mTo, to, sizeof(float)*size);
        
        mCurrTime = 0.f;
        mTime = inTime;
    }
    
    SoraShaderParamModifier::~SoraShaderParamModifier() {
        if(mFrom)
            sora_free(mFrom);
        if(mTo)
            sora_free(mTo);
        if(mCurr)
            sora_free(mCurr);
    }
    
    bool SoraShaderParamModifier::update(float dt) {
        mCurrTime += dt;
        
        for(uint32 i=0;i <mSize; ++i) {
            mCurr[i] = lerp(mFrom[i], mTo[i], mCurrTime / mTime);
        }
        
        if(mCurrTime >= mTime) {
            return true;
        }
        return false;
    }
    
    void SoraShaderParamModifier::modify(SoraShader* object) {
        object->setParameterfv(mName.c_str(), mCurr, mSize);
    }
    
    void SoraShaderParamModifier::reset() {
        mCurrTime = 0.f;
        
        memcpy(mCurr, mFrom, sizeof(float)*mSize);
    }
    
    SoraModifier<SoraShader>* SoraShaderParamModifier::clone() {
        return new SoraShaderParamModifier(mName, mFrom, mTo, mSize, mTime);
    }
    
} // namespace sora
