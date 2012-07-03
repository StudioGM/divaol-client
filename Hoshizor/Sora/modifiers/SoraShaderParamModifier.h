//
//  SoraShaderParamModifier.h
//  Sora
//
//  Created by Ruiwei Bu on 10/9/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraShaderParamModifier_h
#define Sora_SoraShaderParamModifier_h

#include "SoraPlatform.h"
#include "SoraModifier.h"

namespace sora {
    
    class SoraShader;
    
    class SoraShaderParamModifier: public SoraModifier<SoraShader> {
    public:
        SoraShaderParamModifier(const std::string& name, float* from, float* to, uint32 size, float inTime);
        virtual ~SoraShaderParamModifier();
        
        bool update(float dt);
        void modify(SoraShader* object);
        void reset();
        
        SoraModifier<SoraShader>* clone();
        
    private:
        float* mFrom;
        float* mTo;
        float* mCurr;
        uint32 mSize;
        std::string mName;
        
        float mTime;
        float mCurrTime;
    };
        
} // namespace sora


#endif
