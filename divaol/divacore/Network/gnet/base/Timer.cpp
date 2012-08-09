/****************************************************
 *  Timestamp.cpp
 *
 *  Created by tempbuffer on 8/1/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  Time Counter
 ****************************************************/

#include "Timer.h"

namespace Base
{
	/*************************
	 * TimeUtil
	 *************************/

	TimeVal TimeUtil::currentTime()
	{
		TimeVal ts;
#if defined(BASE_OS_WINDOWS)

		FILETIME ft;
		GetSystemTimeAsFileTime(&ft);
		ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
		epoch.LowPart  = 0xD53E8000;
		epoch.HighPart = 0x019DB1DE;

		ULARGE_INTEGER tts;
		tts.LowPart  = ft.dwLowDateTime;
		tts.HighPart = ft.dwHighDateTime;
		tts.QuadPart -= epoch.QuadPart;
		ts = tts.QuadPart/10;
#else
		struct timeval tv;
		if (gettimeofday(&tv, NULL))
			throw ("cannot get time of day");
		ts = TimeVal(tv.tv_sec)*Resolution() + tv.tv_usec;
#endif
		return ts;
	}

	void TimeUtil::getDate(uint16 &year, uint16 &month, uint16 &day)
	{
#ifdef BASE_OS_WINDOWS
		SYSTEMTIME st;
		GetSystemTime(&st);
		year = st.wYear;
		month = st.wMonth;
		day = st.wDay;
#else
		#error no time support
#endif
	}

	void TimeUtil::getTime(uint16 &hour, uint16 &minute, uint16 &second)
	{
#ifdef BASE_OS_WINDOWS
		SYSTEMTIME st;
		GetSystemTime(&st);
		hour = st.wHour;
		minute = st.wMinute;
		second = st.wSecond;
#else
		#error no time support
#endif
	}

	base_string TimeUtil::getFormatTime(bool time, bool date)
	{
		base_string ret;
		if(time)
		{
			uint16 hour,minute,second;
			getTime(hour,minute,second);
			ret += FormatString("%02d:%02d:%02d",hour,minute,second);
		}
		if(time&&date)
			ret += " ";
		if(date)
		{
			uint16 year,month,day;
			getDate(year,month,day);
			ret += FormatString("%d/%d/%d",year,month,day);
		}
		return ret;
	}

	void TimeUtil::mSleep(uint32 millisecond) 
	{
#ifndef BASE_OS_WINDOWS
		struct timespec timeout0;
		struct timespec timeout1;
		struct timespec* tmp;
		struct timespec* t0 = &timeout0;
		struct timespec* t1 = &timeout1;

		t0->tv_sec = msec / 1000;
		t0->tv_nsec = (msec % 1000) * (1000 * 1000);	
		while ((nanosleep(t0, t1) == (-1)) && (errno == EINTR)) {

			tmp = t0;
			t0 = t1;
			t1 = tmp;
		}
#else
		Sleep(millisecond);
#endif
	}


	/*************************
	 * Timestamp
	 *************************/

	Timestamp::Timestamp() {
		update();
	}
	Timestamp::Timestamp(TimeVal time) {
		mTime = time;
	}
	Timestamp::Timestamp(const Timestamp &other) {
		mTime = other.mTime;
	}

	Timestamp& Timestamp::operator=(const Timestamp &other) {
		mTime = other.mTime;
		return *this;
	}
	Timestamp& Timestamp::operator=(TimeVal time) {
		mTime = time;
		return *this;
	}

	//global

	void  Timestamp::update() {
		mTime = TimeUtil::currentTime();
	}

	TimeDiff  Timestamp::elapsed() const {
		Timestamp now;
		return now - *this;
	}

	TimeDiff  Timestamp::elaspedInMillisecond() {
		return elapsed()*1000/TimeUtil::resolution();
	}

	double  Timestamp::elapsedInSecond() {
		return double(elapsed())/TimeUtil::resolution();
	}

	bool  Timestamp::isElapsed(TimeDiff interval) const {
		return elapsed()>=interval;
	}
}