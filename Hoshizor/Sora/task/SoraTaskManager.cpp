//
//  SoraTaskManager.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/29/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraTaskManager.h"
#include "SoraTask.h"
#include "SoraTaskNotification.h"
#include "SoraSingletonHolder.h"

namespace sora {
        
    SoraTaskManager::SoraTaskManager(bool multithreaded) {
        mMultiThread = multithreaded;
        
        if(multithreaded) {
#ifndef SORA_ENABLE_MULTI_THREAD
            THROW_SORA_EXCEPTION(RuntimeException, "cannot use multithreaded task manager when SORA_ENABLE_MULTI_THREAD is not enabled");
#else
      
            mThreadPool.start(1);
#endif
        }
    }
    
    SoraTaskManager::~SoraTaskManager() {
#ifdef SORA_ENABLE_MULTI_THREAD
        if(mMultiThread) 
            joinAll();
#else
        cancelAll();
#endif
    }
    
    void SoraTaskManager::taskRun(void* task) {
        sora_assert(task);
        
        SoraAbstractTask* rtask = static_cast<SoraAbstractTask*>(task);
        rtask->run();
    }
    
    void SoraTaskManager::start(SoraAbstractTask* task, bool periodical) {
        sora_assert(task);
        if(task->state() == SoraTask::TaskRunning) 
            THROW_SORA_EXCEPTION(RuntimeException, "SoraTaskManager: attempt to start a running task");
        
        task->mPeriodical = periodical;
        
        // avoid add a task already in the task list
        bool isInTaskList = false;
        TaskList::const_iterator it = mTasks.begin();
        TaskList::const_iterator end = mTasks.end();
        for(; it != end; ++it) {
            if(it->get() == task) {
                isInTaskList = true;
            }
        }
        if(!isInTaskList) {
            mTasks.push_back(TaskPtr(task));
        }
    
        task->setOwner(this);
        
#ifndef SORA_ENABLE_MULTI_THREAD
        task->run();
#else
        MUTEX_LOCK(mMutex);
        
        task->mState = sora::SoraAbstractTask::TaskPreparing;
        mThreadPool.run(ThreadTask(&SoraTaskManager::taskRun, this, task));
#endif
    }
    
    void SoraTaskManager::cancelAll() {
#ifdef SORA_ENABLE_MULTI_THREAD
        joinAll();
#else
        TaskList::iterator it = mTasks.begin();
        TaskList::iterator end = mTasks.end();
        
        for(; it != end; ++it) 
            (*it)->cancel();
#endif
    }
    
#ifdef SORA_ENABLE_MULTI_THREAD
    void SoraTaskManager::joinAll() {
        SoraMutexGuard guard(mMutex);
        
        TaskList::iterator it = mTasks.begin();
        TaskList::iterator end = mTasks.end();
        
        for(; it != end; ++it) 
            (*it)->cancel();
        
        mThreadPool.stop();
    }
#endif
    
    void SoraTaskManager::addTask(SoraAbstractTask* task, bool periodical) {
        task->mPeriodical = periodical;
        mPendingTask.push_back(task);
    }
    
    void SoraTaskManager::removeTask(SoraAbstractTask* task) {
        TaskList::iterator it = mTasks.begin();
        TaskList::iterator end = mTasks.end();
        for(; it != end; ++it) {
            if(it->get() == task) {
                mTasks.erase(it);
                break;
            }
        }
    }
    
    void SoraTaskManager::runTask(const std::string& name) {
        SoraAbstractTask* task = taskByName(name);
        if(task) {
            start(task);
        } else {
            if(!mPendingTask.empty()) {
                // otherwise find it in the pending task list
                PendingTaskList::iterator it = mPendingTask.begin();
                PendingTaskList::iterator end = mPendingTask.end();
                for(; it != end; ++it) {
                    if((*it)->getName() == name) {
                        start(*it);
                        
                        // remove from pending task list
                        mPendingTask.erase(it);
                    }
                }
            }
        }
    }
    
    void SoraTaskManager::onUpdate(float dt) {
        if(!mPendingTask.empty()) {
            // otherwise find it in the pending task list
            PendingTaskList::const_iterator it = mPendingTask.begin();
            PendingTaskList::const_iterator end = mPendingTask.end();
            for(; it != end; ++it) {
                start(*it);
            }
            
            mPendingTask.clear();
        }
        
        if(!mTasks.empty()) {
            TaskList::const_iterator it = mTasks.begin();
            TaskList::const_iterator end = mTasks.end();
            
            for(; it != end; ++it) {
                SoraAbstractTask* task = it->get();
                if(task->state() == SoraTask::TaskFinished &&
                   task->mPeriodical) {
#ifndef SORA_ENABLE_MULTI_THREAD
                    task->run();
#else
                    MUTEX_LOCK(mMutex);
                    
                    task->mState = sora::SoraAbstractTask::TaskPreparing;
                    mThreadPool.run(ThreadTask(&SoraTaskManager::taskRun, this, task));
#endif
                }
            }
        }
    }
    
    void SoraTaskManager::taskFinished(SoraAbstractTask* task) {
        SoraTaskNotification notification(SoraTaskNotification::TaskFinished, task);
        
        mNoficationSig(&notification);
        
        // remove the task from the task list
        if(task->mPeriodical == false)
            removeTask(task);
    }
    
    void SoraTaskManager::taskStarted(SoraAbstractTask* task) {
        SoraTaskNotification notification(SoraTaskNotification::TaskStarted, task);
        
        mNoficationSig(&notification);
    }
    
    void SoraTaskManager::taskCanceld(SoraAbstractTask* task) {
        SoraTaskNotification notification(SoraTaskNotification::TaskCanceled, task);
        
        mNoficationSig(&notification);
    }
    
    void SoraTaskManager::taskProgress(SoraAbstractTask* task) {
        SoraTaskNotification notification(SoraTaskNotification::TaskProgress, task);
        
        mNoficationSig(&notification);
    }
    
    SoraTaskManager::TaskPtr SoraTaskManager::taskByName(const std::string& name) const {
        TaskList::const_iterator it = mTasks.begin();
        TaskList::const_iterator end = mTasks.end();
        
        for(; it != end; ++it)
            if((*it)->getName() == name)
                return (*it);

		return TaskPtr();
    }
    
    int SoraTaskManager::count() const {
        return mTasks.size();
    }
    
    const SoraTaskManager::TaskList& SoraTaskManager::allTasks() const {
        return mTasks;
    }

    SoraTaskManager& SoraTaskManager::defaultManager(bool multiThreaded) {
        if(!multiThreaded) {
			static SoraTaskManager mTaskManager(false);
            return mTaskManager;
		}
        else {

			static SoraTaskManager mMultiThreadedTaskManager(true);
            return mMultiThreadedTaskManager;
		}
    }
    
    void SoraTaskManager::StartTask(sora::SoraAbstractTask* task) {
        sora_assert(task);
        
        defaultManager(false).start(task);
    }
    
#ifdef SORA_ENABLE_MULTI_THREAD
    void SoraTaskManager::StartAsyncTask(sora::SoraAbstractTask* task) {
        sora_assert(task);
        
        defaultManager(true).start(task);
    }
#endif
    
} // namespace sora
