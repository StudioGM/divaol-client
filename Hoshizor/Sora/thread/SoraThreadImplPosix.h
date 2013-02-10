//
//  SoraThreadImplPosix.h
//  Sora
//
//  Created by Robert Bu on 7/7/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraThreadImplPosix_h
#define Sora_SoraThreadImplPosix_h

#include "SoraPlatform.h"
#include "SoraThreadTask.h"

#if !defined(OS_WIN32) || defined(SORA_WIN32_PTHREAD)

// sora thread using pthread(posix thread)
#ifndef OS_WIN32
#include <pthread.h>
#else
#include "win32/pthread.h"
#endif

namespace sora {
    
    class SoraThreadImpl: SoraUncopyable {
        friend class SoraThread;
        
    protected:
        SoraThreadImpl(): active(false) {}
        
        ~SoraThreadImpl() {
            exitImpl();
        }
    
        inline int32 startImpl() {
            if(!thread_task.isValid() || active) {
                return 0;
            }
            
            int32 result = pthread_create(&thread, NULL, SoraThreadImpl::entry, this);
            return result;
        }
        
        inline int32 startWithTaskImpl(const SoraThreadTask& task) {
            setThreadTaskImpl(task);
            return startImpl();
        }
        
        inline void joinImpl() {
            if(active) {
                pthread_join(thread, 0);
                setActiveImpl(false);
            }
        }
        
        inline void exitImpl() {
            if(active) {
                pthread_exit(&thread);
                setActiveImpl(false);
            }
        }
        
        inline bool isActiveImpl() const {
            return active;
        }
       
        inline void setThreadTaskImpl(const SoraThreadTask& task) {
            thread_task = task;
        }
        
        inline SoraThreadTask getThreadTaskImpl() const {
            return thread_task;
        }
        
        static void* entry(void* pthis);
        
        inline void setActiveImpl(bool flag) {
            active = flag;
        }
        
    private:
		pthread_t thread;
		pthread_attr_t attr;
        
        SoraThreadTask thread_task;
        
        bool active;
    };

    
    
} // namespace sora

#endif

#endif
