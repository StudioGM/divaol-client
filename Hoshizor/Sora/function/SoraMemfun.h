//
//  SoraMemfun.h
//  zeromq_client
//
//  Created by Ruiwei Bu on 8/15/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef SoraMemfun_h
#define SoraMemfun_h

namespace sora {
    
    template<typename sig, typename obj>
    class SoraMemfun;
    
    template<typename R, typename obj>
    class SoraMemfun<R(void), obj> {
    public:
        typedef obj ObjType;
        typedef R ReturnType;
        
        typedef R (obj::*Memfun)(void);
        
        explicit SoraMemfun(Memfun func):
        mFunc(func) { }
        
        inline R invoke(obj* left) {
            return (left->*mFunc)();
        }
        
        inline R operator()(obj* left) {
            return (left->*mFunc)();
        }
        
        operator bool() {
            return mFunc != NULL;
        }
        
    private:
        Memfun mFunc;
    };
    
    template<typename R, 
             typename T0, 
             typename obj>
    class SoraMemfun<R(T0), obj> {
    public:
        typedef obj ObjType;
        typedef R ReturnType;
        
        typedef R (obj::*Memfun)(T0);
        
        explicit SoraMemfun(Memfun func):
        mFunc(func) { }
        
        inline R invoke(obj* left, T0 a0) {
            return (left->*mFunc)(a0);
        }

        inline R operator()(obj* left, T0 a0) {
            return (left->*mFunc)(a0);
        }
        
        operator bool() {
            return mFunc != NULL;
        }
        
    private:
        Memfun mFunc;
    };
    
    template<typename R, 
             typename T0, 
             typename T1, 
             typename obj>
    class SoraMemfun<R(T0, T1), obj> {
    public:
        typedef obj ObjType;
        typedef R ReturnType;
        
        typedef R (obj::*Memfun)(T0, T1);
        
        explicit SoraMemfun(Memfun func):
        mFunc(func) { }
        
        inline R invoke(obj* left, T0 a0, T1 a1) {
            return (left->*mFunc)(a0, a1);
        }
        
        inline R operator()(obj* left, T0 a0, T1 a1) {
            return (left->*mFunc)(a0, a1);
        }
        
        operator bool() {
            return mFunc != NULL;
        }
        
    private:
        Memfun mFunc;
    };
    
    template<typename R, 
             typename T0, 
             typename T1, 
             typename T2, 
             typename obj>
    class SoraMemfun<R(T0, T1, T2), obj> {
    public:
        typedef obj ObjType;
        typedef R ReturnType;
        
        typedef R (obj::*Memfun)(T0, T1, T2);
        
        explicit SoraMemfun(Memfun func):
        mFunc(func) { }
        
        inline R invoke(obj* left, T0 a0, T1 a1, T2 a2) {
            return (left->*mFunc)(a0, a1, a2);
        }
        
        inline R operator()(obj* left, T0 a0, T1 a1, T2 a2) {
            return (left->*mFunc)(a0, a1, a2);
        }
        
        operator bool() {
            return mFunc != NULL;
        }
        
    private:
        Memfun mFunc;
    };
    
    template<typename R, 
             typename T0, 
             typename T1,
             typename T2, 
             typename T3, 
             typename obj>
    class SoraMemfun<R(T0, T1, T2, T3), obj> {
    public:
        typedef obj ObjType;
        typedef R ReturnType;
        
        typedef R (obj::*Memfun)(T0, T1, T2, T3);
        
        explicit SoraMemfun(Memfun func):
        mFunc(func) { }
        
        inline R invoke(obj* left, T0 a0, T1 a1, T2 a2, T3 a3) {
            return (left->*mFunc)(a0, a1, a2, a3);
        }
        
        inline R operator()(obj* left, T0 a0, T1 a1, T2 a2, T3 a3) {
            return (left->*mFunc)(a0, a1, a2, a3);
        }
        
        operator bool() {
            return mFunc != NULL;
        }
        
    private:
        Memfun mFunc;
    };
    
    template<typename R, 
             typename T0, 
             typename T1, 
             typename T2, 
             typename T3, 
             typename T4, 
             typename obj>
    class SoraMemfun<R(T0, T1, T2, T3, T4), obj> {
    public:
        typedef obj ObjType;
        typedef R ReturnType;
        
        typedef R (obj::*Memfun)(T0, T1, T2, T3, T4);
        
        explicit SoraMemfun(Memfun func):
        mFunc(func) { }
        
        inline R operator()(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4) {
            return (left->*mFunc)(a0, a1, a2, a3, a4);
        }
        
