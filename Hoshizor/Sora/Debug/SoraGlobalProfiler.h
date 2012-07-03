/*
 *  SoraGlobalProfiler.h
 *  Sora
 *
 *  Created by GriffinBu on 1/23/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_GLOBAL_PROFILER_H_
#define SORA_GLOBAL_PROFILER_H_

#include "SoraCore.h"
#include "SoraSingleton.h"

#include <iostream>
#include <fstream>
#include <stdio.h>

#include "SoraInternalLogger.h"

namespace sora {
	
	class SORA_API SoraGlobalProfiler: public SoraSingleton<SoraGlobalProfiler> {
		friend class SoraSingleton<SoraGlobalProfiler>;
	public:
		struct Profile {
			SoraString sName;
			uint64 elapsedTime;
			
			Profile(): elapsedTime(0) {}
			Profile(const char* name, uint64 t): sName(name), elapsedTime(t) {}
		};
		typedef sora_hash_map<SoraString, Profile> PROFILE_CONT;

	public:
		void storeProfile(const char* name, uint64 elapsedTime) {
			PROFILE_CONT::iterator itprofile = profiles.find(name);
			if(itprofile == profiles.end()) {
				log_mssg(vamssg("ProfileName=%s, time=%llu\n", name, elapsedTime), LOG_LEVEL_NOTICE);
			}
			profiles[name] = Profile(name, elapsedTime);
		}
		
		void printProfiles() {
			PROFILE_CONT::iterator itprofile = profiles.begin();
			while(itprofile != profiles.end()) {
				log_mssg(vamssg("ProfileName=%s, time=%llu\n", itprofile->second.sName.c_str(), itprofile->second.elapsedTime),
							  LOG_LEVEL_NOTICE);
				++itprofile;
			}
		}
		
		void logProfiles() {
			PROFILE_CONT::iterator itprofile = profiles.begin();
			while(itprofile != profiles.end()) {
				log_mssg(vamssg("ProfileName=%s, time=%llu", itprofile->second.sName.c_str(), itprofile->second.elapsedTime),
							   LOG_LEVEL_NOTICE);
				++itprofile;
			}
		}
		
		std::ostream& operator<<(std::ostream& stream) {
			PROFILE_CONT::iterator itprofile = profiles.begin();
			while(itprofile != profiles.end()) {
				stream << itprofile->second.sName.c_str() << itprofile->second.elapsedTime << std::endl;
				++itprofile;
			}
			return stream;
		}
		
		void printProfile(const char* name) {
			PROFILE_CONT::iterator itprofile = profiles.find(name);
			if(itprofile != profiles.end()) {
				log_mssg(vamssg("ProfileName=%s, time=%llu", itprofile->second.sName.c_str(), itprofile->second.elapsedTime),
							  LOG_LEVEL_NOTICE);
			}
		}
		
		Profile getProfile(const char* name) const {
			PROFILE_CONT::const_iterator itprofile = profiles.find(name);
			if(itprofile != profiles.end()) {
				return itprofile->second;
			}
			return Profile();
		}
		
		void flush() {
			profiles.clear();
		}
		
		const PROFILE_CONT& getProfiles() const { return profiles; }
		
	private:
		PROFILE_CONT profiles;
	};
	
	static SoraGlobalProfiler* SoraProfiler = SoraGlobalProfiler::Instance();
	
	
} // namespace sora

#endif