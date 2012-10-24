/****************************************************
 *  Observer.h
 *
 *  Created by tempbuffer on 9/27/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  A base class which is singleton
 ****************************************************/

#ifndef _BASE_OBSERVER_
#define _BASE_OBSERVER_

#include "../Common.h"
#include "../Function/Function.h"

namespace Base
{
	class Notification {
	public:
		Notification(std::string description = "", dword msg = 0, void *source = 0, void *extra = 0, dword arg = 0):
			mMsg(msg),mDescription(description),mSource(source),mExtra(extra),mArg(arg) {}
		virtual ~Notification () {}

		uint32 msg() const {return mMsg;}
		dword arg() const {return mArg;}
		std::string description() const {return mDescription;}
		void* source() const {return mSource;}
		void* extra() const {return mExtra;}
	public:
		uint32 mMsg;
		dword mArg;
		std::string mDescription;
		void *mSource;
		void *mExtra;
	};
	typedef Function<void(Notification)> Observer;
	class ObserverHandler
	{
		typedef std::vector<Observer> Observers;
	public:
		void attachObserver(Observer ob) {
			for(Observers::iterator ptr = mObservers.begin(); ptr != mObservers.end(); ptr++)
				if(*ptr==ob)
					return;
			mObservers.push_back(ob);
		}

		void detachObserver(Observer ob) {
			for(Observers::iterator ptr = mObservers.begin(); ptr != mObservers.end(); ptr++)
				if(*ptr==ob) {
					mObservers.erase(ptr);
					break;
				}
		}

		void clearObserver() {
			mObservers.clear();
		}

		void notify(std::string description, uint32 msg, void *extra = 0, dword arg = 0) {
			Notification nf(description,msg,this,extra,arg);
			for(Observers::iterator ptr = mObservers.begin(); ptr != mObservers.end(); ptr++)
				ptr->invoke(nf);
		}
	protected:
		Observers mObservers;
	};
}

#endif