/*
 *  DivaTimeCounterImplWin32.h
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
		FLOAT __stdcall DXUtil_Timer(   TIMER_COMMAND   command   ) ;
		
	public:
		TimeCounterImpl():time(0),isPaused(true) {}

		inline void start() {DXUtil_Timer(TIMER_START); DXUtil_Timer(TIMER_START); isPaused = false;}
		inline void reset() {DXUtil_Timer(TIMER_RESET); DXUtil_Timer(TIMER_STOP);}
		inline double getTime() 
		{
			return DXUtil_Timer(TIMER_GETAPPTIME);
		}
		inline void pause() 
		{
			if(!isPaused)
			{
				DXUtil_Timer(TIMER_STOP);
				isPaused = true;
			}
		}
		inline void resume()
		{
			if(isPaused)
			{
				DXUtil_Timer(TIMER_START);
				isPaused = false;
			}
		}
	};
}

#endif