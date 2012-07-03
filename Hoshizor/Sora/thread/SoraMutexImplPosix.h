//
//  SoraMutexImplPosix.h
//  Sora
//
//  Created by Robert Bu on 7/7/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraMutexImplPosix_h
#define Sora_SoraMutexImplPosix_h

#include "SoraPlatform.h"

#include "SoraUncopyable.h"
#include "SoraException.h"

#ifndef OS_WIN32
#include <pthread.h>
#else
#include "pthread.h"
#endif

namespace sora {
    
    class SoraMutexLockImpl: SoraUncopyable {
    protected:
        SoraMutexLockImpl() {
            pthread_mutex_init(&mutex, NULL);
        }
        
        ~SoraMutexLockImpl() {
            pthread_mutex_destroy(&mutex);
        }
        
        inline void lockImpl() {
            if(pthread_mutex_lock(&mutex))
                THROW_SORA_EXCEPTION(RuntimeException, "Cannot lock mutex");
        }
        
        inline void unlockImpl() {
            if(pthread_mutex_unlock(&mutex))
                THROW_SORA_EXCEPTION(RuntimeException, "Cannot lock mutex");
        }
        
        inline void* getSysMutexImpl() {
            return &mutex;
        }
        
    private:
        pthread_mutex_t mutex;
    };

    
} // namespace sora

#endif
