/****************************************************
 *  MutexImpl.h
 *
 *  Created by tempbuffer on 8/8/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  MutexImpl for posix
 ****************************************************/

#ifndef _BASE_MUTEX_IMPL
#define _BASE_MUTEX_IMPL

#include "../../Common.h"
#include "../../Uncopyable.h"

#ifndef BASE_OS_WINDOWS

#include <pthread.h>

namespace sora {

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


} // namespace sora

#endif
#endif