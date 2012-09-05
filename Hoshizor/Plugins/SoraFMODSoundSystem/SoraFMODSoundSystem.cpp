//
//  SoraFMODSoundSystem.cpp
//  Sora
//
//  Created by Griffin Bu on 4/18/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraFMODSoundSystem.h"
#include "SoraFMODSoundFile.h"
#include "SoraCore.h"
#include "SoraLogger.h"

namespace sora {
    
    SoraFMODSoundSystem::SoraFMODSoundSystem() {
        pfs = SoraFMODSystem::Instance();
    }
        
    int32 SoraFMODSoundSystem::init() {
        return pfs->init(32, FMOD_INIT_NORMAL, NULL);
    }
    
    void SoraFMODSoundSystem::shutdown() {
        SoraFMODSystem::Destroy();
    }
    
    SoraMusicFile* SoraFMODSoundSystem::createMusicFile(const SoraWString& path, bool bStream) {
        return new SoraFMODMusicFile(path, bStream);
    }
    
    SoraSoundEffectFile* SoraFMODSoundSystem::createSoundEffectFile(const SoraWString& path) {
        return new SoraFMODSoundEffectFile(path);
    }
    
    SoraMusicFile* SoraFMODSoundSystem::createMusicFile(bool bStream) {
        return new SoraFMODMusicFile(bStream);
    }
    
    SoraSoundEffectFile* SoraFMODSoundSystem::createSoundEffectFile() {
        return new SoraFMODSoundEffectFile;
    }
    
    SoraMusicFile* SoraFMODSoundSystem::createMusicFileFromMemory(void* pdata, uint32 size) {
        SoraFMODMusicFile* file = new SoraFMODMusicFile(true);
        if(file->readFileMem(pdata, size))
            return file;
        else {
            delete file;
            return 0;
        }
        return 0;
    }
    
    SoraSoundEffectFile* SoraFMODSoundSystem::createSoundEffectFileFromMemory(void* pdata, uint32 size) {
        SoraFMODSoundEffectFile* file = new SoraFMODSoundEffectFile();
        if(file->readFileMem(pdata, size))
            return file;
        else {
            delete file;
            return 0;
        }
        return 0;
    }
    
    void SoraFMODSoundSystem::update() {
        pfs->update();
    }
    
#ifdef SORA_AUTOMATIC_REGISTER
    //SORA_STATIC_RUN_CODE(SoraCore::Instance()->registerSoundSystem(new SoraFMODSoundSystem()));
#endif
   
} // namespace sora