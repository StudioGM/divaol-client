//
//  SoraPreDeclare.h
//  Sora
//
//  Created by Robert Bu on 7/31/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraPreDeclare_h
#define Sora_SoraPreDeclare_h

#include "SoraPlatform.h"
#include "SoraAny.h"
#include "SoraStringId.h"

#include "util/SoraPointTemplate.h"

#include <string>
#include <map>

namespace sora {
    
#ifdef USE_TR1_SHARED_PTR
    
#include <tr1/memory>
    #define SoraSharedPtr std::tr1::shared_ptr 
#else
    #define SoraSharedPtr SoraAutoPtr
#endif // USE_TR1_SHARED_PTR
    
    typedef std::map<std::string, SoraAny> NamedValueList;
    
    // cast a value in NamedValueList to T
    // would throw BadCastException if conversation fails
    template<typename T>
    static T GetTFromNamedValueList(const std::string& val, const NamedValueList& list) {
        NamedValueList::const_iterator it = list.find(val);
        if(it != list.end()) {
            return AnyCast<T>(it->second);
        }
    }
    
    
    typedef SoraPointTemplate<1, float> float1;
    typedef SoraPointTemplate<2, float> float2;
    typedef SoraPointTemplate<3, float> float3;
    typedef SoraPointTemplate<4, float> float4;
    
    typedef SoraPointTemplate<1, int32> int1;
    typedef SoraPointTemplate<2, int32> int2;
    typedef SoraPointTemplate<3, int32> int3;
    typedef SoraPointTemplate<4, int32> int4;
    
    typedef SoraPointTemplate<1, uint32> uint1;
    typedef SoraPointTemplate<2, uint32> uint2;
    typedef SoraPointTemplate<3, uint32> uint3;
    typedef SoraPointTemplate<4, uint32> uint4;
    
    
#ifdef SORA_USE_RTTI
    
    
    /**
     * RTTI Check 
     **/
    template<typename T1, typename T2>
    bool isClassSame(T1* t1, T2* t2) {
        return (&typeid(*t1) == &typeid(*t2) ||
                typeid(*t1) == typeid(t2));
    }
    template<typename T1, typename T2>
    bool isClassClass(T1* t1) {
        return (&typeid(*t1) == &typeid(T2) ||
                typeid(*t1) == typeid(T2));
    }
#endif
    
    // runtime def & not def
    
    static std::map<SoraStringId, int> g_define_map;
    
    static void SoraDefine(const std::string& f, int val=0) {
        g_define_map.insert(std::make_pair(GetUniqueStringId(f), val));
    }
    
    static bool SoraIfDefined(const std::string& f) {
        std::map<SoraStringId, int>::const_iterator it = g_define_map.find(GetUniqueStringId(f));
        return it != g_define_map.end();
    }
    
#define SORA_DEFINE(name) \
    SoraDefine(#name)
    
#define SORA_IF_DEFINED(name) \
    SoraIfDefined(#name)
    
#define SORA_MIN(a, b) (a < b ? a : b)
#define SORA_MAX(a, b) (a > b ? a : b)
#define SORA_ABS(x)    (x < 0 ? -x : x)
    
    
#define SORA_DEF_SET_GET(type, name, var) \
    public: \
        type get##name() const { \
            return var; \
        } \
        void set##name(const type& val) { \
            var = val; \
        } \
    
    
    /**
     *  helper macro to define a field which have a get##name and set##name
     *  @param field type, such as int, float
     *  @param name name to be used as get(name) and set(name)
     *  Must be used within the class
     */
#define SORA_CLASS_DEF_FIELD_SET_GET(type, name) \
    private: \
        type name; \
    public: \
        type get##name() const { \
            return name; \
        } \
        void set##name(type val) { \
            name = val; \
        }
    
    /**
     *  helper macro to define a field which have a get##name and set##name
     *  @param field type, such as int, float
     *  @param name name to be used as get(name) and set(name)
     *  @param name prefix of the variable, the defined variable would be pref##name
     *  Must be used within the class
     */
#define SORA_CLASS_DEF_FIELD_SET_GET_P(type, name, pref) \
    private: \
        type pref##name; \
    public: \
        type get##name() const { \
            return pref##name; \
        } \
        void set##name(type val) { \
            pref##name = val; \
        }
    
} // namespace sora


#endif
