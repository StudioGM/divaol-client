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

			size_t size() {
				BASE_MUTEX_GUARD(mMutex);
				return mQueue.size();
			}

			bool empty() const {
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