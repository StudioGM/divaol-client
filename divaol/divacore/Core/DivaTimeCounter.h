/*
 *  DivaTimeCounter.h
 *
 *  Created by Hyf042 on 1/17/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 */

#ifndef DIVA_TIME_COUNTER
#define DIVA_TIME_COUNTER

#include "Timer/DivaTimeCounterImplSora.h"
//#include "Timer/DivaTimeCounterImplBass.h"

namespace divacore
{
	/*
	TimeCounter
	*/
	class TimeCounter : public TimeCounterImpl
	{
	public:
		void start() {TimeCounterImpl::start();}
		void reset() {TimeCounterImpl::reset();}
		double getTime() {return TimeCounterImpl::getTime();}
		void pause() {TimeCounterImpl::pause();}
		void resume() {TimeCounterImpl::resume();}
		void set(double time) {TimeCounterImpl::set(time);}
	};

	typedef TimeCounter* TimeCounterPtr;
}

#endif