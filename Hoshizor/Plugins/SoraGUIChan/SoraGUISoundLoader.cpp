/*
 *  SoraSoundLoader.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/26/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraGUISoundLoader.h"
#include "SoraGUISound.h"
#include "SoraCore.h"
#include "SoraLogger.h"

namespace gcn {
	
	SoraGUISoundLoader::SoraGUISoundLoader() {
		Sound::setSoundLoader(this);
	}
	
	SoraGUISoundLoader::~SoraGUISoundLoader() {
		Sound::setSoundLoader(NULL);
		
		for(SoundMap::iterator itSound = mSoundMap.begin(); itSound != mSoundMap.end(); ++itSound) {
			if(itSound->second != NULL) {
				delete (itSound->second);
			}
		}
	}

	Sound* SoraGUISoundLoader::load(const std::string& soundName) {
		sora::SoraStringId soundid = sora::GetUniqueStringId(soundName);
		SoraGUISound* psound = new SoraGUISound;
		
		SoundMap::iterator itSound = mSoundMap.find(soundid);
		if(itSound != mSoundMap.end() && (itSound->second != NULL)) {
			psound->pfile = itSound->second;
			return psound;
		} else {
			sora::SoraSoundEffectFile* psefile = sora::SoraCore::Ptr->createSoundEffectFile(sora::s2ws(soundName));
			if(psefile != NULL) {
				mSoundMap[soundid] = psefile;
				psound->pfile = psefile;
				return psound;
			} else
                sora::log_error("SoraGUISoundLoader: Error loading sound file");
		}
		
		return NULL;
	}
    
    void SoraGUISoundLoader::play(const std::string& soundName) {
        sora::SoraStringId soundid = sora::GetUniqueStringId(soundName);
        SoundMap::iterator itSound = mSoundMap.find(soundid);
		if(itSound != mSoundMap.end() && (itSound->second != NULL)) {
            itSound->second->play();
        } else {
            Sound* sound = load(soundName);
            if(sound) {
                sound->play();
                delete sound;
            }
        }
    }

} // namespace gcn