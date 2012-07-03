//
//  SoraBlockingQueue.h
//  Sora
//
//  Created by Robert Bu on 7/6/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraBlockingQueue_h
#define Sora_SoraBlockingQueue_h

#include "SoraPlatform.h"

#include "SoraMutex.h"
#include "SoraCondition.h"

#include <cassert>
#include <deque>

namespace sora {
    
    template<typename T>
    class SoraBlockingQueue: SoraUncopyable {
    public:
        SoraBlockingQueue():
        mutex(),
        cond(mutex),
        queue() {
        }
        
        void put(const T& x) {
            SoraMutexGuard lock(mutex);
            queue.push_back(x);
            cond.notify();
        }
        
        T take() {
            while(queue.empty())  
                cond.wait();
            
            SoraMutexGuard lock(mutex);
            assert(!queue.empty());
            T front(queue.front());
            queue.pop_front();
            return front;
        }
        
        size_t size() const {
            SoraMutexGuard lock(mutex);
            return queue.size();
        }
        
    private:
        mutable SoraMutexLock mutex;
        SoraCondition cond;
        
        std::deque<T> queue;
    };
    
} // namespace sora

#endif
