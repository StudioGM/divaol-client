/*
 *  SoraSoundEffectManager.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraSoundEffectManager.h"
#include "SoraEnvValues.h"
#include "SoraCore.h"

namespace sora {
	
	SoraSoundEffectManager::SoraSoundEffectManager(): effectVolume(100.f) {
		SET_ENV_FLOAT("SoundEffectVolume", effectVolume);
	}
	
	SoraSoundEffectManager::~SoraSoundEffectManager() {
		SOUND_EFFECT_MAP::iterator itEffect = mSoundEffects.begin();
		for(itEffect = mSoundEffects.begin(); itEffect != mSoundEffects.end(); ++itEffect) {
			if(itEffect->second != NULL) {
				delete itEffect->second;
				itEffect->second = NULL;
			}
		}
		mSoundEffects.clear();
	}

	SoundEffectId SoraSoundEffectManager::load(const std::wstring& effectName, SoundEffectId sid) {
		LOADED_EFFECT_MAP::iterator itPrev = mLoadedEffects.find(str2idnc(effectName));
		if(itPrev != mLoadedEffects.end()) {
			mSoundEffects[sid] = itPrev->second;
			return sid;
		}
		
		SOUND_EFFECT_MAP::iterator itEffect = mSoundEffects.find(sid);
		while(itEffect != mSoundEffects.end() && (itEffect->second != NULL)) {
			sid++;
			itEffect = mSoundEffects.find(sid);
		} 
		
		SoraSoundEffectFile* pefile = SoraCore::Ptr->createSoundEffectFile(effectName);
		if(pefile == NULL)
			return 0;
		
		mSoundEffects[sid] = pefile;
		mLoadedEffects[(str2idnc(effectName))] = pefile;
		return sid;
	}
	
	void SoraSoundEffectManager::unload(SoundEffectId eid) {
		SOUND_EFFECT_MAP::iterator itEffect = mSoundEffects.find(eid);
		if(itEffect != mSoundEffects.end() && (itEffect->second != NULL)) {
			delete itEffect->second;
			itEffect->second = NULL;
			
			mSoundEffects.erase(itEffect);
		}
	}
	
	SoraSoundEffectFile* SoraSoundEffectManager::get(SoundEffectId eid) const {
		SOUND_EFFECT_MAP::const_iterator itEffect = mSoundEffects.find(eid);
		if(itEffect != mSoundEffects.end() && (itEffect->second != NULL)) {
			return itEffect->second;
		}
		return NULL;
	}
	
	bool SoraSoundEffectManager::play(SoundEffectId eid) {
		SOUND_EFFECT_MAP::iterator itEffect = mSoundEffects.find(eid);
		if(itEffect != mSoundEffects.end() && (itEffect->second != NULL)) {
			itEffect->second->play();
			return true;
		}
		return false;
	}
	
	void SoraSoundEffectManager::stop(SoundEffectId eid) {
		SOUND_EFFECT_MAP::iterator itEffect = mSoundEffects.find(eid);
		if(itEffect != mSoundEffects.end() && (itEffect->second != NULL)) {
			itEffect->second->stop();
		}
	}
	
	void SoraSoundEffectManager::setVolume(float vol) {
		SET_ENV_FLOAT("SoundEffectVolume", vol);
		
		SOUND_EFFECT_MAP::iterator itEffect = mSoundEffects.begin();
		while(itEffect != mSoundEffects.end()) {
			if(itEffect->second != NULL)
				itEffect->second->setVolume(vol);
			++itEffect;
		}
		effectVolume = vol;
	}
	
	float SoraSoundEffectManager::getVolume() const {
		return effectVolume;
	}
	
	bool SoraSoundEffectManager::playDirect(const std::wstring& effectName) {
		//SoundEffectId eid = str2id(effectName);
		//if(!play(eid)) {
			SoraSoundEffectFile* pefile = SoraCore::Ptr->createSoundEffectFile(effectName);
			if(pefile == NULL)
				return false;
			
		//	mSoundEffects[eid] = pefile;
			pefile->setVolume(effectVolume);
			pefile->play();
			return true;
		//}
		//return false;
	}

} // namespace sora