/*
 *  SoraSoundLoader.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/26/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_GCN_SOUND_LOADER_H_
#define SORA_GCN_SOUND_LOADER_H_

#include "guichan.hpp"

#include "SoraSingleton.h"
#include "SoraPlatform.h"
#include "SoraStringId.h"

#include "SoraSoundFile.h"

#include <map>

namespace gcn {
	
	/*
	 SoraSound loader for GUIChan with cache support
	 */
	class SoraGUISoundLoader: public gcn::SoundLoader {
	public:
		SoraGUISoundLoader();
		virtual ~SoraGUISoundLoader();
		
		gcn::Sound* load(const std::string& soundName);
        void play(const std::string& soundName);
		
	private:
		typedef std::map<sora::SoraStringId, sora::SoraSoundEffectFile*> SoundMap;
		SoundMap mSoundMap;
	};
	
} // namespace sora


#endif // SORA_SOUND_LOADER_H_