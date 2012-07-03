//
//  SoraThreadLocal.h
//  Sora
//
//  Created by Robert Bu on 7/6/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraThreadLocal_h
#define Sora_SoraThreadLocal_h

#include "SoraPlatform.h"

#ifndef OS_WIN32
#include <pthread.h>
#else
#include "pthread.h"
#endif

#include "SoraUncopyable.h"

namespace sora {
    
    template<typename T>
    class SoraThreadLocal: SoraUncopyable {
    public:
        SoraThreadLocal() {
            pthread_key_create(&key, &SoraThreadLocal::destructor);
        }
        
        ~SoraThreadLocal() {
            pthread_key_delete(key);
        }
        
        T& value() {
            T* threadValue = static_cast<T*>(pthread_getspecific(key));
            if(!threadValue) {
                T* newobj = new T();
                pthread_setspecific(key, newobj);
                threadValue = newobj;
            }
            return *threadValue;
        }
        
    private:
        static void destructor(void* x) {
            T* obj = static_cast<T*>(x);
            delete obj;
        }
        
        pthread_key_t key;
    };
    
} // namespace sora

#endif
