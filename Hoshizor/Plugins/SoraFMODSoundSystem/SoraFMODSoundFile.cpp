//
//  SoraFMODSoundFile.cpp
//  Sora
//
//  Created by Griffin Bu on 4/18/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraFMODSoundFile.h"
#include "SoraFileUtility.h"
#include "SoraCore.h"

namespace sora {
    
#ifndef OS_WIN32
    static FMOD_RESULT SORACALL myChannelCallback(FMOD_CHANNEL *channel, FMOD_CHANNEL_CALLBACKTYPE type, void *commanddata1, void *commanddata2) {
#else
	static FMOD_RESULT _stdcall myChannelCallback(FMOD_CHANNEL *channel, FMOD_CHANNEL_CALLBACKTYPE type, void *commanddata1, void *commanddata2) {
#endif
		SoraPlaybackEventHandler* pMusicFile = NULL;
        FMOD::Channel* pChannel = (FMOD::Channel*)channel;
        if(pChannel) {
            void* pmyData;
            pChannel->getUserData(&pmyData);
            if(pmyData)
                pMusicFile = (SoraFMODMusicFile*)pmyData;
        }
        
        if(!pMusicFile) {
            log_mssg("error converting event data object to playbackeventHandler", 
						  LOG_LEVEL_ERROR);
            return FMOD_ERR_EVENT_FAILED;
        }
        
        if(type == FMOD_CHANNEL_CALLBACKTYPE_END) {
			if(pMusicFile->isEventPublishEnabled())
				pMusicFile->publishEvent(SoraPlaybackEvent::Ended);
        }
        return FMOD_OK;
    }
    
    SoraFMODMusicFile::SoraFMODMusicFile(bool bStream) {
        pSystem = SoraFMODSystem::Instance()->getSystem();
        if(!pSystem)
            log_mssg("Creating FMODMusicFile: No FMOD System available", LOG_LEVEL_ERROR);
        
        pSound = NULL;
        pChannel = NULL;
        pSoundData = NULL;
		
		volume = 100.f;
		pan = 0.f;
		pitch = 0.f;
                
        bIsStream = bStream;
    }
    
    SoraFMODMusicFile::SoraFMODMusicFile(const StringType& path, bool bStream) {
        pSystem = SoraFMODSystem::Instance()->getSystem();
        if(!pSystem)
            log_mssg(vamssg("Creating %s: No FMOD System available", path.c_str()),
						  LOG_LEVEL_ERROR);
        bIsStream = bStream;
        
        pSound = NULL;
        pChannel = NULL;
        pSoundData = NULL;
		
		volume = 100.f;
		pan = 0.f;
		pitch = 0.f;

        if(!SoraFileUtility::FileExists(path)) {            
            uint32 size;
            void* data = SoraCore::Ptr->getResourceFile(path, size);
            if(data) {
                readFileMem(data, size);
                SoraCore::Ptr->freeResourceFile(data);
             } else 
                log_mssg(vamssg("Creating %s: File not exists", path.c_str()),
							  LOG_LEVEL_ERROR);
        } else {
            readFile(path);
        }
    }
    
    SoraFMODMusicFile::~SoraFMODMusicFile() {
		closeFile();
    }
    
    int32 SoraFMODMusicFile::readFile(const StringType& path) {
        if(!pSystem) return 0;
        if(pSound) closeFile();
        
        FMOD_RESULT result;
        if(!bIsStream)
            result = pSystem->createSound(path.c_str(), FMOD_CREATESAMPLE | FMOD_SOFTWARE | FMOD_2D, 0, &pSound);
        else 
            result = pSystem->createStream(path.c_str(), FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &pSound);
        return FMOD_ERROR_CHECK(result)==FMOD_OK?1:0;
    }
    
    int32 SoraFMODMusicFile::readFileMem(void* data, uint32 length) {
        if(!pSystem) return 0;
        if(pSound) closeFile();

        FMOD_RESULT result;
        FMOD_CREATESOUNDEXINFO exinfo;
        exinfo.length = length;
        if(!bIsStream)
            result = pSystem->createSound((const char*)data, FMOD_CREATESAMPLE | FMOD_SOFTWARE | FMOD_2D | FMOD_OPENMEMORY, &exinfo, &pSound);
        else
            result = pSystem->createStream((const char*)data, FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_OPENMEMORY, &exinfo, &pSound);
        return FMOD_ERROR_CHECK(result)==FMOD_OK?1:0;
    }
    
    void SoraFMODMusicFile::closeFile() {
        if(pSound) {
            if(pChannel) {
				pChannel->setUserData(0);
                pChannel->stop();
			}
            pSound->release();
        }
    }
    
    void SoraFMODMusicFile::play() {
        if(pSound) {
            if(pChannel)
                pChannel->stop();
            FMOD_RESULT result = pSystem->playSound(FMOD_CHANNEL_FREE, pSound, false, &pChannel);
			if(pChannel) {
				pChannel->setVolume(volume/100.f);
				if(pan != 0.f) pChannel->setPan(pan);
			}
			
            if(FMOD_ERROR_CHECK(result) && mEventPublish) {
                publishEvent(SoraPlaybackEvent::Started);
                
                // register FMOD Channel Callback to get SOUND_ENDED event
                pChannel->setUserData((void*)this);
                pChannel->setCallback(myChannelCallback);
                pChannel->setPaused(false);
            }
        }
    }
    
    void SoraFMODMusicFile::stop() {
        if(pChannel) {
            FMOD_RESULT result = pChannel->stop();
            if(FMOD_ERROR_CHECK(result) && mEventPublish)
				publishEvent(SoraPlaybackEvent::Stopped);
        }
    }
    
    void SoraFMODMusicFile::pause() {
        if(pChannel) {
            FMOD_RESULT result = pChannel->setPaused(true);
            if(FMOD_ERROR_CHECK(result) && mEventPublish) {
                publishEvent(SoraPlaybackEvent::Resumed);
            }
        }
    }
    
    void SoraFMODMusicFile::resume() {
        if(pChannel) {
            FMOD_RESULT result = pChannel->setPaused(false);
            if(FMOD_ERROR_CHECK(result) && mEventPublish) {
                publishEvent(SoraPlaybackEvent::Resumed);
            }
        }
    }
    
    void SoraFMODMusicFile::setVolume(float vol) {
        if(pChannel)
            pChannel->setVolume(vol);
		volume = vol;
    }
    
    void SoraFMODMusicFile::setPan(float pan) {
        if(pChannel)
            pChannel->setPan(pan);
		this->pan = pan;
    }
    
    void SoraFMODMusicFile::setPitch(float pitch) {
      //  if(pChannel)
      //      pChannel->setPitch(
		this->pitch = pitch;
    }
    
    float SoraFMODMusicFile::getVolume() const {
        if(pChannel) {
            float vol;
            FMOD_RESULT result = pChannel->getVolume(&vol);
            FMOD_ERROR_CHECK(result);
            return vol;
        }
        return 0.f;
    }
    
    float SoraFMODMusicFile::getPitch() const {
        return 0.f;
    }
    
    float SoraFMODMusicFile::getPan() const {
        if(pChannel) {
            float pan;
            FMOD_RESULT result = pChannel->getPan(&pan);
            FMOD_ERROR_CHECK(result);
            return pan;
        }
        return 0.f;
    }
    
    void SoraFMODMusicFile::setPosition(float x, float y, float z) {
        
    }
    
    void SoraFMODMusicFile::getPosition(float &x, float &y, float &z) {
    
    }
    
    void SoraFMODMusicFile::setMusicPosition(int32 pos) {
        if(pChannel)
            pChannel->setPosition(pos, FMOD_TIMEUNIT_MS);
    }
    
    int32 SoraFMODMusicFile::getMusicPosition() const {
        if(pChannel) {
            unsigned int pos;
            FMOD_RESULT result = pChannel->getPosition(&pos, FMOD_TIMEUNIT_MS);
            FMOD_ERROR_CHECK(result);
            return pos;
        }
        return 0;
    }
    
    int32 SoraFMODMusicFile::getLength() const {
        if(pSound) {
            unsigned int length;
            FMOD_RESULT result = pSound->getLength(&length, FMOD_TIMEUNIT_MS);
            FMOD_ERROR_CHECK(result);
            return length;
        }
        return 0;
    }
    
    void SoraFMODMusicFile::setRepeat(bool flag) {
        if(pChannel)
            pChannel->setMode(flag ? FMOD_LOOP_NORMAL: FMOD_LOOP_OFF);
    }
    
    bool SoraFMODMusicFile::isRepeat() const {
        if(pChannel) {
            FMOD_MODE mode;
            pChannel->getMode(&mode);
            return mode & FMOD_LOOP_NORMAL;
        }
        return 0;
    }
    
    bool SoraFMODMusicFile::isPlaying() const {
        if(pChannel) {
            bool bp;
            pChannel->isPlaying(&bp);
            return bp;
        }
        return false;
    }
    
    SoraFMODSoundEffectFile::SoraFMODSoundEffectFile() {
        pSystem = SoraFMODSystem::Instance()->getSystem();
        if(!pSystem)
            log_mssg("Creating FMODSoundEffectFile: No FMOD System available");
            
        pSound = NULL;
        pChannel = NULL;
		
		volume = 100.f;
		pan = 0.f;
		pitch = 0.f;
    }
    
    SoraFMODSoundEffectFile::SoraFMODSoundEffectFile(const StringType& path) {
        pSystem = SoraFMODSystem::Instance()->getSystem();
        if(!pSystem)
            log_mssg(vamssg("Creating %s: No FMOD System available", path.c_str()),
						  LOG_LEVEL_ERROR);
        
        pSound = NULL;
        pChannel = NULL;
        pSoundData = NULL;
		
		volume = 100.f;
		pan = 0.f;
		pitch = 0.f;
        
        if(!SoraFileUtility::FileExists(path)) {            
            uint32 size;
            void* data = SoraCore::Ptr->getResourceFile(path, size);
            if(data) {
                readFileMem(data, size);
                SoraCore::Ptr->freeResourceFile(data);
            } else 
                log_mssg(vamssg("Creating %s: File not exists", path.c_str()),
							  LOG_LEVEL_ERROR);
        } else {
            readFile(path);
        }
    }

	
	SoraFMODSoundEffectFile::~SoraFMODSoundEffectFile() {
		closeFile();
    }

	 void SoraFMODSoundEffectFile::closeFile() {
        if(pSound) {
            if(pChannel)
                pChannel->stop();
            pSound->release();
        }
    }
    
    int32 SoraFMODSoundEffectFile::readFile(const StringType& path) {
        if(!pSystem) return 0;
        if(pSound) closeFile();
        
        FMOD_RESULT result;
        result = pSystem->createSound(path.c_str(), FMOD_CREATESAMPLE | FMOD_SOFTWARE | FMOD_2D, 0, &pSound);
    
        return FMOD_ERROR_CHECK(result)==FMOD_OK?1:0;
    }
    
    int32 SoraFMODSoundEffectFile::readFileMem(void* data, uint32 length) {
        if(!pSystem) return 0;
        if(pSound) closeFile();
        
        FMOD_RESULT result;
        FMOD_CREATESOUNDEXINFO exinfo;
        exinfo.length = length;
        result = pSystem->createSound((const char*)data, FMOD_CREATESAMPLE | FMOD_SOFTWARE | FMOD_2D | FMOD_OPENMEMORY, &exinfo, &pSound);
        return FMOD_ERROR_CHECK(result)==FMOD_OK?1:0;
    }
    
    void SoraFMODSoundEffectFile::play() {
        if(pSound) {
			if(pChannel)
				pChannel->stop();
            FMOD_RESULT result = pSystem->playSound(FMOD_CHANNEL_FREE, pSound, false, &pChannel);
			if(pChannel) {
				pChannel->setVolume(volume/100.f);
				if(pan != 0.f) pChannel->setPan(pan);
			}
			if(FMOD_ERROR_CHECK(result) && mEventPublish) {
				
				
                publishEvent(SoraPlaybackEvent::Started);
                pChannel->setUserData((void*)this);
                pChannel->setCallback(myChannelCallback);
            }
        }
    }
    
    void SoraFMODSoundEffectFile::stop() {
        if(pChannel) {
            FMOD_RESULT result = pChannel->stop();
            if(FMOD_ERROR_CHECK(result)&& mEventPublish) {
                publishEvent(SoraPlaybackEvent::Stopped);
            }
        }
    }
    
    void SoraFMODSoundEffectFile::setVolume(float vol) {
        if(pChannel)
            pChannel->setVolume(vol);
		volume = vol;
    }
    
    void SoraFMODSoundEffectFile::setPan(float pan) {
        if(pChannel)
            pChannel->setPan(pan);
		this->pan = pan;
    }
    
    void SoraFMODSoundEffectFile::setPitch(float pitch) {
        //  if(pChannel)
        //      pChannel->setPitch(
		this->pitch = pitch;
    }
    
    float SoraFMODSoundEffectFile::getVolume() const {
        if(pChannel) {
            float vol;
            FMOD_RESULT result = pChannel->getVolume(&vol);
            FMOD_ERROR_CHECK(result);
            return vol;
        }
        return 0.f;
    }
    
    float SoraFMODSoundEffectFile::getPitch() const {
        return 0.f;
    }
    
    float SoraFMODSoundEffectFile::getPan() const {
        if(pChannel) {
            float pan;
            FMOD_RESULT result = pChannel->getPan(&pan);
            FMOD_ERROR_CHECK(result);
            return pan;
        }
        return 0.f;
    }
    
    void SoraFMODSoundEffectFile::setPosition(float x, float y, float z) {
        
    }
    
    void SoraFMODSoundEffectFile::getPosition(float &x, float &y, float &z) {
        
    }
    
        void SoraFMODSoundEffectFile::setRepeat(bool flag) {
        if(pChannel)
            pChannel->setMode(flag ? FMOD_LOOP_NORMAL: FMOD_LOOP_OFF);
    }
    
    bool SoraFMODSoundEffectFile::isRepeat() const {
        if(pChannel) {
            FMOD_MODE mode;
            pChannel->getMode(&mode);
            return mode & FMOD_LOOP_NORMAL;
        }
        return 0;
    }
    
    bool SoraFMODSoundEffectFile::isPlaying() const {
        if(pChannel) {
            bool bp;
            pChannel->isPlaying(&bp);
            return bp;
        }
        return false;
    }
} // namespace sora