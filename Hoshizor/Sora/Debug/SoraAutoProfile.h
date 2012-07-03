/*
 *  SoraAutoProfile.h
 *  Sora
 *
 *  Created by GriffinBu on 1/23/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_AUTO_PROFILE_H_
#define SORA_AUTO_PROFILE_H_

#include "SoraGlobalProfiler.h"

#ifdef OS_OSX
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

#ifdef OS_PSP
#include <psprtc.h>
#endif

#include "SoraTimestamp.h"

namespace sora {
	
	struct SORA_API SoraAutoProfile {
		SoraAutoProfile(const char* name): 
        sName(name), 
        time(sora::SoraTimestamp()) {
        }
        
		~SoraAutoProfile() {
			SoraProfiler->storeProfile(sName, time.elapsed());
		}
		
		const char* sName;
        SoraTimestamp time;
	};
	
	#define SORA_PROFILE(name) ::sora::SoraAutoProfile(name)
} // namespace sora

#endif