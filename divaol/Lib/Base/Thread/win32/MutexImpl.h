/****************************************************
 *  MutexImpl.h
 *
 *  Created by tempbuffer on 8/8/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  MutexImpl for win32
 ****************************************************/

#ifndef _BASE_MUTEX_IMPL
#define _BASE_MUTEX_IMPL

#include "../../Common.h"
#include "../../Uncopyable.h"

#ifdef BASE_OS_WINDOWS

namespace Base
{
	class MutexImpl : public Uncopyable
	{
	protected:
		MutexImpl() {
			mutex = CreateMutexW(NULL,FALSE,NULL);
		}
		~MutexImpl() {
			CloseHandle(mutex);
		}
		inline void lockImpl() {
			WaitForSingleObject(mutex,INFINITE);
		}
		inline void unlockImpl() {
			ReleaseMutex(mutex);
		}

	private:
		HANDLE mutex;
	};
}

#endif
#endif