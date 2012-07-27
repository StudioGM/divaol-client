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
#include "divacore/MusicManager/DivaBassMusicManager.h"

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

		HSAMPLE timerSample;
		HCHANNEL timerChannel;

	private:
		inline void _reset() {timeStamp = SoraTimestamp();}
		inline void _update() {time += timeStamp.elapsed();}
	public:
		TimeCounterImpl():time(0),isPaused(true) {}

		inline void start() 
		{
			timerSample = ::BASS_SampleCreate(203752000ll, 28160, 1, 1, BASS_SAMPLE_LOOP|BASS_SAMPLE_OVER_POS);
			timerChannel = BASS_SampleGetChannel(timerSample,false);	
			::BASS_ChannelPlay(timerChannel,false);
		}
		inline void reset() {pause(); ::BASS_ChannelSetPosition(timerChannel,0,BASS_POS_BYTE);}
		inline double getTime() 
		{
			return ::BASS_ChannelBytes2Seconds(timerChannel,BASS_ChannelGetPosition(timerChannel,BASS_POS_BYTE));
		}
		inline void pause() 
		{
			if(!isPaused)
			{
				::BASS_ChannelPause(timerChannel);
			}
		}
		inline void resume()
		{
			if(isPaused)
			{
				::BASS_ChannelPlay(timerChannel,false);
			}
		}
		inline void set(double time)
		{
			BASS_ChannelSetPosition(timerChannel,BASS_ChannelSeconds2Bytes(timerChannel,time),BASS_POS_BYTE);
		}
	};
}

#endif