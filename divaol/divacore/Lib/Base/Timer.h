/****************************************************
 *  Timestamp.h
 *
 *  Created by tempbuffer on 8/1/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  Time Counter
 ****************************************************/

#ifndef _BASE_TIMER_
#define _BASE_TIMER_

#include "Common.h"
#include "Singleton.h"

#include <ctime>

namespace Base
{
	/*************************
	 * Timestamp
	 *************************/

	typedef int64 TimeVal;
	typedef int64 TimeDiff;

	class TimeUtil
	{
	public:
		enum {FORMAT_DATE,FORMAT_TIME};
		//global
		static TimeVal currentTime();
		static TimeVal resolution() {return 1000000;}

		static void getDate(uint16 &year, uint16 &month, uint16 &day);
		static void getTime(uint16 &hour, uint16 &minute, uint16 &second);
		static String getFormatTime(bool time = true, bool date = true);

		static void mSleep(uint32 millisecond);
	};

	class Timestamp
	{
	public:
		Timestamp();
		Timestamp(TimeVal time);
		Timestamp(const Timestamp &other);
		~Timestamp() {}

		Timestamp& operator=(const Timestamp &other);
		Timestamp& operator=(TimeVal time);

		void update();

		TimeDiff elapsed() const;

		TimeDiff elaspedInMillisecond();

		double elapsedInSecond();

		bool isElapsed(TimeDiff interval) const;

		//oepration
		bool operator == (const Timestamp& ts) const {return mTime==ts.mTime;}
		bool operator != (const Timestamp& ts) const {return mTime!=ts.mTime;}
		bool operator >  (const Timestamp& ts) const {return mTime>ts.mTime;}
		bool operator >= (const Timestamp& ts) const {return mTime>=ts.mTime;}
		bool operator <  (const Timestamp& ts) const {return mTime<ts.mTime;}
		bool operator <= (const Timestamp& ts) const {return mTime<=ts.mTime;}

		Timestamp  operator +  (TimeDiff d) const {return Timestamp(mTime+d);}
		Timestamp  operator -  (TimeDiff d) const {return Timestamp(mTime-d);}
		TimeDiff   operator -  (const Timestamp& ts) const {return mTime-ts.mTime;}
		Timestamp& operator += (TimeDiff d) {return (*this=*this+d);}
		Timestamp& operator -= (TimeDiff d) {return (*this=*this-d);}

	private:
		TimeVal mTime;
	};

	typedef Singleton<Timestamp> GlobalTimeStamp;

	/*************************
	 * Timer
	 *************************/

	class Timer
	{
		TimeVal mTime;
		Timestamp mTimeStamp;
		bool mIsPaused;

	private:
		inline void _refresh();
		inline void _update();

	public:
		Timer():mTime(0),mIsPaused(true) {}

		inline void start();
		inline void reset();
		inline double getTimeInSecond();
		inline TimeVal getTimeInMillisecond();
		inline TimeVal elapsed();
		inline void pause();
		inline void resume();
		inline void set(double time);
	};

	void Timer::_refresh() {
		mTimeStamp = Timestamp();
	}
	void Timer::_update() {
		mTime += mTimeStamp.elapsed();
	}

	void Timer::start() {
		_refresh();
		mTime = 0; 
		mIsPaused = false;
	}
	void Timer::reset() {
		pause(); 
		mTime = 0;
	}
	double Timer::getTimeInSecond() 
	{
		return elapsed()/double(TimeUtil::resolution());
	}
	TimeVal Timer::getTimeInMillisecond()
	{
		return elapsed()*1000/TimeUtil::resolution();
	}
	TimeVal Timer::elapsed()
	{
		if(mIsPaused)
			return mTime;
		else
			return mTime+mTimeStamp.elapsed();
	}
	void Timer::pause() 
	{
		if(!mIsPaused)
		{
			_update();
			mIsPaused = true;
		}
	}
	void Timer::resume()
	{
		if(mIsPaused)
		{
			_refresh();
			mIsPaused = false;
		}
	}
	void Timer::set(double time)
	{
		pause();
		mTime = TimeVal(time*TimeUtil::resolution());
		_refresh();
	}
}

#endif