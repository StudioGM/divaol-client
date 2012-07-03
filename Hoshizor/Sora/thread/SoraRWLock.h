//
//  SoraRWLock.h
//  Sora
//
//  Created by Robert Bu on 7/7/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraRWLock_h
#define Sora_SoraRWLock_h

#include "SoraPlatform.h"

/*

 */
#if !defined(OS_WIN32) || defined(SORA_WIN32_PTHREAD)
#include "SoraRWLockImplPosix.h"
#else
#include "SoraRWLockImplWin32.h"
#endif

namespace sora {
    
    class SoraRWLock: public SoraRWLockImpl {
        SoraRWLock() {}
        ~SoraRWLock() {}
        
        void readLock() {
            readLockImpl();
        }
        
        void writeLock() {
            writeLockImpl();
        }
        
        void unlock() {
            unlockImpl();
        }
    };
    
} // namespace sora



#endif
