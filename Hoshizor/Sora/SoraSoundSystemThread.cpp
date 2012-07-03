//
//  SoraSoundSystemThread.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraSoundSystemThread.h"
#include "SoraSoundSystem.h"

namespace sora {
    
    static SoraSoundSystemThread* g_instance = 0;
    
    SoraSoundSystemThread::~SoraSoundSystemThread() {
        mExit = true;
        mCond.notify();
        
        if(mSoundSystem) {
            mSoundSystem->shutdown();
        }
        mSSThread.exit();
    }
    
    SoraSoundSystemThread* SoraSoundSystemThread::Instance() {
        if(!g_instance)
            g_instance = new SoraSoundSystemThread;
        return g_instance;
    }
    
    void SoraSoundSystemThread::Destroy() {
        if(g_instance)
            delete g_instance;
    }
    
    void SoraSoundSystemThread::setSoundSystem(SoraSoundSystem* ss) {
        mSoundSystem = ss;
    }
    
    void SoraSoundSystemThread::start() {
        assert(mSoundSystem != 0);
        
        mThreadTask.setAsMemberFunc(&SoraSoundSystemThread::threadTask, this);
        mThreadTask.setArg(this);
        
        mSSThread.startWithTask(mThreadTask);
    }
    
    void SoraSoundSystemThread::threadTask(void* args) {
        while(true) {
            mCond.wait();
            if(mExit)
                break;
            
            if(mSoundSystem)
                mSoundSystem->update();   
        }
    }
    
    void SoraSoundSystemThread::onUpdate(float dt) {
        mCond.notify();
    }
} // namespace sora
