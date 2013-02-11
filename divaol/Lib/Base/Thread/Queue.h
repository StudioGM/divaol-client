/****************************************************
 *  Queue.h
 *
 *  Created by tempbuffer on 8/8/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  Queue with multithread safey
 ****************************************************/

#ifndef _BASE_THREAD_SAFE_QUEUE_H_
#define _BASE_THREAD_SAFE_QUEUE_H_

#include "Mutex.h"

#include <queue>

namespace Base
{
	namespace ThreadSafe
	{
		template<typename T>
		class Queue : public Uncopyable
		{
		public:
			Queue(): mMutex(), mQueue(){
			}

			void push(const T& x) {
				BASE_MUTEX_GUARD(mMutex);
				mQueue.push(x);
			}

			void put(const T& x) {
				push(x);
			}

			void pop() {
				BASE_MUTEX_GUARD(mMutex);
				base_assert(!mQueue.empty());
				queue.pop();
			}

			T front() {
				BASE_MUTEX_GUARD(mMutex);
				base_assert(!mQueue.empty());
				return mQueue.front();
			}

			T take() {	
				BASE_MUTEX_GUARD(mMutex);
				base_assert(!mQueue.empty());
				T front = mQueue.front();
				mQueue.pop();
				return front;
			}

			bool take(T &ret)
			{
				BASE_MUTEX_GUARD(mMutex);
				if(mQueue.empty())
					return false;
				ret = mQueue.front();
				mQueue.pop();
				return true;
			}

			bool task_on()
			{
				mMutex.lock();
				if(mQueue.empty()) {
					mMutex.unlock();
					return false;
				}
				return true;
			}

			T task_take()
			{
				base_assert(!mQueue.empty());
				T front = mQueue.front();
				mQueue.pop();
				return front;
			}

			void task_done()
			{
				mMutex.unlock();
			}

			size_t size() {
				BASE_MUTEX_GUARD(mMutex);
				return mQueue.size();
			}

			bool empty() {
				BASE_MUTEX_GUARD(mMutex);
				return mQueue.empty();
			}

			void clear()
			{
				BASE_MUTEX_GUARD(mMutex);
				while(!mQueue.empty())
					mQueue.pop();
			}

		protected:
			Mutex mMutex;
			std::queue<T> mQueue;
		};
	}
}

#endif