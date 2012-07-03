//
//  SoraThreadPool.h
//  Sora
//
//  Created by Robert Bu on 7/6/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraThreadPool_h
#define Sora_SoraThreadPool_h

#include "SoraPlatform.h"

#include "SoraMutex.h"
#include "SoraCondition.h"
#include "SoraThreadTask.h"
#include "SoraThread.h"

#include <vector>
#include <deque>

namespace sora {
        
    class SoraThreadPool: SoraUncopyable {
    public:
        explicit SoraThreadPool(const std::string& name = std::string());
        ~SoraThreadPool();
        
        void start(int32 numThreads);
        void stop();
        
        void run(const SoraThreadTask&);
        
        bool isRunning() const;
        
        static SoraThreadPool& defaultPool();
        
    private:
        void runInThread(void* arg);
        SoraThreadTask take();
        
        SoraMutexLock mutex;
        SoraCondition cond;
        std::string name;
        
        typedef std::vector<SoraThread*> THREAD_LIST;
        THREAD_LIST threads;
        
        std::deque<SoraThreadTask> queue;
        bool running;
    };
    
    
} // namespace sora


#endif
