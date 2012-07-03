/*
 *  SoraTimer.h
 *  Hoshizora
 *
 *  Created by Robert Bu on 8/24/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#ifndef SORA_TIMER_H_
#define SORA_TIMER_H_

#include "SoraPlatform.h"

namespace sora {
	
#define SORA_FPS_INFINITE 1000
	
	class SORA_API SoraTimer {
	public:
		virtual void setFPS(int32 fps) = 0;
		virtual float getDelta() = 0;
		virtual float getFPS() = 0;
		virtual bool update() = 0;
        
        static float TimeScale;
        static uint64 FrameCount;
	};
}

#endif