/*
 *  SoraSoundEffectManager.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_SOUND_EFFECT_MANAGER_H_
#define SORA_SOUND_EFFECT_MANAGER_H_

#include "SoraPlatform.h"
#include "SoraSoundFile.h"
#include "SoraSingleton.h"
#include "SoraStringId.h"

namespace sora {
		
	typedef uint32 SoundEffectId;
	/*
	 a global sound effect manager for fast and convinient sound effect playback
	 especially in scripts
	 */
	class SoraSoundEffectManager: public SoraSingleton<SoraSoundEffectManager> {
		friend class SoraSingleton<SoraSoundEffectManager>;
		
	protected:
		SoraSoundEffectManager();
		~SoraSoundEffectManager();
		
	public:
		
		/*
		 load a sound effect into the manager
		 if cannot load the effect, the @retval would be -1
		 @param sid = the id of the sound effect, if it already has been taken, then would return next available id
		 otherwise @retval = sid, so never take se id -1
		 */
		SoundEffectId load(const std::wstring& effectName, SoundEffectId sid);
		void unload(SoundEffectId);
				
		SoraSoundEffectFile* get(SoundEffectId) const;
		
		bool play(SoundEffectId effId);
		void stop(SoundEffectId effId);

		bool playDirect(const std::wstring& effectName);
		
		/* applies to global effects */
		/* 0 - 100 */
		void setVolume(float vol);
		float getVolume() const;
		
	private:
		typedef sora_hash_map<SoundEffectId, SoraSoundEffectFile*> SOUND_EFFECT_MAP;
		SOUND_EFFECT_MAP mSoundEffects;
		
		typedef sora_hash_map<stringId, SoraSoundEffectFile*> LOADED_EFFECT_MAP;
		LOADED_EFFECT_MAP mLoadedEffects;
		
		float effectVolume;
	};
	
	
} // namespace sora


#endif // SORA_SOUND_EFFECT_MANAGER_H_