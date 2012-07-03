//
//  SoraMutexImplWin32.h
//  Sora
//
//  Created by Robert Bu on 7/7/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraMutexImplWin32_h
#define Sora_SoraMutexImplWin32_h

#include "SoraPlatform.h"

#ifdef OS_WIN32

#include <windows.h>

#include "SoraException.h"
#include "SoraUncopyable.h"

namespace sora {
    
    class SoraMutexLockImpl: public SoraUncopyable {
    protected:
        SoraMutexLockImpl() {
            //InitializeCriticalSectionAndSpinCount(&cs, 4000);
			mutex = CreateMutexW(NULL,FALSE,NULL);
        }
        
        ~SoraMutexLockImpl() {
            //DeleteCriticalSection(&cs);
			CloseHandle(mutex);
        }
        
        inline void lockImpl() {
            try {
				WaitForSingleObject(mutex,INFINITE);
                //EnterCriticalSection(&cs);
            } catch(...) {
                THROW_SORA_EXCEPTION(RuntimeException, "Cannot lock mutex");
            }
        }
        
        inline void unlockImpl() {
            try {
				ReleaseMutex(mutex);
                //LeaveCriticalSection(&cs);
            } catch(...) {
                THROW_SORA_EXCEPTION(RuntimeException, "Cannot lock mutex");
            }
        }
        
        inline void* getSysMutexImpl() {
            return &mutex;
        }
        
    private:
        //CRITICAL_SECTION cs;
		HANDLE mutex;
    };
    
    
} // namespace sora

#endif //OS_WIN32

#endif
