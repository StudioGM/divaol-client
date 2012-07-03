//
//  SoraAutomicCounter.h
//  Sora
//
//  Created by Robert Bu on 7/7/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraAutomicCounter_h
#define Sora_SoraAutomicCounter_h

#include "SoraPlatform.h"
#include "SoraUncopyable.h"

#include "SoraMutex.h"

#ifdef OS_WIN32
#include <windows.h>
#else defined(OS_OSX)
#include <libkern/OSAtomic.h>
#endif

namespace sora {
    
    class SoraAtomicCounter {
    public:
        typedef int ValueType;
        
        SoraAtomicCounter();
        explicit SoraAtomicCounter(int initialValue);
        
        SoraAtomicCounter(const SoraAtomicCounter&);
        ~SoraAtomicCounter();
        
        SoraAtomicCounter& operator =(const SoraAtomicCounter& counter);
        SoraAtomicCounter& operator =(int value);
        
        operator int() const;
        
        int value() const;
        
        int operator++();
        int operator++(int);
        int operator--();
        int operator--(int);
        bool operator!() const;
        
    private:
#if defined(OS_WIN32)
        typedef volatile LONG ImplType;
#elif defined(OS_OSX)
        typedef int ImplType;
#else
        struct ImplType {
            mutable SoraMutexLock mutex;
            volatile int value;
            
            ImplType(int v=0):
            value(v) { }
            
            ImplType(const ImplType& rhs):
            value(rhs.value) {}
        };
#endif
        ImplType counter;
    };
    
#ifdef OS_WIN32
    inline SoraAtomicCounter::operator int() const {
        return counter;
    }
    
    inline int SoraAtomicCounter::value() const {
        return counter;
    }
    
    inline int SoraAtomicCounter::operator++() {
        return InterlockedIncrement(&counter);
    }
    
    inline int SoraAtomicCounter::operator++(int) {
        int result(counter);
        InterlockedIncrement(&counter);
        return result;
    }
    
    inline int SoraAtomicCounter::operator--() {
        return InterlockedDecrement(&counter);
    }
    
    inline int SoraAtomicCounter::operator--(int) {
        int result(counter);
        InterlockedDecrement(&counter);
        return result;
    }
    
    inline bool SoraAtomicCounter::operator !() const {
        return counter == 0;
    }
#elif defined(OS_OSX)
    inline SoraAtomicCounter::operator int() const {
        return counter;
    }
    
    inline int SoraAtomicCounter::value() const {
        return counter;
    }
    
    inline int SoraAtomicCounter::operator++() {
        return OSAtomicIncrement32(&counter);
    }
    
    inline int SoraAtomicCounter::operator++(int) {
        int result(counter);
        OSAtomicIncrement32(&counter);
        return result;
    }
    
    inline int SoraAtomicCounter::operator--() {
        return OSAtomicDecrement32(&counter);
    }
    
    inline int SoraAtomicCounter::operator--(int) {
        int result(counter);
        OSAtomicDecrement32(&counter);
        return result;
    }
    
    inline bool SoraAtomicCounter::operator !() const {
        return counter == 0;
    }
#else
    inline SoraAtomicCounter::operator int() const {
        int result;
        {
            SoraMutexGuard lock(counter.mutex);
            result = counter.value;
        }
        return result;
    }
    
    inline int SoraAtomicCounter::value() const {
        int result;
        {
            SoraMutexGuard lock(counter.mutex);
            result = counter.value;
        }
        return result;
    }
    
    inline int SoraAtomicCounter::operator++() {
        int result;
        {
            SoraMutexGuard lock(counter.mutex);
            result = ++counter.value;
        }
        return result;
    }
    
    inline int SoraAtomicCounter::operator++(int) {
        int result;
        {
            SoraMutexGuard lock(counter.mutex);
            result = counter.value++;
        }
        return result;
    }
    
    inline int SoraAtomicCounter::operator--() {
        int result;
        {
            SoraMutexGuard lock(counter.mutex);
            result = --counter.value;
        }
        return result;
    }
    
    inline int SoraAtomicCounter::operator--(int) {
        int result;
        {
            SoraMutexGuard lock(counter.mutex);
            result = counter.value--;
        }
        return result;
    }
    
    inline bool SoraAtomicCounter::operator !() const {
        bool result;
        {
            SoraMutexGuard lock(counter.mutex);
            result = counter.value == 0;
        }
        return result;
    }
    
#endif //
    
}


#endif
