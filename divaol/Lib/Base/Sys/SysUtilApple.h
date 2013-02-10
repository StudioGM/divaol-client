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

#include <pthread.h>
#include <dirent.h>

namespace Base
{
	/*********************************************
	 * MutexImpol
	 *********************************************/
	class MutexImpl : public Uncopyable
	{
	protected:
		MutexImpl() {
			pthread_mutex_init(&mutex, NULL);
		}
		~MutexImpl() {
			pthread_mutex_destroy(&mutex);
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

	/*********************************************
	 * ThreadImpl
	 *********************************************/
	class ThreadImpl: Uncopyable {
	protected:
		ThreadImpl():
			 mActive(false),mThread(0) {
			 }
			 ~ThreadImpl() {
				 clear();
			 }
			 void clear() {
				 if(mThread==0)
					 return;
				 if(mActive)
					 exit();
				 mActive = false;
				 mThread = 0;
			 }
			 bool start() {
				 if(mActive)
					 return false;

				 int32 result = pthread_create(&mThread, NULL, ThreadImpl::entry, this);
				 return (result>0);
			 }
			 inline bool active() const {return mActive;}

			 inline void wait() {
				 if(mActive) {
					 pthread_join(mThread, 0);
					 setActive(false);
				 }
			 }

			 inline void exit() {
				 if(active()) {
					 pthread_exit(&mThread);
					 setActive(false);
				 }
			 }

			 virtual void callback() = 0;

	private:
		void setActive(bool active) {
			mActive = active;
		}

		static void* entry(void* pthis) {
			ThreadImpl *impl = static_cast<ThreadImpl*>(pthis);
			if(impl) {
				impl->callback();

				impl->setActive(false);
			}
			return 0;
		}
	private:
		pthread_t mThread;

		bool mActive;
	};
    
    class DirectoryIteratorImpl {
    public:
        DirectoryIteratorImpl(const String& path) {
            mDir = opendir(path.ansi_str());
            
            if(mDir)
                next();
        }
        
        ~DirectoryIteratorImpl() {
            if(mDir)
                closedir(mDir);
        }
        
        void duplicate();
        void release();
        
        const String& get() const;
        const String& next() {
            if(!mDir)
                return mCurrent;
            
            do {
                struct dirent* entry = readdir(mDir);
                if(entry)
                    mCurrent = String(entry->d_name);
                else
                    mCurrent.clear();
            }
            while(mCurrent == L"." || mCurrent == L".." || (mCurrent.size() > 0 && mCurrent[0] == '.'));
            
            return mCurrent;
        }
        
        bool isFolder() const { return false; }
        
    private:
        DIR* mDir;
        String mCurrent;
        int mRC;
    };
    
    const String& DirectoryIteratorImpl::get() const {
        return mCurrent;
    }
    
    inline void DirectoryIteratorImpl::duplicate() {
        ++mRC;
    }
    
    inline void DirectoryIteratorImpl::release() {
        if (--mRC) {
            delete this;
        }
    }
}

#endif