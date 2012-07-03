#include "SoraTimestamp.h"
#include <algorithm>
#if !defined(_WIN32)
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>
#endif


namespace sora {


SoraTimestamp::SoraTimestamp()
{
	update();
}


SoraTimestamp::SoraTimestamp(TimeVal tv)
{
	_ts = tv;
}


SoraTimestamp::SoraTimestamp(const SoraTimestamp& other)
{
	_ts = other._ts;
}


SoraTimestamp::~SoraTimestamp()
{
}


SoraTimestamp& SoraTimestamp::operator = (const SoraTimestamp& other)
{
	_ts = other._ts;
	return *this;
}


SoraTimestamp& SoraTimestamp::operator = (TimeVal tv)
{
	_ts = tv;
	return *this;
}


void SoraTimestamp::swap(SoraTimestamp& timestamp)
{
	std::swap(_ts, timestamp._ts);
}


SoraTimestamp SoraTimestamp::FromEpochTime(std::time_t t)
{
	return SoraTimestamp(TimeVal(t)*Resolution());
}


SoraTimestamp SoraTimestamp::FromUtcTime(UtcTimeVal val)
{
	val -= (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
	val /= 10;
	return SoraTimestamp(val);
}
    
    SoraTimestamp::TimeVal SoraTimestamp::CurrentTime() {
        TimeVal ts;
#if defined(_WIN32)
        
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


void SoraTimestamp::update()
{
#if defined(_WIN32)

	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
	epoch.LowPart  = 0xD53E8000;
	epoch.HighPart = 0x019DB1DE;

	ULARGE_INTEGER ts;
	ts.LowPart  = ft.dwLowDateTime;
	ts.HighPart = ft.dwHighDateTime;
	ts.QuadPart -= epoch.QuadPart;
	_ts = ts.QuadPart/10;

#else

	struct timeval tv;
	if (gettimeofday(&tv, NULL))
		throw ("cannot get time of day");
	_ts = TimeVal(tv.tv_sec)*Resolution() + tv.tv_usec;
	
#endif
}


#if defined(_WIN32)


SoraTimestamp SoraTimestamp::fromFileTimeNP(uint32 fileTimeLow, uint32 fileTimeHigh)
{
	ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
	epoch.LowPart  = 0xD53E8000;
	epoch.HighPart = 0x019DB1DE;
	
	ULARGE_INTEGER ts;
	ts.LowPart  = fileTimeLow;
	ts.HighPart = fileTimeHigh;
	ts.QuadPart -= epoch.QuadPart;

	return SoraTimestamp(ts.QuadPart/10);
}


void SoraTimestamp::toFileTimeNP(uint32& fileTimeLow, uint32& fileTimeHigh) const
{
	ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
	epoch.LowPart  = 0xD53E8000;
	epoch.HighPart = 0x019DB1DE;

	ULARGE_INTEGER ts;
	ts.QuadPart  = _ts*10;
	ts.QuadPart += epoch.QuadPart;
	fileTimeLow  = ts.LowPart;
	fileTimeHigh = ts.HighPart;
}


#endif



} // namespace Common
