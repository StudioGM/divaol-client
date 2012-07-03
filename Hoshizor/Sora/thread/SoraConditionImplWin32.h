//
//  SoraConditionImplWin32.h
//  Sora
//
//  Created by Robert Bu on 7/7/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraConditionImplWin32_h
#define Sora_SoraConditionImplWin32_h

#include "SoraPlatform.h"

#ifdef OS_WIN32

#include <windows.h>

/**
 * This implemention is far more complex than the POSIX one
 * See http://www1.cse.wustl.edu/~schmidt/win32-cv-1.html for different ways of pthread_cond_* implemention on Windows
 **/

namespace sora {
    
    typedef struct {
        int waiters_count_;
        // Number of waiting threads.
        
        CRITICAL_SECTION waiters_count_lock_;
        // Serialize access to <waiters_count_>.
        
        HANDLE sema_;
        // Semaphore used to queue up threads waiting for the condition to
        // become signaled. 
        
        HANDLE waiters_done_;
        // An auto-reset event used by the broadcast/signal thread to wait
        // for all the waiting thread(s) to wake up and be released from the
        // semaphore. 
        
        size_t was_broadcast_;
        // Keeps track of whether we were broadcasting or signaling.  This
        // allows us to optimize the code if we're just signaling.
    } pthread_cond_t;
    
    typedef HANDLE pthread_mutex_t;
    
    static int 
    _win32_pthread_cond_init (pthread_cond_t *cv,
                       const void *)
    {
        cv->waiters_count_ = 0;
        cv->was_broadcast_ = 0;
        cv->sema_ = CreateSemaphoreA (NULL,       // no security
                                      0,          // initially 0
                                      0x7fffffff, // max count
                                      NULL);      // unnamed 
        InitializeCriticalSection (&cv->waiters_count_lock_);
        cv->waiters_done_ = CreateEventA (NULL,  // no security
                                         FALSE, // auto-reset
                                         FALSE, // non-signaled initially
                                         NULL); // unnamed
		return 0;
    }
    
    static int
    _win32_pthread_cond_wait (pthread_cond_t *cv, 
                       pthread_mutex_t *external_mutex)
    {
        // Avoid race conditions.
        EnterCriticalSection (&cv->waiters_count_lock_);
        cv->waiters_count_++;
        LeaveCriticalSection (&cv->waiters_count_lock_);
        
        // This call atomically releases the mutex and waits on the
        // semaphore until <pthread_cond_signal> or <pthread_cond_broadcast>
        // are called by another thread.

		//LeaveCriticalSection((LPCRITICAL_SECTION)external_mutex);
        SignalObjectAndWait (*external_mutex, cv->sema_, INFINITE, FALSE);

        // Reacquire lock to avoid race conditions.
        EnterCriticalSection (&cv->waiters_count_lock_);
        
        // We're no longer waiting...
        cv->waiters_count_--;
        
        // Check to see if we're the last waiter after <pthread_cond_broadcast>.
        int last_waiter = cv->was_broadcast_ && cv->waiters_count_ == 0;
        
        LeaveCriticalSection (&cv->waiters_count_lock_);
        
        // If we're the last waiter thread during this particular broadcast
        // then let all the other threads proceed.
        if (last_waiter)
		{
            // This call atomically signals the <waiters_done_> event and waits until
            // it can acquire the <external_mutex>.  This is required to ensure fairness. 
            SignalObjectAndWait (cv->waiters_done_, *external_mutex, INFINITE, FALSE);
			//EnterCriticalSection((LPCRITICAL_SECTION)external_mutex);
		}
        else
		{
            // Always regain the external mutex since that's the guarantee we
            // give to our callers. 
            WaitForSingleObject (*external_mutex, INFINITE);
			//EnterCriticalSection((LPCRITICAL_SECTION)external_mutex);
		}
		return 0;
    }
    
    static int
    _win32_pthread_cond_signal (pthread_cond_t *cv)
    {
        EnterCriticalSection (&cv->waiters_count_lock_);
        int have_waiters = cv->waiters_count_ > 0;
        LeaveCriticalSection (&cv->waiters_count_lock_);
        
        // If there aren't any waiters, then this is a no-op.  
        if (have_waiters)
            ReleaseSemaphore (cv->sema_, 1, 0);
		return 0;
    }
    
    static int
    _win32_pthread_cond_broadcast (pthread_cond_t *cv)
    {
        // This is needed to ensure that <waiters_count_> and <was_broadcast_> are
        // consistent relative to each other.
        EnterCriticalSection (&cv->waiters_count_lock_);
        int have_waiters = 0;
        
        if (cv->waiters_count_ > 0) {
            // We are broadcasting, even if there is just one waiter...
            // Record that we are broadcasting, which helps optimize
            // <pthread_cond_wait> for the non-broadcast case.
            cv->was_broadcast_ = 1;
            have_waiters = 1;
        }
        
        if (have_waiters) {
            // Wake up all the waiters atomically.
            ReleaseSemaphore (cv->sema_, cv->waiters_count_, 0);
            
            LeaveCriticalSection (&cv->waiters_count_lock_);
            
            // Wait for all the awakened threads to acquire the counting
            // semaphore. 
            WaitForSingleObject (cv->waiters_done_, INFINITE);
            // This assignment is okay, even without the <waiters_count_lock_> held 
            // because no other waiter threads can wake up to access it.
            cv->was_broadcast_ = 0;
        }
        else
            LeaveCriticalSection (&cv->waiters_count_lock_);
		return 0;
    }
} // namespace sora

#endif // OS_WIN32

#endif
