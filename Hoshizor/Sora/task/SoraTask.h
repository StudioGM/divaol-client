//
//  SoraTask.h
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/29/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef Sora_SoraTask_h
#define Sora_SoraTask_h

#include "SoraPlatform.h"
#include "SoraFunction.h"

namespace sora {
    
    /**
     * Abstract task class
     **/
    
    class SoraTaskManager;
    
    class SoraAbstractTask {
        friend class SoraTaskManager;
        
    public:
        enum TaskState {
            TaskIdle,
            TaskRunning,
            TaskCanceling,
            TaskFinished,
            TaskPreparing,
        };
        
        SoraAbstractTask(const std::string& name);
        virtual ~SoraAbstractTask();
        
        const std::string& getName() const;
        
        void run();
        void cancel();
        void reset();
        
        virtual void taskRun() = 0;
        virtual void taskCancel() = 0;
        
        void reportProgress(float progress);
        float getProgress() const;
        
        void setOwner(SoraTaskManager* owner);
        SoraTaskManager* getOwner() const;
        
        TaskState state() const;
        
    private:
        std::string mName;
        
        float mProgress;
        TaskState mState;
        
        SoraTaskManager* mOwner;
        
    private:
        /* periodical task? say execute every update cycle */
        bool mPeriodical;
    };
    
    class SoraTask: public SoraAbstractTask {
    public:
        typedef SoraFunction<void(SoraTask*)> FuncType;
        
        SoraTask(const std::string& name, const FuncType& func);
        
        void taskRun();
        void taskCancel();
        
    private:
        FuncType mFunc;
    };
    
    
#define WaitForTaskFinish(task) while(task->state() == sora::SoraAbstractTask::TaskPreparing || \
                                      task->state() == sora::SoraAbstractTask::TaskRunning);
    
    
} // namespace sora

#endif
