//
//  SoraRWLockImplWin32.h
//  Sora
//
//  Created by Robert Bu on 7/7/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraRWLockImplWin32_h
#define Sora_SoraRWLockImplWin32_h

#include "SoraPlatform.h"
#include "SoraException.h"

#ifdef OS_WIN32

#include <windows.h>

namespace sora {
    
    class SoraRWLockImpl: public SoraUncopyable {
    protected:
        SoraRWLockImpl():
        readers(0),
        writersWaiting(0),
        writers(0) {
            mutex = CreateMutexW(NULL, FALSE, NULL);
            if(mutex != NULL) {
                readEvent = CreateEventW(NULL, TRUE, TRUE, NULL);
                if(readEvent != NULL) {
                    writeEvent = CreateEventW(NULL, TRUE, TRUE, NULL);
                }
            }
        }
        ~SoraRWLockImpl() {
            CloseHandle(mutex);
            CloseHandle(readEvent);
            Closehandle(writeEvent);
        }
        
        inline void readLockImpl() {
            HANDLE h[2];
            h[0] = mutex;
            h[1] = readEvent;
            switch(WaitForMultipleObjects(2, h, TRUE, INFINITE)) {
                case WAIT_OBJECT_0:
                case WAIT_OBJECT_0+1:
                    ++readers;
                    ResetEvent(writeEvent);
                    ReleaseMutex(mutex);
                    break;
                default:
                    THROW_SORA_EXCEPTION(RuntimeException, "Cannot lock reader/writer lock");
            }
        }
        
        inline void writeLockImpl() {
            addWriter();
            HANDLE h[2];
            h[0] = mutex;
            h[1] = writeEvent;
            switch(WaitForMultipleObjects(2, h, TRUE, INFINITE)) {
                case WAIT_OBJECT_0:
                case WAIT_OBJECT_0+1:
                    --writersWaiting;
                    ++readers;
                    ++writers;
                    ResetEvent(readEvent);
                    ResetEvent(writeEvent);
                    ReleaseMutex(mutex);
                    break;
                default:
                    removeWriter();
                    THROW_SORA_EXCEPTION(RuntimeException, "Cannot lock reader/writer lock");
            }
        }
        
        inline void unlockImpl() {
            switch(WaitForSingleObject(mutex, INFINITE)) {
                case WAIT_OBJECT_0:
                    writers = 0;
                    if(writersWaiting == 0)
                        SetEvent(readEvent);
                    if(--readers == 0)
                        SetEvent(writeEvent);
                    ReleaseMutex(mutex);
                    break;
                default:
                    THROW_SORA_EXCEPTION(RuntimeException, "Cannot unlock reader/writer lock");
            }
        }
        
    private:
        inline bool isValid() const {
            return mutex && readEvent && writeEvent;
        }
        
        inline void addWriter() {
            switch(WaitForSingleObject(mutex, INFINITE)) {
                case WAIT_OBJECT_0:
                    if(++writersWaiting == 1)
                        ResetEvent(readEvent);
                    ReleaseMutex(mutex);
                    break;
                default:
                    THROW_SORA_EXCEPTION(RuntimeException, "Cannot lock reader/writer lock");
            }
        }
        
        inline void removeWriter() {
            switch(WaitForSingleObject(mutex, INFINITE)) {
                case WAIT_OBJECT_0:
                    if(--writersWaiting == 0 && writes == 0)
                        SetEvent(readEvent);
                    ReleaseMutex(mutex);
                    break;
                default:
                    THROW_SORA_EXCEPTION(RuntimeException, "Cannot lock reader/writers lock");
            }
        }
        
        HANDLE mutex;
        HANDLE readEvent;
        HANDLE writeEvent;
        unsigned readers;
        unsigned writersWaiting;
        unsigned writers;
    };
    
    
} // namespace sora

#endif //OS_WIN32

#endif
