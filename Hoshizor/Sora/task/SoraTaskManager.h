//
//  SoraTaskManager.h
//  Sora
//
//  Created by Ruiwei Bu on 9/29/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraTaskManager_h
#define Sora_SoraTaskManager_h

#include "SoraPlatform.h"
#include "SoraUncopyable.h"
#include "SoraAutoPtr.h"
#include "SoraSignal.h"

#include "SoraAutoUpdate.h"

#ifdef SORA_ENABLE_MULTI_THREAD
#include "thread/SoraThreadPool.h"
#endif

#include <list>

namespace sora {
    
    class SoraAbstractTask;
    class SoraEventHandler;
    class SoraTaskNotification;
    
    class SoraTaskManager: public SoraUncopyable, public SoraAutoUpdate {
        friend class SoraAbstractTask;
        
    public:
        typedef SoraAutoPtr<SoraAbstractTask>           TaskPtr;
        typedef std::list<TaskPtr>                      TaskList;
        typedef std::list<SoraAbstractTask*>            PendingTaskList;
        typedef SoraSignal<void(SoraTaskNotification*)> NotificationSignal;
       
    public:
        SoraTaskManager(bool multithreaded=false);
        ~SoraTaskManager();
        
        // start a task, if the task finished, it would be removed from the task list
        void start(SoraAbstractTask* task, bool periodical=false);
        
        // request to cancel all tasks
        void cancelAll();
        
#ifdef SORA_ENABLE_MULTI_THREAD
        void joinAll();
#endif
        
    public:
        // add a task to the task list
        // all task would run during the next update cycle unless manually started
        void addTask(SoraAbstractTask* task, bool periodical=false);
        void removeTask(SoraAbstractTask* task);
        
        // run a added task manually
        void runTask(const std::string& name);
        
        void onUpdate(float dt);
        
    private:
        void taskFinished(SoraAbstractTask* task);
        void taskStarted(SoraAbstractTask* task);
        void taskCanceld(SoraAbstractTask* task);
        void taskProgress(SoraAbstractTask* task);
        
    public:
        TaskPtr taskByName(const std::string& name) const;
        
        int count() const;
        
        bool isMultiThreaded() const;
        
        const TaskList& allTasks() const;
        
        // notifications would be posted under SoraTaskNotification subclass
        template<typename T>
        SoraConnection subscribeToNotification(const T& func);
        
        static SoraTaskManager& defaultManager(bool multiThreaded=false);
        
    public:
        /**
         * Static Functions
         **/
        
        // start managed in defaultManager
        static void StartTask(SoraAbstractTask* task);
#ifdef SORA_ENABLE_MULTI_THREAD
        static void StartAsyncTask(SoraAbstractTask* task);
#endif
        
    private:
        bool mMultiThread;
        
        TaskList mTasks;
        PendingTaskList mPendingTask;
        
        NotificationSignal mNoficationSig;
        
#ifdef SORA_ENABLE_MULTI_THREAD
        SoraThreadPool mThreadPool;
        SoraMutexLock mMutex;
        
        void taskRun(void* task);
#endif
    };
    
    template<typename T>
    inline SoraConnection SoraTaskManager::subscribeToNotification(const T& func) {
        return mNoficationSig.connect(func);
    }

} // namespace sora


#endif
