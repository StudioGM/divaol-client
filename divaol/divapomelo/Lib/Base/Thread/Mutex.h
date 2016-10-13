/****************************************************
 *  Mutex.h
 *
 *  Created by tempbuffer on 8/8/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  Mutex
 ****************************************************/

#ifndef _BASE_MUTEX_H_
#define _BASE_MUTEX_H_

#include "../Common.h"

namespace Base
{
	class Mutex : public MutexImpl
	{
	public:
		Mutex() {}
		~Mutex() {}

		void lock() {
			lockImpl();
		}
		void unlock() {
			unlockImpl();
		}
	};

	class MutexGuard : public Uncopyable
	{
	public:
		explicit MutexGuard(Mutex &mutex):_mutex(mutex) {
			_mutex.lock();
		}
		~MutexGuard() {
			_mutex.unlock();
		}

	private:
		Mutex &_mutex;
	};

	//lock until the end of the code segment
	#define BASE_MUTEX_GUARD(mutex) Base::MutexGuard BASE_UNIQUE_NAME(__mutex_guard__)(mutex)

	//lock until the end of the file
	#define BASE_GLOBAL_MUTEX_GUARD()\
		static Base::Mutex BASE_UNIQUE_NAME(__global_mutex__);\
		MutexGuard BASE_UNIQUE_NAME(__mutex_guard__);

	//prevent MutexGuard(mutex), because it will automatically release at once
	#define MutexGuard(mutex) error "Missing MutexGuard object name"
}

#endif