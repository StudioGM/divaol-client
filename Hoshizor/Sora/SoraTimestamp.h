#ifndef SORA_TIME_STAMP
#define SORA_TIME_STAMP

#include "SoraPlatform.h"
#include <ctime>

/** 
 * Original TimeStamp class from Poco C++ Library
 * Modifired for Sora by darkfall
 **/

namespace sora {

class SORA_API SoraTimestamp
	/// A Timestamp stores a monotonic time value
	/// with (theoretical) microseconds resolution.
	/// Timestamps can be compared with each other
	/// and simple arithmetics are supported.
	/// Timestamps are UTC (Coordinated Universal Time)
	/// based and thus independent of the timezone
	/// in effect on the system.
{
public:
	typedef int64 TimeVal;    /// monotonic UTC time value in microsecond resolution
	typedef int64 UtcTimeVal; /// monotonic UTC time value in 100 nanosecond resolution
	typedef int64 TimeDiff;   /// difference between two timestamps in microseconds

	SoraTimestamp();
		/// Creates a timestamp with the current time.
		
	SoraTimestamp(TimeVal tv);
		/// Creates a timestamp from the given time value.
		
	SoraTimestamp(const SoraTimestamp& other);
		/// Copy constructor.
		
	~SoraTimestamp();
		/// Destroys the timestamp
		
	SoraTimestamp& operator = (const SoraTimestamp& other);
	SoraTimestamp& operator = (TimeVal tv);
	
	void swap(SoraTimestamp& timestamp);
		/// Swaps the Timestamp with another one.
	
	void update();
		/// Updates the Timestamp with the current time.

	bool operator == (const SoraTimestamp& ts) const;
	bool operator != (const SoraTimestamp& ts) const;
	bool operator >  (const SoraTimestamp& ts) const;
	bool operator >= (const SoraTimestamp& ts) const;
	bool operator <  (const SoraTimestamp& ts) const;
	bool operator <= (const SoraTimestamp& ts) const;
	
	SoraTimestamp  operator +  (TimeDiff d) const;
	SoraTimestamp  operator -  (TimeDiff d) const;
	TimeDiff   operator -  (const SoraTimestamp& ts) const;
	SoraTimestamp& operator += (TimeDiff d);
	SoraTimestamp& operator -= (TimeDiff d);
	
	std::time_t epochTime() const;
		/// Returns the timestamp expressed in time_t.
		/// time_t base time is midnight, January 1, 1970.
		/// Resolution is one second.
		
	UtcTimeVal utcTime() const;
		/// Returns the timestamp expressed in UTC-based
		/// time. UTC base time is midnight, October 15, 1582.
		/// Resolution is 100 nanoseconds.
	
	TimeVal epochMicroseconds() const;
		/// Returns the timestamp expressed in microseconds
		/// since the Unix epoch, midnight, January 1, 1970.
    
    static TimeVal CurrentTime();
	
	TimeDiff elapsed() const;
		/// Returns the time elapsed since the time denoted by
		/// the timestamp. Equivalent to Timestamp() - *this.
	
	bool isElapsed(TimeDiff interval) const;
		/// Returns true iff the given interval has passed
		/// since the time denoted by the timestamp.
	
	static SoraTimestamp FromEpochTime(std::time_t t);
		/// Creates a timestamp from a std::time_t.
		
	static SoraTimestamp FromUtcTime(UtcTimeVal val);
		/// Creates a timestamp from a UTC time value.
		
	static TimeVal Resolution();
		/// Returns the resolution in units per second.
		/// Since the timestamp has microsecond resolution,
		/// the returned value is always 1000000.
    
#if defined(_WIN32)
	static SoraTimestamp fromFileTimeNP(uint32 fileTimeLow, uint32 fileTimeHigh);
	void toFileTimeNP(uint32& fileTimeLow, uint32& fileTimeHigh) const;
#endif

private:
	TimeVal _ts;
};


//
// inlines
//
inline bool SoraTimestamp::operator == (const SoraTimestamp& ts) const
{
	return this->_ts == ts._ts;
}


inline bool SoraTimestamp::operator != (const SoraTimestamp& ts) const
{
	return this->_ts != ts._ts;
}


inline bool SoraTimestamp::operator >  (const SoraTimestamp& ts) const
{
	return this->_ts > ts._ts;
}


inline bool SoraTimestamp::operator >= (const SoraTimestamp& ts) const
{
	return this->_ts >= ts._ts;
}


inline bool SoraTimestamp::operator <  (const SoraTimestamp& ts) const
{
	return this->_ts < ts._ts;
}


inline bool SoraTimestamp::operator <= (const SoraTimestamp& ts) const
{
	return this->_ts <= ts._ts;
}


inline SoraTimestamp SoraTimestamp::operator + (SoraTimestamp::TimeDiff d) const
{
	return SoraTimestamp(this->_ts + d);
}


inline SoraTimestamp SoraTimestamp::operator - (SoraTimestamp::TimeDiff d) const
{
	return SoraTimestamp(this->_ts - d);
}


inline SoraTimestamp::TimeDiff SoraTimestamp::operator - (const SoraTimestamp& ts) const
{
	return this->_ts - ts._ts;
}


inline SoraTimestamp& SoraTimestamp::operator += (SoraTimestamp::TimeDiff d)
{
	this->_ts += d;
	return *this;
}


inline SoraTimestamp& SoraTimestamp::operator -= (SoraTimestamp::TimeDiff d)
{
	this->_ts -= d;
	return *this;
}


inline std::time_t SoraTimestamp::epochTime() const
{
	return std::time_t(this->_ts/Resolution());
}


inline SoraTimestamp::UtcTimeVal SoraTimestamp::utcTime() const
{
	return this->_ts*10 + (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
}


inline SoraTimestamp::TimeVal SoraTimestamp::epochMicroseconds() const
{
	return this->_ts;
}


inline SoraTimestamp::TimeDiff SoraTimestamp::elapsed() const
{
	SoraTimestamp now;
	return now - *this;
}


inline bool SoraTimestamp::isElapsed(SoraTimestamp::TimeDiff interval) const
{
	SoraTimestamp now;
	SoraTimestamp::TimeDiff diff = now - *this;
	return diff >= interval;
}


inline SoraTimestamp::TimeVal SoraTimestamp::Resolution()
{
	return 1000000;
}


inline void swap(SoraTimestamp& s1, SoraTimestamp& s2)
{
	s1.swap(s2);
}


} // namespace Common


#endif // _Timestamp_INCLUDED
