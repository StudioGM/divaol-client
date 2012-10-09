/****************************************************
 *  Singleton.h
 *
 *  Created by tempbuffer on 7/31/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  A base class which is singleton
 ****************************************************/

#ifndef _BASE_SINGLETON_
#define _BASE_SINGLETON_

namespace Base
{
	template<typename T>
	class SingletonAutoReleaser 
	{
		public:
			void declare() {}
			~SingletonAutoReleaser() {
				T::destroy();
			}
		private:
	};

	template<typename T>
	class Singleton
	{
	public:
		template<typename R>
		static T& initialize(R arg) {
			if(!mInstance) {
				mInstance = new T(arg);
				mReleaser.declare();
			}
			return *mInstance;
		}

		static T& instance() {
			return *instancePtr();
		}

		static T* instancePtr(){
			if(!mInstance) {
				mInstance = new T;
				mReleaser.declare();
			}
			return mInstance;
		}

		static void destroy() {
			if(mInstance) {
				delete mInstance;
			}
			mInstance = 0;
		}
		static SingletonAutoReleaser<Singleton<T>> mReleaser;
	private:
		static T* mInstance;
		

	protected:
		Singleton() {}
		virtual ~Singleton() {}
	};

	template<typename T>
	T* Singleton<T>::mInstance = 0;
	template<typename T>
	SingletonAutoReleaser<Singleton<T>> Singleton<T>::mReleaser;
}

#endif