        inline R invoke(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4) {
            return (left->*mFunc)(a0, a1, a2, a3, a4);
        }
        
        operator bool() {
            return mFunc != NULL;
        }
        
    private:
        Memfun mFunc;
    };
    
    template<typename R, 
             typename T0, 
             typename T1, 
             typename T2, 
             typename T3, 
             typename T4, 
             typename T5, 
             typename obj>
    class SoraMemfun<R(T0, T1, T2, T3, T4, T5), obj> {
    public:
        typedef obj ObjType;
        typedef R ReturnType;
        
        typedef R (obj::*Memfun)(T0, T1, T2, T3, T4, T5);
        
        explicit SoraMemfun(Memfun func):
        mFunc(func) { }
        
        inline R operator()(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
            return (left->*mFunc)(a0, a1, a2, a3, a4, a5);
        }
        
        inline R invoke(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
            return (left->*mFunc)(a0, a1, a2, a3, a4, a5);
        }
        
        operator bool() {
            return mFunc != NULL;
        }
        
    private:
        Memfun mFunc;
    };
    
    template<typename R, 
             typename T0, 
             typename T1, 
             typename T2, 
             typename T3, 
             typename T4, 
             typename T5, 
             typename T6, 
             typename obj>
    class SoraMemfun<R(T0, T1, T2, T3, T4, T5, T6), obj> {
    public:
        typedef R (obj::*Memfun)(T0, T1, T2, T3, T4, T5, T6);
        
        explicit SoraMemfun(Memfun func):
        mFunc(func) { }
        
        inline R operator()(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) {
            return (left->*mFunc)(a0, a1, a2, a3, a4, a5, a6);
        }
        
        inline R invoke(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) {
            return (left->*mFunc)(a0, a1, a2, a3, a4, a5, a6);
        }
        
        operator bool() {
            return mFunc != NULL;
        }
        
    private:
        Memfun mFunc;
    };
    
    template<typename R, 
             typename T0, 
             typename T1, 
             typename T2, 
             typename T3, 
             typename T4, 
             typename T5, 
             typename T6, 
             typename T7,
             typename obj>
    class SoraMemfun<R(T0, T1, T2, T3, T4, T5, T6, T7), obj> {
    public:
        typedef obj ObjType;
        typedef R ReturnType;
        
        typedef R (obj::*Memfun)(T0, T1, T2, T3, T4, T5, T6, T7);
        
        explicit SoraMemfun(Memfun func):
        mFunc(func) { }
        
        inline R operator()(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) {
            return (left->*mFunc)(a0, a1, a2, a3, a4, a5, a6, a7);
        }
        
        inline R invoke(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) {
            return (left->*mFunc)(a0, a1, a2, a3, a4, a5, a6, a7);
        }
        
        operator bool() {
            return mFunc != NULL;
        }
        
    private:
        Memfun mFunc;
    };
    
    template<typename R, 
             typename T0, 
             typename T1, 
             typename T2, 
             typename T3, 
             typename T4, 
             typename T5, 
             typename T6, 
             typename T7,
             typename T8,
             typename obj>
    class SoraMemfun<R(T0, T1, T2, T3, T4, T5, T6, T7, T8), obj> {
    public:
        typedef obj ObjType;
        typedef R ReturnType;
        
        typedef R (obj::*Memfun)(T0, T1, T2, T3, T4, T5, T6, T7, T8);
        
        explicit SoraMemfun(Memfun func):
        mFunc(func) { }
        
        inline R operator()(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) {
            return (left->*mFunc)(a0, a1, a2, a3, a4, a5, a6, a7, a8);
        }
        
        inline R invoke(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) {
            return (left->*mFunc)(a0, a1, a2, a3, a4, a5, a6, a7, a8);
        }
        
        operator bool() {
            return mFunc != NULL;
        }
        
    private:
        Memfun mFunc;
    };
        
    template<typename R, 
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
            typename obj>
    class SoraMemfun<R(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9), obj> {
    public:
        typedef obj ObjType;
        typedef R ReturnType;
        
        typedef R (obj::*Memfun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9);
        
        explicit SoraMemfun(Memfun func):
        mFunc(func) { }
        
