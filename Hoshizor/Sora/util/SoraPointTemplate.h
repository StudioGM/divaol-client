//
//  SoraPointTemplte.h
//  Sora
//
//  Created by Ruiwei Bu on 11/8/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraPointTemplate_h
#define Sora_SoraPointTemplate_h

#include "SoraPreprocessor.h"

namespace sora {
    
    template<int Count, typename Type>
    struct SoraPointTemplate {
        
        typedef SoraPointTemplate<Count, Type> SelfType;
        typedef Type PointType;
        
        enum {
            PointCount = Count,
        };
                
        sora_static_assert(Count > 1);
        
        template<typename T2>
        SelfType operator+(const SoraPointTemplate<Count, T2>& rhs) const {
            SelfType tmp = *this;
            for(int i=0; i<PointCount; ++i) {
                tmp.value[i] += static_cast<Type>(rhs.value[i]);
            }
            return tmp;
        }
        
        template<typename T2>
        SelfType operator-(const SoraPointTemplate<Count, T2>& rhs) const {
            SelfType tmp = *this;
            for(int i=0; i<PointCount; ++i) {
                tmp.value[i] -= static_cast<Type>(rhs.value[i]);
            }
            return tmp;
        }
        
        template<typename T2>
        SelfType& operator=(const SoraPointTemplate<Count, T2>& rhs) {
            for(int i=0; i<PointCount; ++i) {
                this->value[i] = static_cast<Type>(rhs.value[i]);
            }
            return *this;
        }
        
        template<typename T2>
        bool operator==(const SoraPointTemplate<Count, T2>& rhs) {
            for(int i=0; i<PointCount; ++i) {
                if(this->value[i] != static_cast<Type>(rhs.value[i]))
                    return false;
            }
            return true;
        }
        
        template<typename T2>
        bool operator!=(const SoraPointTemplate<Count, T2>& rhs) {
            return !(*this == rhs);
        }
        
        template<typename T2>
        SelfType& operator+=(const SoraPointTemplate<Count, T2>& rhs) {
            for(int i=0; i<PointCount; ++i) {
                this->value[i] += static_cast<Type>(rhs.value[i]);
            }
            return *this;
        }
        
        template<typename T2>
        SelfType& operator-=(const SoraPointTemplate<Count, T2>& rhs) {
            SelfType tmp = *this;
            for(int i=0; i<PointCount; ++i) {
                this->value[i] -= static_cast<Type>(rhs.value[i]);
            }
            return *this;
        }
        
        Type& operator[](uint32 index) {
            sora_assert(index >= 0 && index < PointCount);
            return this->value[index];
        }
        
        const Type& operator[](uint32 index) const {
            sora_assert(index >= 0 && index < PointCount);
            return this->value[index];
        }
        
        
        Type value[Count];
    };

    
#define SORA_POINT_TEMP_PARAM(n, d) d SORA_CAT(value, n)
#define SORA_POINT_TEMP_PARAM_ARGS(n, d) SORA_ENUM_N(n, SORA_POINT_TEMP_PARAM, d) 
#define SORA_POINT_PARAM_EXPAND(n, d) d[n] = d##n;

#define DEF_POINT_TEMPLATE_N(n, Type) \
    template<>                                         \
    struct SoraPointTemplate<n, Type> {                             \
        typedef SoraPointTemplate<n, Type> SelfType;                \
        typedef Type PointType;                                     \
        enum {                                                      \
            PointCount = n,                                         \
        };                                                          \
        SoraPointTemplate() {                                       \
        }                                                           \
        SoraPointTemplate(SORA_POINT_TEMP_PARAM_ARGS(n, Type)) {     \
            SORA_REPEAT_N(n, SORA_POINT_PARAM_EXPAND, value)        \
        } \
        template<typename T2>                                           \
        SelfType operator+(const SoraPointTemplate<n, T2>& rhs) const { \
            SelfType tmp = *this;                                       \
            for(int i=0; i<PointCount; ++i) {                           \
                tmp.value[i] += static_cast<Type>(rhs.value[i]);        \
            }                                                           \
            return tmp;                                                 \
        }                                                               \
        template<typename T2>                                           \
        SelfType operator-(const SoraPointTemplate<n, T2>& rhs) const { \
            SelfType tmp = *this;                                       \
            for(int i=0; i<PointCount; ++i) {                           \
                tmp.value[i] -= static_cast<Type>(rhs.value[i]);        \
            }\
            return tmp;\
        }\
        template<typename T2>\
        SelfType& operator=(const SoraPointTemplate<n, T2>& rhs) {\
            for(int i=0; i<PointCount; ++i) {\
                this->value[i] = static_cast<Type>(rhs.value[i]);\
            }\
            return *this;\
        }\
        template<typename T2>\
        bool operator==(const SoraPointTemplate<n, T2>& rhs) {\
            for(int i=0; i<PointCount; ++i) {\
                if(this->value[i] != static_cast<Type>(rhs.value[i]))\
                    return false;\
            }\
            return true;\
        }\
        template<typename T2>\
        bool operator!=(const SoraPointTemplate<n, T2>& rhs) {\
            return !(*this == rhs);\
        }\
        template<typename T2>\
        SelfType& operator+=(const SoraPointTemplate<n, T2>& rhs) {\
            for(int i=0; i<PointCount; ++i) {\
                this->value[i] += static_cast<Type>(rhs.value[i]);\
            }\
            return *this;\
        }\
        template<typename T2>\
        SelfType& operator-=(const SoraPointTemplate<n, T2>& rhs) {\
            SelfType tmp = *this;\
            for(int i=0; i<PointCount; ++i) {\
                this->value[i] -= static_cast<Type>(rhs.value[i]);\
            }\
            return *this;\
        }\
        Type& operator[](uint32 index) {\
            sora_assert(index >= 0 && index < PointCount);\
            return this->value[index];\
        }\
        const Type& operator[](uint32 index) const {\
            sora_assert(index >= 0 && index < PointCount);\
            return this->value[index];\
        }\
        Type value[n];\
    };
    
    DEF_POINT_TEMPLATE_N(1, float);
    DEF_POINT_TEMPLATE_N(2, float);
    DEF_POINT_TEMPLATE_N(3, float);
    DEF_POINT_TEMPLATE_N(4, float);
    
    DEF_POINT_TEMPLATE_N(1, int32);
    DEF_POINT_TEMPLATE_N(2, int32);
    DEF_POINT_TEMPLATE_N(3, int32);
    DEF_POINT_TEMPLATE_N(4, int32);
    
    DEF_POINT_TEMPLATE_N(1, uint32);
    DEF_POINT_TEMPLATE_N(2, uint32);
    DEF_POINT_TEMPLATE_N(3, uint32);
    DEF_POINT_TEMPLATE_N(4, uint32);

} // namespace sora

#endif
