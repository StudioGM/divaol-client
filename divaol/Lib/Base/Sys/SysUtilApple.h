/****************************************************
 *  SysUtilApple.h
 *
 *  Created by tempbuffer on 9/16/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  A bunch of things that are related to apple 
 ****************************************************/

#if !defined(_BASE_SYS_UTIL_) && defined(BASE_OS_FAMILY_APPLE)
#define _BASE_SYS_UTIL_

#include "../Common.h"

namespace Base
{
	// posix mutex
	class MutexImpl : public Uncopyable
	{
	protected:
		MutexImpl() {
			phread_mutex_init(&mutex, NULL);
		}
		~MutexImpl() {
			phread_mutex_destory(&mutex);
		}
		inline void lockImpl() {
			if(pthread_mutex_lock(&mutex))
				BASE_THROW_EXCEPTION("Cannot lock mutex");
		}
		inline void unlockImpl() {
			if(pthread_mutex_unlock(&mutex))
				BASE_THROW_EXCEPTION("Cannot unlock mutex");
		}

	private:
		pthread_mutex_t mutex;
	};
}

#endif