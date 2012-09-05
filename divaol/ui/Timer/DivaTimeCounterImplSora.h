/*
 *  DivaTimeCounterImplSora.h
 *
 *  Created by Hyf042 on 1/17/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_TIME_COUNTER_IMPL
#define DIVA_TIME_COUNTER_IMPL

#include "SoraTimestamp.h"

namespace divacore
{
	using namespace sora;

	/*
	TimeCounterImplSora
	*/
	class TimeCounterImpl
	{
		enum{TIMER_START,TIMER_RESET,TIMER_STOP,TIMER_ADVANCE,TIMER_GETABSOLUTETIME,TIMER_GETAPPTIME,TIMER_GETELAPSEDTIME};
		typedef uint32 TIMER_COMMAND;

		int64 time;
		SoraTimestamp timeStamp;
		bool isPaused;

	private:
		inline void _reset() {timeStamp = SoraTimestamp();}
		inline void _update() {time += timeStamp.elapsed();}
	public:
		TimeCounterImpl():time(0),isPaused(true) {}

		inline void start() {time = 0; _reset(); isPaused = false;}
		inline void reset() {pause(); time = 0;}
		inline double getTime() 
		{
			if(isPaused)
				return time/double(SoraTimestamp::Resolution());
			else
				return (time+timeStamp.elapsed())/double(SoraTimestamp::Resolution());
		}
		inline void pause() 
		{
			if(!isPaused)
			{
				_update();
				isPaused = true;
			}
		}
		inline void resume()
		{
			if(isPaused)
			{
				_reset();
				isPaused = false;
			}
		}
	};
}

#endif