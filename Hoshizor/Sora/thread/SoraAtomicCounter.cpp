//
//  SoraAtomicCounter.cpp
//  Sora
//
//  Created by Robert Bu on 7/7/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraAtomicCounter.h"

namespace sora {
    
#ifdef OS_WIN32
    SoraAtomicCounter::SoraAtomicCounter(): counter(0) {
    }
    
    SoraAtomicCounter::SoraAtomicCounter(int value): counter(value) {
    }
    
    SoraAtomicCounter::SoraAtomicCounter(const SoraAtomicCounter& _counter):
    counter(_counter.value()) {
    }
    
    SoraAtomicCounter::~SoraAtomicCounter() {
        
    }
    
    SoraAtomicCounter& SoraAtomicCounter::operator=(const SoraAtomicCounter& rhs) {
        InterlockedExchange(&counter, rhs.value());
        return *this;
    }
    
    SoraAtomicCounter& SoraAtomicCounter::operator=(int value) {
        InterlockedExchange(&counter, value);
        return *this;
    }
    
#elif defined(OS_OSX)
    SoraAtomicCounter::SoraAtomicCounter(): counter(0) {
    }
    
    SoraAtomicCounter::SoraAtomicCounter(int value): counter(value) {
    }
    
    SoraAtomicCounter::SoraAtomicCounter(const SoraAtomicCounter& _counter):
    counter(_counter.value()) {
    }
    
    SoraAtomicCounter::~SoraAtomicCounter() {
        
    }
    
    SoraAtomicCounter& SoraAtomicCounter::operator=(const SoraAtomicCounter& rhs) {
        counter = rhs.value();
        return *this;
    }
    
    SoraAtomicCounter& SoraAtomicCounter::operator=(int value) {
        counter = value;
        return *this;
    }
    
#else 
    SoraAtomicCounter::SoraAtomicCounter(): counter(0) {
    }
    
    SoraAtomicCounter::SoraAtomicCounter(int value): counter(value) {
    }
    
    SoraAtomicCounter::SoraAtomicCounter(const SoraAtomicCounter& _counter):
    counter(_counter.counter) {
    }
    
    SoraAtomicCounter::~SoraAtomicCounter() {
        
    }
    
    SoraAtomicCounter& SoraAtomicCounter::operator=(const SoraAtomicCounter& rhs) {
        SoraMutexGuard lock(counter.mutex);
        counter.value = rhs.value();
        return *this;
    }
    
    SoraAtomicCounter& SoraAtomicCounter::operator=(int value) {
        SoraMutexGuard lock(counter.mutex);
        counter.value = value;
        return *this;
    }
    
#endif
    
}
