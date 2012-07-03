/*
 *  SoraOSXTimer.h
 *  Sora
 *
 *  Created by GriffinBu on 1/22/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_PSP_TIMER_H_
#define SORA_PSP_TIMER_H_

#include "../SoraTimer.h"
#include "../SoraPlatform.h"

#ifdef OS_PSP

#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h> 
#include <time.h>
#include <psprtc.h>

namespace sora {
	
	class SoraPSPTimer: public SoraTimer {
	public:
		SoraPSPTimer(): fpsInterval(SORA_FPS_INFINITE), frameCounter(0), fps(0.f), fTime(0.f), fTimeScale(1.f) {
			tickFrequency = sceRtcGetTickResolution();
			sceRtcGetCurrentTick(&time);
			oldtime = time;
		}
		
		void setFPS(int32 fps) {
			fpsInterval = 1.f / fps / (float)tickFrequency;
			this->fps = (float32)fps;
		}
		
		float32 getFPS()	{ return fps; }
		float32 getDelta()	{ return fdt; }
		float32 getTime() { return fTime; }
		int32 getFrameCount() { return frameCounter; }
		void setTimeScale(float32 ts) { fTimeScale = ts; }
		float32 getTimeScale() { return fTimeScale; }
		uint64 getCurrentSystemTime() { 
			uint64 t;
			sceRtcGetCurrentTick(&t);
			return t;
		}
		
		bool update() {
			if(fpsInterval == SORA_FPS_INFINITE) {
				if(oldtime != time) {
					oldtime = time;
					sceRtcGetCurrentTick(&time);
					fdt = (time - oldtime) / tickFrequency;
					fps = 1.f / fdt;
				} else {
					sceRtcGetCurrentTick(&time);
				}
				return true;
			}
		/*	
			if(time - oldtime < fpsInterval) {
				for(;;) {
					uint64 nanodt = time - oldtime;
					if(nanodt >= fpsInterval - 5) {
						for(;;)
							if(machTimeToNano(mach_absolute_time() - oldtime) >= fpsInterval)
								break;
						break;
					} else {
						msleep(1);
					}
				}
			}*/
			sceRtcGetCurrentTick(&time);
			
			if(oldtime <= time - fpsInterval && oldtime >= time - (fpsInterval*2)) {
				fdt = fpsInterval * tickFrequency * fTimeScale;
				oldtime += fpsInterval;
			} else {
				fdt = (time-oldtime)/tickFrequency * fTimeScale;
				oldtime = time;
			}
			if(fdt >= 1.f)
				fdt = 1.f / fps;
			
			fps = 1.f / fdt;
			
			frameCounter++;
			fTime += fdt;
			return true;
		}
		
	private:		
		uint64 time, oldtime;
		uint64 fpsInterval;
		uint64 tickFrequency;
		
		float32 fps;
		float32 fdt;
		int32 frameCounter;
		float32 fTime;
		float32 fTimeScale;
	};
	
} // namespace sora

#endif // OS_PSP
#endif // SORA_PSP_TIMER_H_