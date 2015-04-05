/****************************************************
 *  TaskPool.h
 *
 *  Created by tempbuffer on 1/2/14.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  Thread Pool
 ****************************************************/

#ifndef _BASE_TASK_POOL_H_
#define _BASE_TASK_POOL_H_

#include "Thread.h"
#include "Queue.h"
#include "Mutex.h"
#include "../Timer.h"

namespace Base
{
	//!FIXME
	template<typename FuncSig>
	class TaskPool {
		typedef Function<FuncSig> func_type;

		static const int DefaultMaxThread = 4;
		vector<Thread> threads;
		ThreadSafe::Queue tasks;
		bool isRunning;
		bool isAccept;

		// to avoid assign void
		template<typename T>
		void invoke() {
			mRetVal = mFunc.invokeWithArg();
		}
		template<>
		void invoke<void>() {
			 mFunc.invokeWithArg();
		}

		virtual void callback() {
			invoke<result_type>();
		} 
	public:
		TaskPool(int threadNum = DefaultMaxThread) {
			isRunning = false;
			isAccept = false;
		}
		TaskPool() {
		}

		int taskCount() const { return tasks.size(); }

		void start() {
			isRunning = true;
			isAccept = true;
		}

		void join() {
			if (isRunning) {
				isAccept = false;

				while(TaskCount > 0)
					TimeUtil::mSleep(1);

				stop();
			}
		}

		void stop() {
			if (isRunning) {
				isRunning
			}
		}

		void set(func_type func) {
			if(!active())
				mFunc = func;
		}
		nomalized_ret_type getRetVal() const {return mRetVal;}

		bool active() const {return ThreadImpl::active();}
		
		inline void start() {
			ThreadImpl::start();
		}

		inline void stop() {
			ThreadImpl::exit();
		}

		inline void wait() {
			ThreadImpl::wait();
		}

		func_type& func() {
			return mFunc;
		}

		void operator()() {
			start();
		}
		operator bool() const{
			return active();
		}
		bool operator!() const{
			return !active();
		}

		//------------------------------------------
		// start templates & operator() templates
		//------------------------------------------
		template<typename T0>
		void start(T0 a0) {
			mFunc.setArg(a0);
			ThreadImpl::start();
		}
		template<typename T0, typename T1>
		void start(T0 a0, T1 a1) {
			mFunc.setArg(a0,a1);
			ThreadImpl::start();
		}
		template<typename T0, typename T1, typename T2>
		void start(T0 a0, T1 a1, T2 a2) {
			mFunc.setArg(a0,a1,a2);
			ThreadImpl::start();
		}
		template<typename T0, typename T1, typename T2, typename T3>
		void start(T0 a0, T1 a1, T2 a2, T3 a3) {
			mFunc.setArg(a0,a1,a2,a3);
			ThreadImpl::start();
		}
		template<typename T0, typename T1, typename T2, typename T3, typename T4>
		void start(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4) {
			mFunc.setArg(a0,a1,a2,a3,a4);
			ThreadImpl::start();
		}
		template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
		void start(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
			mFunc.setArg(a0,a1,a2,a3,a4,a5);
			ThreadImpl::start();
		}
		template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
		void start(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) {
			mFunc.setArg(a0,a1,a2,a3,a4,a5,a6);
			ThreadImpl::start();
		}
		template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
		void start(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) {
			mFunc.setArg(a0,a1,a2,a3,a4,a5,a6,a7);
			ThreadImpl::start();
		}
		template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
		void start(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) {
			mFunc.setArg(a0,a1,a2,a3,a4,a5,a6,a7,a8);
			ThreadImpl::start();
		}
		template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
		void start(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9) {
			mFunc.setArg(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9);
			ThreadImpl::start();
		}
		template<typename T0>
		void operator()(T0 a0) {
			mFunc.setArg(a0);
			ThreadImpl::start();
		}
		template<typename T0, typename T1>
		void operator()(T0 a0, T1 a1) {
			mFunc.setArg(a0,a1);
			ThreadImpl::start();
		}
		template<typename T0, typename T1, typename T2>
		void operator()(T0 a0, T1 a1, T2 a2) {
			mFunc.setArg(a0,a1,a2);
			ThreadImpl::start();
		}
		template<typename T0, typename T1, typename T2, typename T3>
		void operator()(T0 a0, T1 a1, T2 a2, T3 a3) {
			mFunc.setArg(a0,a1,a2,a3);
			ThreadImpl::start();
		}
		template<typename T0, typename T1, typename T2, typename T3, typename T4>
		void operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4) {
			mFunc.setArg(a0,a1,a2,a3,a4);
			ThreadImpl::start();
		}
		template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
		void operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
			mFunc.setArg(a0,a1,a2,a3,a4,a5);
			ThreadImpl::start();
		}
		template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
		void operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) {
			mFunc.setArg(a0,a1,a2,a3,a4,a5,a6);
			ThreadImpl::start();
		}
		template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
		void operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) {
			mFunc.setArg(a0,a1,a2,a3,a4,a5,a6,a7);
			ThreadImpl::start();
		}
		template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
		void operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) {
			mFunc.setArg(a0,a1,a2,a3,a4,a5,a6,a7,a8);
			ThreadImpl::start();
		}
		template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
		void operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9) {
			mFunc.setArg(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9);
			ThreadImpl::start();
		}
		//------------------------------------------
		// end templates & operator() templates
		//------------------------------------------
	private:
		func_type mFunc;
		nomalized_ret_type mRetVal;
	};

	typedef Thread<void()> thread;
}

#endif