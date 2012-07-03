//
//  SoraSoundSystemThread.h
//  Sora
//
//  Created by Ruiwei Bu on 9/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraSoundSystemThread_h
#define Sora_SoraSoundSystemThread_h

#include "thread/SoraThread.h"

namespace sora {
    
    class SoraSoundSystem;
    
    class SORA_API SoraSoundSystemThread {
        SoraSoundSystemThread():mCond(mMutex), mExit(false) { }
        ~SoraSoundSystemThread();
        
    public:
        
        static SoraSoundSystemThread* Instance();
        static void Destroy();
        
        void setSoundSystem(SoraSoundSystem* ss);
        
        void start();
        void onUpdate(float dt);
        
    private:
        void threadTask(void* args);
        
        SoraThread mSSThread;
        SoraThreadTask mThreadTask;
        SoraCondition mCond;
        SoraMutexLock mMutex;
        bool mExit;
        
        SoraSoundSystem* mSoundSystem;
    };
    
} // namespace sora

#endif
