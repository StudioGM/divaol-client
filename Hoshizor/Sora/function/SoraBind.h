//
//  SoraBind.h
//  zeromq_client
//
//  Created by Ruiwei Bu on 8/15/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef SoraBind_h
#define SoraBind_h

#include "SoraMemfun.h"

namespace sora {
    
    namespace detail {
        
        template<typename obj, typename sig>
        struct MemfunContainer {
            typedef obj ObjType;
            typedef SoraMemfun<sig, obj> MemfunType;
            typedef typename MemfunType::Memfun MemfunPtr;
            
            MemfunContainer(ObjType* object,
                            MemfunPtr func):
            mMemfun(MemFun(func)),
            mObj(object) {
                
            }
            
            operator bool() {
                return mObj && mMemfun;
            }
            
        protected:
            ObjType* mObj;
            MemfunType mMemfun;
        };
        
    } // namespace detail
    
    template<typename obj, typename sig>
    class SoraBind;
    
    template<typename obj, typename R>
    class SoraBind<obj, R(void)>: public detail::MemfunContainer<obj, R(void)> {
    public:
        typedef detail::MemfunContainer<obj, R(void)> base_type;
        typedef typename base_type::ObjType ObjType;
        typedef typename base_type::MemfunPtr MemfunPtr;
        
        SoraBind(ObjType* object,
                 MemfunPtr memfun):
        base_type(object,
                  memfun) {
            
        }
        
        inline R operator()() {
            return base_type::mMemfun(base_type::mObj);
        }
    };
    
    template<typename obj, typename R, typename T0>
    class SoraBind<obj, R(T0)>: public detail::MemfunContainer<obj, R(T0)> {
    public:
        typedef detail::MemfunContainer<obj, R(T0)> base_type;
        typedef typename base_type::ObjType ObjType;
        typedef typename base_type::MemfunPtr MemfunPtr;
        
        SoraBind(ObjType* object,
                 MemfunPtr memfun):
        base_type(object,
                  memfun) {
            
        }
        
        inline R operator()(T0 a0) {
            return base_type::mMemfun(base_type::mObj, a0);
        }
    };
    
    template<typename obj, typename R, typename T0, typename T1>
    class SoraBind<obj, R(T0, T1)>: public detail::MemfunContainer<obj, R(T0, T1)> {
    public:
        typedef detail::MemfunContainer<obj, R(T0, T1)> base_type;
        typedef typename base_type::ObjType ObjType;
        typedef typename base_type::MemfunPtr MemfunPtr;
        
        SoraBind(ObjType* object,
                 MemfunPtr memfun):
        base_type(object,
                  memfun) {
            
        }
        
        inline R operator()(T0 a0, T1 a1) {
            return base_type::mMemfun(base_type::mObj, a0, a1);
        }
    };
    
    template<typename obj, typename R, typename T0, typename T1, typename T2>
    class SoraBind<obj, R(T0, T1, T2)>: public detail::MemfunContainer<obj, R(T0, T1, T2)> {
    public:
        typedef detail::MemfunContainer<obj, R(T0, T1, T2)> base_type;
        typedef typename base_type::ObjType ObjType;
        typedef typename base_type::MemfunPtr MemfunPtr;
        
        SoraBind(ObjType* object,
                 MemfunPtr memfun):
        base_type(object,
                  memfun) {
            
        }
        
        inline R operator()(T0 a0, T1 a1, T2 a2) {
            return base_type::mMemfun(base_type::mObj, a0, a1, a2);
        }
    };
    
    template<typename obj, typename R, typename T0, typename T1, typename T2, typename T3>
    class SoraBind<obj, R(T0, T1, T2, T3)>: public detail::MemfunContainer<obj, R(T0, T1, T2, T3)> {
    public:
        typedef detail::MemfunContainer<obj, R(T0, T1, T2, T3)> base_type;
        typedef typename base_type::ObjType ObjType;
        typedef typename base_type::MemfunPtr MemfunPtr;
        
        SoraBind(ObjType* object,
                 MemfunPtr memfun):
        base_type(object,
                  memfun) {
            
        }
        
        inline R operator()(T0 a0, T1 a1, T2 a2, T3 a3) {
            return base_type::mMemfun(base_type::mObj, a0, a1, a2, a3);
        }
    };
    
    template<typename obj, typename R, typename T0, typename T1, typename T2, typename T3, typename T4>
    class SoraBind<obj, R(T0, T1, T2, T3, T4)>: public detail::MemfunContainer<obj, R(T0, T1, T2, T3, T4)> {
    public:
        typedef detail::MemfunContainer<obj, R(T0, T1, T2, T3, T4)> base_type;
        typedef typename base_type::ObjType ObjType;
        typedef typename base_type::MemfunPtr MemfunPtr;
        
        SoraBind(ObjType* object,
                 MemfunPtr memfun):
        base_type(object,
                  memfun) {
            
        }
        
