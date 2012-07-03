//
//  SoraTuple.h
//  Sora
//
//  Created by Ruiwei Bu on 10/18/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraTuple_h
#define Sora_SoraTuple_h

#include "SoraPlatform.h"
#include "SoraPreprocessor.h"


namespace sora {
    
    namespace tuple {
        
        struct null_type;
        
    } // namespace tuple
    
    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    struct SoraTuple;
    
    template<typename T>
    struct SoraTuple<T, tuple::null_type, tuple::null_type, tuple::null_type, tuple::null_type> {
        SoraTuple() { }
        
        SoraTuple(T a0):
        mA0(a0) {
            
        }
        
        typedef SoraTuple<T, tuple::null_type, tuple::null_type, tuple::null_type, tuple::null_type> SelfType;
        
        T get0() { return mA0; }
        
        template<typename AT>
        bool operator=(const SoraTuple<AT, tuple::null_type, tuple::null_type, tuple::null_type, tuple::null_type>& rhs) {
            mA0 = rhs.mA0;
        }
        
        T mA0;
    };
    
    template<typename T, typename T1>
    struct SoraTuple<T, T1, tuple::null_type, tuple::null_type, tuple::null_type> {
        SoraTuple() { }
        
        SoraTuple(T a0, T1 a1):
        mA0(a0),
        mA1(a1) {
            
        }
        
        typedef SoraTuple<T, T1, tuple::null_type, tuple::null_type, tuple::null_type> SelfType;
        
        T get0() { return mA0; }
        T1 get1() { return mA1; }
        
        template<typename AT, typename AT1>
        bool operator=(const SoraTuple<AT, AT1, tuple::null_type, tuple::null_type, tuple::null_type>& rhs) {
            mA0 = rhs.mA0;
            mA1 = rhs.mA1;
        }
        
        T mA0;
        T1 mA1;
    };
    
    template<typename T, typename T1, typename T2>
    struct SoraTuple<T, T1, T2, tuple::null_type, tuple::null_type> {
        SoraTuple() { }
        
        SoraTuple(T a0, T1 a1, T2 a2):
        mA0(a0),
        mA1(a1),
        mA2(a2) {
            
        }
        
        typedef SoraTuple<T, T1, T2, tuple::null_type, tuple::null_type> SelfType;
        
        T get0() { return mA0; }
        T1 get1() { return mA1; }
        T2 get2() { return mA2; }
        
        template<typename AT, typename AT1, typename AT2>
        bool operator=(const SoraTuple<AT, AT1, AT2, tuple::null_type, tuple::null_type>& rhs) {
            mA0 = rhs.mA0;
            mA1 = rhs.mA1;
            mA2 = rhs.mA2;
        }
        
        T mA0;
        T1 mA1;
        T2 mA2;
    };
    
    template<typename T, typename T1, typename T2, typename T3>
    struct SoraTuple<T, T1, T2, T3, tuple::null_type> {
        SoraTuple() { }
        
        SoraTuple(T a0, T1 a1, T2 a2, T3 a3):
        mA0(a0),
        mA1(a1),
        mA2(a2),
        mA3(a3) {
            
        }
        
        typedef SoraTuple<T, T1, T2, T3, tuple::null_type> SelfType;
        
        T get0() { return mA0; }
        T1 get1() { return mA1; }
        T2 get2() { return mA2; }
        T3 get3() { return mA3; }
        
        template<typename AT, typename AT1, typename AT2, typename AT3>
        bool operator=(const SoraTuple<AT, AT1, AT2, AT3, tuple::null_type>& rhs) {
            mA0 = rhs.mA0;
            mA1 = rhs.mA1;
            mA2 = rhs.mA2;
            mA3 = rhs.mA3;
        }
        
        T mA0;
        T1 mA1;
        T2 mA2;
        T3 mA3;
    };
    
    template<typename T, typename T1, typename T2, typename T3, typename T4>
    struct SoraTuple<T, T1, T2, T3, T3> {
        SoraTuple() { }
        
        SoraTuple(T a0, T1 a1, T2 a2, T3 a3, T4 a4):
        mA0(a0),
        mA1(a1),
        mA2(a2),
        mA3(a3),
        mA4(a4) {
            
        }
        
        typedef SoraTuple<T, T1, T2, T3, T4> SelfType;
        
        T get0() { return mA0; }
        T1 get1() { return mA1; }
        T2 get2() { return mA2; }
        T3 get3() { return mA3; }
        T4 get4() { return mA4; }
        
        template<typename AT, typename AT1, typename AT2, typename AT3, typename AT4>
        bool operator=(const SoraTuple<AT, AT1, AT2, AT3, AT4>& rhs) {
            mA0 = rhs.mA0;
            mA1 = rhs.mA1;
            mA2 = rhs.mA2;
            mA3 = rhs.mA3;
            mA4 = rhs.mA4;
        }
        
        T mA0;
        T1 mA1;
        T2 mA2;
        T3 mA3;
        T4 mA4;
    };
    
} // namespace sora



#endif
