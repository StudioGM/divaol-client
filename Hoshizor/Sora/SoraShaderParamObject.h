//
//  SoraShaderParamObject.h
//  Sora
//
//  Created by Ruiwei Bu on 11/13/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraShaderParamObject_h
#define Sora_SoraShaderParamObject_h

#include "SoraPlatform.h"
#include "SoraShader.h"
#include "SoraPreDeclare.h"
#include "SoraMath.h"

namespace sora {
    
    class SoraShader;
    
    template<typename T>
    class SoraShaderParamSetter;
    
    class SORA_API SoraShaderParameter {
    public:
        SoraShaderParameter():
        mParent(0) { }
        
        SoraShaderParameter(SoraShader* parent, const char* name):
        mParent(parent),
        mParamName(name) { }
        
        template<typename T>
        void operator=(const T& param);
        
        SoraShaderParameter& operator=(const SoraShaderParameter& rhs) {
            if(this != &rhs) {
                this->mParent = rhs.mParent;
                this->mParamName = rhs.mParamName;
            }
            return *this;
        }
        
    private:
        SoraShader* mParent;
        std::string mParamName;
    };
    
    template<typename T>
    inline void SoraShaderParameter::operator=(const T& param) {
        if(this->mParent != 0) {
            SoraShaderParamSetter<T>(mParent, mParamName.c_str(), param);
        }
    };
    
    template<typename T>
    class SoraShaderParamSetter {
    public:
    };
    
    template<>
    class SoraShaderParamSetter<float> {
    public:
        SoraShaderParamSetter(SoraShader* shader, const char* name, float f) {
            shader->setParameter1f(name, f);
        }
    };
    
    template<>
    class SoraShaderParamSetter<int> {
    public:
        SoraShaderParamSetter(SoraShader* shader, const char* name, int i) {
            shader->setParameter1i(name, i);
        }
    };
    
    template<>
    class SoraShaderParamSetter<float2> {
    public:
        SoraShaderParamSetter(SoraShader* shader, const char* name, const float2& f) {
            shader->setParameterfv(name, &f[0], 2);
        }
    };
    
    template<>
    class SoraShaderParamSetter<int2> {
    public:
        SoraShaderParamSetter(SoraShader* shader, const char* name, const int2& i) {
            shader->setParameteriv(name, &i[0], 2);
        }
    };
    
    template<>
    class SoraShaderParamSetter<float3> {
    public:
        SoraShaderParamSetter(SoraShader* shader, const char* name, const float3& f) {
            shader->setParameterfv(name, &f[0], 3);
        }
    };
    
    template<>
    class SoraShaderParamSetter<int3> {
    public:
        SoraShaderParamSetter(SoraShader* shader, const char* name, const int3& i) {
            shader->setParameteriv(name, &i[0], 3);
        }
    };
    
    template<>
    class SoraShaderParamSetter<float4> {
    public:
        SoraShaderParamSetter(SoraShader* shader, const char* name, const float4& f) {
            shader->setParameterfv(name, &f[0], 4);
        }
    };
    
    template<>
    class SoraShaderParamSetter<int4> {
    public:
        SoraShaderParamSetter(SoraShader* shader, const char* name, const int4& i) {
            shader->setParameteriv(name, &i[0], 4);
        }
    };
    
    template<>
    class SoraShaderParamSetter<SoraVector> {
    public:
        SoraShaderParamSetter(SoraShader* shader, const char* name, const SoraVector& i) {
            shader->setParameter2f(name, i.x, i.y);
        }
    };
    
    template<>
    class SoraShaderParamSetter<SoraVector3> {
    public:
        SoraShaderParamSetter(SoraShader* shader, const char* name, const SoraVector3& i) {
            shader->setParameter3f(name, i.x, i.y, i.z);
        }
    };
    
    template<>
    class SoraShaderParamSetter<SoraVector4> {
    public:
        SoraShaderParamSetter(SoraShader* shader, const char* name, const SoraVector4& i) {
            shader->setParameter4f(name, i.x, i.y, i.z, i.w);
        }
    };
    
    template<>
    class SoraShaderParamSetter<SoraMatrix3> {
    public:
        SoraShaderParamSetter(SoraShader* shader, const char* name, const SoraMatrix3& i) {
            shader->setParameterfv(name, &i.x[0], 9);
        }
    };
    
    template<>
    class SoraShaderParamSetter<SoraMatrix4> {
    public:
        SoraShaderParamSetter(SoraShader* shader, const char* name, const SoraMatrix4& i) {
            shader->setParameterfv(name, &i.x[0], 16);
        }
    };

} // namespace sora

#endif