        inline R operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4) {
            return base_type::mMemfun(base_type::mObj, a0, a1, a2, a3, a4);
        }
    };
    
    template<typename obj, 
            typename R, 
            typename T0, 
            typename T1, 
            typename T2, 
            typename T3, 
            typename T4,
            typename T5>
    class SoraBind<obj, R(T0, T1, T2, T3, T4, T5)>: public detail::MemfunContainer<obj, R(T0, T1, T2, T3, T4, T5)> {
    public:
        typedef detail::MemfunContainer<obj, R(T0, T1, T2, T3, T4, T5)> base_type;
        typedef typename base_type::ObjType ObjType;
        typedef typename base_type::MemfunPtr MemfunPtr;
        
        SoraBind(ObjType* object,
                 MemfunPtr memfun):
        base_type(object,
                  memfun) {
            
        }
        
        inline R operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
            return base_type::mMemfun(base_type::mObj, a0, a1, a2, a3, a4, a5);
        }
    };
    
    template<typename obj, 
            typename R, 
            typename T0, 
            typename T1, 
            typename T2, 
            typename T3, 
            typename T4,
            typename T5,
            typename T6>
    class SoraBind<obj, R(T0, T1, T2, T3, T4, T5, T6)>: 
        public detail::MemfunContainer<obj, R(T0, T1, T2, T3, T4, T5, T6)> {
            
    public:
        typedef detail::MemfunContainer<obj, R(T0, T1, T2, T3, T4, T5, T6)> base_type;
        typedef typename base_type::ObjType ObjType;
        typedef typename base_type::MemfunPtr MemfunPtr;
        
        SoraBind(ObjType* object,
                 MemfunPtr memfun):
        base_type(object,
                  memfun) {
            
        }
        
        inline R operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) {
            return base_type::mMemfun(base_type::mObj, a0, a1, a2, a3, a4, a5, a6);
        }
    };
    
    template<typename obj, 
            typename R, 
            typename T0, 
            typename T1, 
            typename T2, 
            typename T3, 
            typename T4,
            typename T5,
            typename T6,
            typename T7>
    class SoraBind<obj, R(T0, T1, T2, T3, T4, T5, T6, T7)>: 
        public detail::MemfunContainer<obj, R(T0, T1, T2, T3, T4, T5, T6, T7)> {
            
    public:
        typedef detail::MemfunContainer<obj, R(T0, T1, T2, T3, T4, T5, T6, T7)> base_type;
        typedef typename base_type::ObjType ObjType;
        typedef typename base_type::MemfunPtr MemfunPtr;
        
        SoraBind(ObjType* object,
                 MemfunPtr memfun):
        base_type(object,
                  memfun) {
            
        }
        
        inline R operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) {
            return base_type::mMemfun(base_type::mObj, a0, a1, a2, a3, a4, a5, a6, a7);
        }
    };
    
    template<typename obj, 
            typename R, 
            typename T0,
            typename T1, 
            typename T2, 
            typename T3, 
            typename T4,
            typename T5,
            typename T6,
            typename T7,
            typename T8>
    class SoraBind<obj, R(T0, T1, T2, T3, T4, T5, T6, T7, T8)>: 
    public detail::MemfunContainer<obj, R(T0, T1, T2, T3, T4, T5, T6, T7, T8)> {
        
    public:
        typedef detail::MemfunContainer<obj, R(T0, T1, T2, T3, T4, T5, T6, T7, T8)> base_type;
        typedef typename base_type::ObjType ObjType;
        typedef typename base_type::MemfunPtr MemfunPtr;
        
        SoraBind(ObjType* object,
                 MemfunPtr memfun):
        base_type(object,
                  memfun) {
            
        }
        
        inline R operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) {
            return base_type::mMemfun(base_type::mObj, a0, a1, a2, a3, a4, a5, a6, a7, a8);
        }
    };
    
    template<typename obj, 
            typename R, 
            typename T0,
            typename T1, 
            typename T2, 
            typename T3, 
            typename T4,
            typename T5,
            typename T6,
            typename T7,
            typename T8,
            typename T9>
    class SoraBind<obj, R(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)>: 
    public detail::MemfunContainer<obj, R(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)> {
        
    public:
        typedef detail::MemfunContainer<obj, R(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)> base_type;
        typedef typename base_type::ObjType ObjType;
        typedef typename base_type::MemfunPtr MemfunPtr;
        
        SoraBind(ObjType* object,
                 MemfunPtr memfun):
        base_type(object,
                  memfun) {
            
        }
        
        inline R operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9) {
            return base_type::mMemfun(base_type::mObj, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
        }
    };
    
    template<typename obj, 
            typename R, 
            typename T0,
            typename T1, 
            typename T2, 
            typename T3, 
            typename T4,
            typename T5,
            typename T6,
            typename T7,
            typename T8,
            typename T9,
            typename T10>
    class SoraBind<obj, R(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)>: 
    public detail::MemfunContainer<obj, R(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)> {
        
    public:
        typedef detail::MemfunContainer<obj, R(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)> base_type;
        typedef typename base_type::ObjType ObjType;
        typedef typename base_type::MemfunPtr MemfunPtr;
        
        SoraBind(ObjType* object,
                 MemfunPtr memfun):
        base_type(object,
                  memfun) {
            
        }
        
        inline R operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10) {
            return base_type::mMemfun(base_type::mObj, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
        }
    };
    
    template<typename obj, typename R>
    SoraBind<obj, R(void)> Bind(obj* object, R (obj::*Memfun)()) {
        return SoraBind<obj, R(void)>(object, Memfun);
    }
    
    template<typename obj, typename R, typename T0>
    SoraBind<obj, R(T0)> Bind(obj* object, R (obj::*Memfun)(T0)) {
        return SoraBind<obj, R(T0)>(object, Memfun);
    }
    
    template<typename obj, typename R, typename T0, typename T1>
    SoraBind<obj, R(T0, T1)> Bind(obj* object, R (obj::*Memfun)(T0, T1)) {
        return SoraBind<obj, R(T0, T1)>(object, Memfun);
    }
    
    template<typename obj, typename R, typename T0, typename T1, typename T2>
    SoraBind<obj, R(T0, T1, T2)> Bind(obj* object, R (obj::*Memfun)(T0, T1, T2)) {
        return SoraBind<obj, R(T0, T1, T2)>(object, Memfun);
    }
    
    template<typename obj, typename R, typename T0, typename T1, typename T2, typename T3>
    SoraBind<obj, R(T0, T1, T2, T3)> Bind(obj* object, R (obj::*Memfun)(T0, T1, T2, T3)) {
        return SoraBind<obj, R(T0, T1, T2, T3)>(object, Memfun);
    }
    
    template<typename obj, typename R, typename T0, typename T1, typename T2, typename T3, typename T4>
    SoraBind<obj, R(T0, T1, T2, T3, T4)> Bind(obj* object, R (obj::*Memfun)(T0, T1, T2, T3, T4)) {
        return SoraBind<obj, R(T0, T1, T2, T3, T4)>(object, Memfun);
    }
    
    template<typename obj, typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    SoraBind<obj, R(T0, T1, T2, T3, T4, T5)> Bind(obj* object, R (obj::*Memfun)(T0, T1, T2, T3, T4, T5)) {
        return SoraBind<obj, R(T0, T1, T2, T3, T4, T5)>(object, Memfun);
    }
    
    template<typename obj, 
            typename R, 
            typename T0, 
            typename T1,
            typename T2, 
            typename T3, 
            typename T4,
            typename T5,
            typename T6>
    SoraBind<obj, R(T0, T1, T2, T3, T4, T5, T6)> Bind(obj* object, 
                                                      R (obj::*Memfun)(T0, T1, T2, T3, T4, T5, T6)) {
        return SoraBind<obj, R(T0, T1, T2, T3, T4, T5, T6)>(object, Memfun);
    }
    
    template<typename obj, 
            typename R, 
            typename T0, 
            typename T1,
            typename T2, 
            typename T3, 
            typename T4,
            typename T5,
            typename T6,
            typename T7>
    SoraBind<obj, R(T0, T1, T2, T3, T4, T5, T6, T7)> Bind(obj* object, 
                                                          R (obj::*Memfun)(T0, T1, T2, T3, T4, T5, T6, T7)) {
        return SoraBind<obj, R(T0, T1, T2, T3, T4, T5, T6, T7)>(object, Memfun);
    }
    
    template<typename obj, 
            typename R, 
            typename T0, 
            typename T1,
            typename T2, 
            typename T3, 
            typename T4,
            typename T5,
            typename T6,
            typename T7,
            typename T8>
    SoraBind<obj, R(T0, T1, T2, T3, T4, T5, T6, T7, T8)> Bind(obj* object, 
                                                              R (obj::*Memfun)(T0, T1, T2, T3, T4, T5, T6, T7, T8)) {
        return SoraBind<obj, R(T0, T1, T2, T3, T4, T5, T6, T7, T8)>(object, Memfun);
    }
    
    template<typename obj, 
            typename R, 
            typename T0, 
            typename T1,
            typename T2, 
            typename T3, 
            typename T4,
            typename T5,
            typename T6,
            typename T7,
            typename T8,
            typename T9>
    SoraBind<obj, R(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)> Bind(obj* object, 
                                                                  R (obj::*Memfun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)) {
        return SoraBind<obj, R(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)>(object, Memfun);
    }
    
    template<typename obj, 
            typename R, 
            typename T0, 
            typename T1,
            typename T2, 
            typename T3, 
            typename T4,
            typename T5,
            typename T6,
            typename T7,
            typename T8,
            typename T9,
            typename T10>
    SoraBind<obj, R(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)> Bind(obj* object, 
                                                                  R (obj::*Memfun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)) {
        return SoraBind<obj, R(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)>(object, Memfun);
    }
    
}


#endif