        inline R operator()(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9) {
            return (left->*mFunc)(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
        }
        
        inline R invoke(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9) {
            return (left->*mFunc)(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
        }
        
        operator bool() {
            return mFunc != NULL;
        }
        
    private:
        Memfun mFunc;
    };
    
    template<typename R, 
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
            typename T10,
            typename obj>
    class SoraMemfun<R(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10), obj> {
    public:
        typedef obj ObjType;
        typedef R ReturnType;
        
        typedef R (obj::*Memfun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10);
        
        explicit SoraMemfun(Memfun func):
        mFunc(func) { }
        
        inline R operator()(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10) {
            return (left->*mFunc)(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
        }
        
        inline R invoke(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10) {
            return (left->*mFunc)(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
        }
        
        operator bool() {
            return mFunc != NULL;
        }
        
    private:
        Memfun mFunc;
    };
    
    template<typename R, typename obj>
    inline SoraMemfun<R(), obj> MemFun(R (obj::*Memfun)()) {
        return SoraMemfun<R(), obj>(Memfun);
    }
    
    template<typename R, typename T0, typename obj>
    inline SoraMemfun<R(T0), obj> MemFun(R (obj::*Memfun)(T0)) {
        return SoraMemfun<R(T0), obj>(Memfun);
    }
    
    template<typename R, typename T0, typename T1, typename obj>
    inline SoraMemfun<R(T0, T1), obj> MemFun(R (obj::*Memfun)(T0, T1)) {
        return SoraMemfun<R(T0, T1), obj>(Memfun);
    }
    
    template<typename R, typename T0, typename T1, typename T2, typename obj>
    inline SoraMemfun<R(T0, T1, T2), obj> MemFun(R (obj::*Memfun)(T0, T1, T2)) {
        return SoraMemfun<R(T0, T1, T2), obj>(Memfun);
    }
    
    template<typename R, typename T0, typename T1, typename T2, typename T3, typename obj>
    inline SoraMemfun<R(T0, T1, T2, T3), obj> MemFun(R (obj::*Memfun)(T0, T1, T2, T3)) {
        return SoraMemfun<R(T0, T1, T2, T3), obj>(Memfun);
    }
    
    template<typename R, 
             typename T0, 
             typename T1, 
             typename T2, 
             typename T3, 
             typename T4,
             typename obj>
    inline SoraMemfun<R(T0, T1, T2, T3, T4), obj> MemFun(R (obj::*Memfun)(T0, T1, T2, T3, T4)) {
        return SoraMemfun<R(T0, T1, T2, T3, T4), obj>(Memfun);
    }
    
    template<typename R, 
            typename T0, 
            typename T1, 
            typename T2, 
            typename T3, 
            typename T4,
            typename T5,
            typename obj>
    inline SoraMemfun<R(T0, T1, T2, T3, T4, T5), obj> MemFun(R (obj::*Memfun)(T0, T1, T2, T3, T4, T5)) {
        return SoraMemfun<R(T0, T1, T2, T3, T4, T5), obj>(Memfun);
    }
    
    template<typename R, 
            typename T0, 
            typename T1, 
            typename T2, 
            typename T3, 
            typename T4,
            typename T5,
            typename T6,
            typename obj>
    inline SoraMemfun<R(T0, T1, T2, T3, T4, T5, T6), obj> MemFun(R (obj::*Memfun)(T0, T1, T2, T3, T4, T5, T6)) {
        return SoraMemfun<R(T0, T1, T2, T3, T4, T5, T6), obj>(Memfun);
    }
    
    template<typename R, 
            typename T0, 
            typename T1, 
            typename T2, 
            typename T3, 
            typename T4,
            typename T5,
            typename T6,
            typename T7,
            typename obj>
    inline SoraMemfun<R(T0, T1, T2, T3, T4, T5, T6, T7), obj> MemFun(R (obj::*Memfun)(T0, T1, T2, T3, T4, T5, T6, T7)) {
        return SoraMemfun<R(T0, T1, T2, T3, T4, T5, T6, T7), obj>(Memfun);
    }
    
    template<typename R, 
            typename T0, 
            typename T1, 
            typename T2, 
            typename T3, 
            typename T4,
            typename T5,
            typename T6,
            typename T7,
            typename T8,
            typename obj>
    inline SoraMemfun<R(T0, T1, T2, T3, T4, T5, T6, T7, T8), obj> MemFun(R (obj::*Memfun)(T0, T1, T2, T3, T4, T5, T6, T7, T8)) {
        return SoraMemfun<R(T0, T1, T2, T3, T4, T5, T6, T7, T8), obj>(Memfun);
    }
    
    template<typename R, 
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
            typename obj>
    inline SoraMemfun<R(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9), obj> MemFun(R (obj::*Memfun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)) {
        return SoraMemfun<R(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9), obj>(Memfun);
    }
    
    template<typename R, 
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
            typename T10,
            typename obj>
    inline SoraMemfun<R(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10), obj> MemFun(R (obj::*Memfun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)) {
        return SoraMemfun<R(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10), obj>(Memfun);
    }
} // namespace sora

#endif
