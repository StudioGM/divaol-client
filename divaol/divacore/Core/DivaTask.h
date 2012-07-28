/*
 *  DivaTask.h
 *
 *  Created by Hyf042 on 2/7/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_TASK
#define DIVA_TASK

#include "DivaCommon.h"
#include "DivaMapInfo.h"
#include "SoraTask.h"

namespace divacore
{
	using namespace sora;
	/*
	 * Task
	 */
	class Task
	{
		volatile bool running;
		bool bMultiThread;
		sora::SoraThread thread;
		sora::SoraThreadTask task;
	public:
		Task(bool bMultiThread = false):bMultiThread(bMultiThread),running(false) {}
		Task(void (*ThreadFunc)(void* arg),bool bMultiThread = false):bMultiThread(bMultiThread),running(false) {setAsCFunc(ThreadFunc);}
		Task(const Task &task)
		{
			running = task.running;
			bMultiThread = task.bMultiThread;
			this->task = task.task;
		}
		Task& operator=(const Task &task)
		{
			running = task.running;
			bMultiThread = task.bMultiThread;
			this->task = task.task;
			return *this;
		}
		template<typename T>
		Task(void (T::*ThreadFunc)(void* args), T* obj,bool bMultiThread = false):bMultiThread(bMultiThread),running(false) {
			setAsMemberFunc(ThreadFunc, obj);
		}
		~Task() {stop();}

		template<typename T>
		void setAsMemberFunc(void (T::*ThreadFunc)(void* args), T* obj){task.setAsMemberFunc(ThreadFunc,obj);}
		void setAsCFunc(void (*ThreadFunc)(void* arg)) {task.setAsCFunc(ThreadFunc);}

		void run(void* args = NULL) {start(args);}
		void start(void* args = NULL) 
		{
			if(running)
				return;

			if(bMultiThread)
			{
				running = true;
				sora::SoraThreadTask _task(&Task::runInThread,this);
				task.setArg(args);
				thread.setThreadTask(_task);
				thread.start();
			}
			else
			{
				task.setArg(args);
				task.run();
			}
		}
		void stop()
		{
			if(running)
			{
				thread.exit();
				running = false;
			}
		}
		void waitForFinish()
		{
			if(running)
			{
				thread.join();
				running = false;
			}
		}
		bool isRunning() {return running;}
		bool isMultiThread() {return bMultiThread;}
		void setMultiThread(bool isMulti) {bMultiThread=isMulti;}
		void runInThread(void* args)
		{
			task.run();
			running = false;
		}
	};

	typedef Task* TaskPtr;
}

#endif