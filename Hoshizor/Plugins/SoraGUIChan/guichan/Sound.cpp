/*
 *  Sound.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/26/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "guichan/Sound.hpp"
#include "guichan/SoundLoader.hpp"

namespace gcn {

	SoundLoader* Sound::mSoundLoader = NULL;
	
	Sound::Sound() {
	}
	
	Sound::~Sound() {
	}
	
	SoundLoader* Sound::getSoundLoader() {
		return mSoundLoader;
	}
	
	void Sound::setSoundLoader(SoundLoader* sl) {
		mSoundLoader = sl;
	}

	Sound* Sound::load(const std::string& soundName) {
		if(mSoundLoader) {
			return mSoundLoader->load(soundName);
		}
		return NULL;
	}

} // namespace gcn