//
//  SoraCondition.cpp
//  Sora
//
//  Created by Robert Bu on 7/8/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include <iostream>

#include "SoraCondition.h"

namespace sora {
    
    SoraCondition::~SoraCondition() {
#if !defined(OS_WIN32) || defined(SORA_WIN32_PTHREAD)
        pthread_cond_destroy(&cond);
#else
        //_win32_pthread_cond_destroy(&cond);
#endif
    }
    
    void SoraCondition::wait() {
#if !defined(OS_WIN32) || defined(SORA_WIN32_PTHREAD)
        if(pthread_cond_wait(&cond,
                             static_cast<pthread_mutex_t*>(_mutex.getSysMutex())))
            THROW_SORA_EXCEPTION(RuntimeException, "Error wait");
#else
        if(_win32_pthread_cond_wait(&cond,
                                    static_cast<pthread_mutex_t*>(_mutex.getSysMutex())))
            THROW_SORA_EXCEPTION(RuntimeException, "Error wait");
#endif
    }
    
    void SoraCondition::notify() {
#if !defined(OS_WIN32) || defined(SORA_WIN32_PTHREAD)
        if(pthread_cond_signal(&cond))
            THROW_SORA_EXCEPTION(RuntimeException, "Error notify");
#else
        if(_win32_pthread_cond_signal(&cond))
            THROW_SORA_EXCEPTION(RuntimeException, "Error notify");
#endif
    }
    
    void SoraCondition::notifyAll() {
#if !defined(OS_WIN32) || defined(SORA_WIN32_PTHREAD)
        if(pthread_cond_broadcast(&cond))
            THROW_SORA_EXCEPTION(RuntimeException, "Error notify all");
#else
        if(_win32_pthread_cond_broadcast(&cond))
            THROW_SORA_EXCEPTION(RuntimeException, "Error notify all");
#endif
    }
    
} // namespace sora