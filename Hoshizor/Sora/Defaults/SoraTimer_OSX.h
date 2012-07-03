/*
 *  SoraOSXTimer.h
 *  Sora
 *
 *  Created by GriffinBu on 1/22/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_OSX_TIMER_H_
#define SORA_OSX_TIMER_H_

#include "SoraTimer.h"
#include "SoraPlatform.h"

#if defined(OS_OSX) || defined(OS_IOS)

#include <mach/mach.h>
#include <mach/mach_time.h>
#if defined(OS_OSX)
#include <CoreServices/CoreServices.h>
#endif
#include <time.h>

// 500 nanoseconds
static uint64 TIME_PRECISION = 1000000;

void mach_absolute_difference(uint64_t end, uint64_t start, struct timespec *tp) {  
	uint64_t difference = end - start;  
	static mach_timebase_info_data_t info = {0,0};  
	
	if (info.denom == 0)  
		mach_timebase_info(&info);  
	
	uint64_t elapsednano = difference * (info.numer / info.denom);  
	
	tp->tv_sec = elapsednano * 1e-9;  
	tp->tv_nsec = elapsednano - (tp->tv_sec * 1e9);  
}

namespace sora {
	
	class SoraOSXTimer: public SoraTimer {
	public:
		SoraOSXTimer(): fpsInterval(SORA_FPS_INFINITE), fps(0.f) {
			time = mach_absolute_time();
			oldtime = time;
		}
		
		void setFPS(int32 fps) {
			fpsInterval = 1.f / fps * (1000*1000*1000);
			this->fps = (float)fps;
		}
		
		float getFPS()	{ return fps; }
		float getDelta()	{ return fdt; }
		
		bool update() {
			if(fpsInterval == SORA_FPS_INFINITE) {
				if(oldtime != time) {
					oldtime = time;
					time = mach_absolute_time();
					fdt = dtToMil(time-oldtime) / 1000.f;
					fps = 1.f / (dtToMil(time-oldtime)/1000.f);;
				} else {
					time = mach_absolute_time();
				}
				return true;
			}
			
			if(machTimeToNano(mach_absolute_time() - oldtime) < fpsInterval) {
				for(;;) {
					uint64 nanodt = machTimeToNano(mach_absolute_time() - oldtime);
					if(nanodt >= fpsInterval - TIME_PRECISION) {
						for(;;)
							if(machTimeToNano(mach_absolute_time() - oldtime) >= fpsInterval)
								break;
						break;
					} else {
						msleep(1);
					}
				}
			}
			time = mach_absolute_time();
			
			if(oldtime <= time - fpsInterval && oldtime >= time - (fpsInterval*2)) {
				fdt = dtToMil(fpsInterval) / 1000.f;
				oldtime += fpsInterval;
			} else {
				fdt = dtToMil(time-oldtime) / 1000.f;
				oldtime = time;
			}
			if(fdt >= 1.f)
				fdt = 1.f / fps;
			
			fps = 1.f / fdt;
			
			return true;
		}
		
	private:
		inline uint64_t machTimeToNano(uint64_t dt) {
#if defined(OS_IOS)
			return dt;
#elif defined(OS_OSX) 
			Nanoseconds elapsedNano = (AbsoluteToNanoseconds( *(AbsoluteTime *) &dt ));
			return * (uint64_t *) &elapsedNano;
#endif
		}
		
		inline double dtToMil(uint64_t dt) {
			return ((double)machTimeToNano(dt) / (1000*1000));
		}
		
		uint64_t time, oldtime;
		uint64_t fpsInterval;
		float fps;
		float fdt;
	};
	
} // namespace sora

#endif // OS_OSX

#endif