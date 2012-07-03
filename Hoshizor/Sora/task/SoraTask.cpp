//
//  SoraAbstractTask.cpp
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/29/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "SoraTask.h"
#include "SoraTaskManager.h"

#include "SoraLogger.h"

namespace sora {
    
    SoraAbstractTask::SoraAbstractTask(const std::string& name):
    mName(name),
    mProgress(0.0f),
    mState(TaskIdle),
    mOwner(0),
    mPeriodical(false) {
    }
    
    SoraAbstractTask::~SoraAbstractTask() {
        
    }
    
    const std::string& SoraAbstractTask::getName() const {
        return mName;
    }
    
    void SoraAbstractTask::run() {
        mState = TaskRunning;
        mProgress = 0.0;
        
        if(mOwner)
            mOwner->taskStarted(this);
        
        try {
            taskRun();
        } catch(std::exception& exp) {
            log_error(vamssg("SoraAbstractTask::run: %s", exp.what()));
        }
        
        mState = TaskFinished;
        mProgress = 1.0f;
        
        if(mOwner)
            mOwner->taskFinished(this);
    }
    
    SoraAbstractTask::TaskState SoraAbstractTask::state() const {
        return mState;
    }
    
    void SoraAbstractTask::cancel() {
        if(mState == TaskRunning) {
            mOwner->taskCanceld(this);
        }
        
        taskCancel();
        mState = TaskCanceling;
    }
    
    void SoraAbstractTask::reset() {
        if(mState == TaskRunning)
            mOwner->taskCanceld(this);
        
        mState = TaskIdle;
        mProgress = 0.0f;
    }
    
    void SoraAbstractTask::reportProgress(float progress) {
        mProgress = progress;
        
        if(mOwner)
            mOwner->taskProgress(this);
    }
    
    float SoraAbstractTask::getProgress() const {
        return mProgress;
    }
    
    void SoraAbstractTask::setOwner(SoraTaskManager* manager) {
        mOwner = manager;
    }
    
    SoraTaskManager* SoraAbstractTask::getOwner() const {
        return mOwner;
    }
    
    SoraTask::SoraTask(const std::string& name, const FuncType& func):
    SoraAbstractTask(name),
    mFunc(func) {
        
    }
    
    void SoraTask::taskRun() {
        mFunc(this);
    }
    
    void SoraTask::taskCancel() {
        // 
    }

    
} // namespace sora
