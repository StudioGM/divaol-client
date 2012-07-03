//
//  SoraThreadTask.cpp
//  Sora
//
//  Created by Robert Bu on 7/6/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraThreadTask.h"
#include "SoraLogger.h"

namespace sora {
    
    SoraThreadTask::SoraThreadTask():
    iarg(NULL) {
        
    }
    
    SoraThreadTask::SoraThreadTask(void (*ThreadFunc)(void* arg)):
    iarg(NULL) {
        setAsCFunc(ThreadFunc);
    }
    
    void SoraThreadTask::setAsCFunc(void (*ThreadFunc)(void* arg)) {
        SoraCThreadTask* cTask = new SoraCThreadTask(ThreadFunc);
        if(!cTask)
            THROW_SORA_EXCEPTION(RuntimeException, "Error creating member thread task");
        else
            func = cTask;
    }
    
    void SoraThreadTask::setArg(void* _arg) {
        iarg = _arg;
    }
    
    void* SoraThreadTask::getArg() const {
        return iarg;
    }
    
    bool SoraThreadTask::isValid() const {
        return func.valid();
    }
    
    void SoraThreadTask::run() const {
        if(func.valid()) {
            SoraThreadTaskImpl* realFunc = func.get();
            realFunc->operator ()(iarg);
        }
    }
    
    void SoraThreadTask::operator()() const {
        run();
    }
    
} // namespace sora