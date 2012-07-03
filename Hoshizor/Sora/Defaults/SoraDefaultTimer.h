/*
 *  SoraDefaultTimer.h
 *  Hoshizora
 *
 *  Created by Robert Bu on 8/24/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#ifndef SORA_OSX_TIMER_H
#define SORA_OSX_TIMER_H

#include "SoraTimer.h"

#include "SoraTimestamp.h"

namespace sora {
	class SoraDefaultTimer: public SoraTimer {
	public:
		SoraDefaultTimer(): fpsInterval(SORA_FPS_INFINITE), fps(0.f) {
			time.update();
			oldtime = time;
		}
		
		void setFPS(int32 fps) {
			fpsInterval = (uint64)((1.f / fps) * 1000 * 1000);
			this->fps = (float)fps;
		}
		
		float getFPS()	{ return fps; }
		float getDelta()	{ return fdt; }
		
		bool update() {
			if(fpsInterval == SORA_FPS_INFINITE) {
				if(!(time == oldtime)) {
					fps = 1.f / ((time - oldtime)/(float)(1000*1000));
					oldtime = time;
					time.update();
				} else {
					time.update();
				}
				return true;
			}

			if(SoraTimestamp::CurrentTime()-oldtime.epochMicroseconds() < fpsInterval) {
				for(;;) {
                    uint64 ldt = SoraTimestamp::CurrentTime() - oldtime.epochMicroseconds();
					if(ldt >= fpsInterval - 1000) {
						for(;;)
							if(SoraTimestamp::CurrentTime() - oldtime.epochMicroseconds() >= fpsInterval)
								break;
						break;
					} else {
						msleep(1);
					}
				}
			}
            time.update();
            
            if(oldtime <= time - fpsInterval && oldtime >= time - (fpsInterval*2)) {
				fdt = fpsInterval / (float)(1000*1000);
				oldtime += fpsInterval;
			} else {
				fdt = (time-oldtime) / (float)(1000*1000);
				oldtime = time;
			}
			/* too long */
			if(fdt >= 1.f)
				fdt = 1.f / fps;
			
			fps = 1.f / fdt;
			
			return true;
		}
		
	private:
		SoraTimestamp time, oldtime;
		uint64 fpsInterval;
        float fps;
		float fdt;
    };
}

#endif