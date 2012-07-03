//
//  SoraSemaphore.h
//  Sora
//
//  Created by Robert Bu on 7/8/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraSemaphore_h
#define Sora_SoraSemaphore_h

#include "SoraPlatform.h"

#if !defined(OS_WIN32) || defined(SORA_WIN32_PTHREAD)
#include "SoraSemaphoreImplPosix.h"
#else
#include "SoraSemaphoreImplWin32.h"
#endif

namespace sora {
    
    class SoraSemaphore: public SoraSemaphoreImpl {
        SoraSemaphore(int n): SoraSemaphoreImpl(n, n) { }
        SoraSemaphore(int n, int max): SoraSemaphoreImpl(n, max) { }
        ~SoraSemaphore() { }
        
        void wait() {
            waitImpl();
        }
        
        void set() {
            setImpl();
        }
        
    private:
        
    };
    
} // namespace sora

#endif
