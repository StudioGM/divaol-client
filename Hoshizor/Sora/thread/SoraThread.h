/*
 *  SoraThreadFactory.h
 *  Hoshizora
 *
 *  Created by Robert Bu on 8/27/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SORA_THREAD_H
#define SORA_THREAD_H

#include "SoraPlatform.h"

#if !defined(OS_WIN32) || defined(SORA_WIN32_PTHREAD)
#include "SoraThreadImplPosix.h"
	#if defined(SORA_WIN32_PTHREAD)
		#pragma comment(lib, "pthreadVC2.lib");
	#endif
#else
#include "SoraThreadImplWin32.h"
#endif

#include "SoraThreadTask.h"
#include "SoraMutex.h"
#include "SoraCondition.h"
#include "SoraThreadLocal.h"
#include "SoraBlockingQueue.h"
#include "SoraSemaphore.h"

namespace sora {
		
	class SoraThread: public SoraThreadImpl {
		friend class SoraThreadPool;

	public:
		SoraThread();
        SoraThread(const SoraThreadTask& task, const std::string name=std::string());
        ~SoraThread();
		
        int32 start();
        int32 startWithTask(const SoraThreadTask& task);
        
        bool isActive() const;
        int32 getActiveThreadNum() const;
        
        void setThreadTask(const SoraThreadTask& task);
        SoraThreadTask getThreadTask() const;
        
        void join();
        void exit();
		
        void setName(const std::string& name);
        std::string getName() const;
		
	private:
        std::string name;

		static int32 active_thread_num;
    };
		
} // namespace sora


#endif