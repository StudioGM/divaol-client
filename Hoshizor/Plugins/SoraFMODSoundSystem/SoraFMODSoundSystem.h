//
//  SoraFMODSoundSystem.h
//  Sora
//
//  Created by Griffin Bu on 4/18/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef SORA_FMOD_SOUND_SYSTEM_H_
#define SORA_FMOD_SOUND_SYSTEM_H_

#include "SoraPlatform.h"
#include "SoraSoundSystem.h"

#include "SoraFMODSystem.h"

namespace sora {
    
    class SoraFMODSoundSystem: public SoraSoundSystem {
    public:
        SoraFMODSoundSystem();
        
        int32 init();
		void shutdown();
		
        SoraMusicFile* createMusicFile(const SoraWString& path, bool bStream=false);
        SoraSoundEffectFile* createSoundEffectFile(const SoraWString& path);
        SoraMusicFile* createMusicFile(bool bStream=false);
        SoraSoundEffectFile* createSoundEffectFile();
        SoraMusicFile* createMusicFileFromMemory(void* pdata, uint32 size);
        SoraSoundEffectFile* createSoundEffectFileFromMemory(void* pdata, uint32 size);
		/*
         update sound device
         */
		void update();
        
    private:
        SoraFMODSystem* pfs;
    };
} // namespace sora


#endif