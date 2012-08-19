/*
 *  DivaFmodMusicManager.cpp
 *
 *  Created by Hyf042 on 1/19/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#include "SoraCore.h"
#include "SoraFileUtility.h"
#include "DivaFmodMusicManager.h"

namespace divacore
{
	/*
	FmodMusicManager
	*/
	FMOD::Sound* FmodMusicManager::loadSound(const std::string &file, bool stream)
	{
		FMOD_RESULT result;
		FMOD::Sound *pSound;

		if(!SoraFileUtility::FileExists(file)) {            
			uint32 size;
			void* data = SoraCore::Ptr->getResourceFile(file, size);
			if(data) {
				FMOD_CREATESOUNDEXINFO exinfo;
				exinfo.length = size;
				if(!stream)
					result = pSystem->createSound((const char*)data, FMOD_CREATESAMPLE | FMOD_SOFTWARE | FMOD_2D | FMOD_OPENMEMORY, &exinfo, &pSound);
				else
					result = pSystem->createStream((const char*)data, FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_OPENMEMORY, &exinfo, &pSound);
				SoraCore::Ptr->freeResourceFile(data);
				if(result!=FMOD_OK)
					DIVA_EXCEPTION_MODULE("Creating "+file+": failured","FmodMusicManager");
			} else
				DIVA_EXCEPTION_MODULE("Creating "+file+": File not existed","FmodMusicManager");
		} else {
			if(!stream)
				result = pSystem->createSound(file.c_str(), FMOD_CREATESAMPLE | FMOD_SOFTWARE | FMOD_2D, 0, &pSound);
			else 
				result = pSystem->createStream(file.c_str(), FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &pSound);
			if(result!=FMOD_OK)
				DIVA_EXCEPTION_MODULE("Creating "+file+": failured","FmodMusicManager");
		}
		return pSound;
	}

	FMOD::Sound* FmodMusicManager::getSound(const std::string &ID)
	{
		if(soundPool.find(ID)==soundPool.end())
			DIVA_EXCEPTION_MODULE("Sound "+ID+" not exists","FmodMusicManager");
		return soundPool[ID];
	}
	FMOD::Channel* FmodMusicManager::getChannel(const std::string &channel)
	{
		MUSICPOOL::iterator ptr = musicPool.find(channel);
		if(ptr==musicPool.end())
			DIVA_EXCEPTION_MODULE("Channel: "+channel+" is not existed","MusicManager");
		return ptr->second.second;
	}
	void FmodMusicManager::init()
	{
		if(pSystem)
			DIVA_EXCEPTION_MODULE("FMOD System already exists","FmodMusicManager");
		else {
			FMOD_RESULT result = FMOD::System_Create(&pSystem);
			if(result==FMOD_OK) {
				result = pSystem->init(maxchannel, FMOD_INIT_NORMAL, NULL);
				if(result==FMOD_OK)
					return;
			}
		}
		DIVA_EXCEPTION_MODULE("FMOD System init failed","FmodMusicManager");
	}
	void FmodMusicManager::destroy()
	{
		clear();
	}
	void FmodMusicManager::clear()
	{
		for(MUSICPOOL::iterator ptr = musicPool.begin(); ptr != musicPool.end(); ptr++)
			ptr->second.second->stop();
		for(SOUNDPOOL::iterator ptr = soundPool.begin(); ptr != soundPool.end(); ptr++)
			ptr->second->release();
		soundPool.clear();
		musicPool.clear();
		volumePool.clear();
		tagsVolume.clear();
		musicList.clear();
	}
	void FmodMusicManager::load(const std::string &file, const std::string &ID, bool stream)
	{
		if(soundPool.find(ID)!=soundPool.end())
			DIVA_EXCEPTION_MODULE("Sound "+ID+" already exists","FmodMusicManager");
		soundPool[ID] = loadSound(file,stream);
	}
	void FmodMusicManager::unload(const std::string &ID)
	{
		SOUNDPOOL::iterator soundPtr = soundPool.find(ID);
		if(soundPtr==soundPool.end())
			DIVA_EXCEPTION_MODULE("Sound "+ID+" does not exist","FmodMusicManager");
		soundPtr->second->release();
		soundPool.erase(soundPtr);
	}

	void FmodMusicManager::play(const std::string &ID, const std::string &channel, const std::string &tag)
	{
		FMOD::Sound *pSound = getSound(ID);
		FMOD::Channel *pChannel;
		if(musicPool.find(channel)!=musicPool.end())
		{
			pChannel = musicPool[channel].second;
			pChannel->stop();
		}
		else
			volumePool[channel] = 1.0;
		if(pSound) {
            FMOD_RESULT result = pSystem->playSound(FMOD_CHANNEL_FREE, pSound, false, &pChannel);
			pChannel->setVolume(getTagVolume(tag)*volumePool[channel]);
        }
		musicPool[channel] = std::make_pair<std::string,FMOD::Channel*>(tag,pChannel);
	}
	void FmodMusicManager::playDirect(const std::string &ID, const std::string &tag)
	{
		FMOD::Sound *pSound = getSound(ID);
		FMOD::Channel *pChannel;
		if(pSound) {
            FMOD_RESULT result = pSystem->playSound(FMOD_CHANNEL_FREE, pSound, false, &pChannel);
			pChannel->setVolume(getTagVolume(tag));
        }
	}
	void FmodMusicManager::playDirectWithFile(const std::string &file, bool stream, const std::string &tag)
	{
		FMOD::Sound *pSound = loadSound(file,false);
		FMOD::Channel *pChannel;
		if(pSound) {
            FMOD_RESULT result = pSystem->playSound(FMOD_CHANNEL_FREE, pSound, false, &pChannel);
			pChannel->setVolume(getTagVolume(tag));
        }
	}
	void FmodMusicManager::setVolume(const std::string &channel, float volume)
	{
		if(musicPool.find(channel)==musicPool.end())
			DIVA_EXCEPTION_MODULE("Channel: "+channel+" is not existed","MusicManager");
		volumePool[channel] = volume;
		SOUNDPAIR pair = musicPool[channel];
		pair.second->setVolume(getTagVolume(pair.first)*volume);
	}
	void FmodMusicManager::setTagVolume(const std::string &tag, float volume)
	{
		float vFactor = getTagVolume(tag);
		tagsVolume[tag] = volume;
		for(MUSICPOOL::iterator ptr = musicPool.begin(); ptr != musicPool.end(); ptr++)
			ptr->second.second->setVolume(getVolume(ptr->first)*volume);
	}
	float FmodMusicManager::getVolume(const std::string &channel)
	{
		return volumePool[channel];
	}
	float FmodMusicManager::getTagVolume(const std::string &tag)
	{
		if(tagsVolume.find(tag)==tagsVolume.end())
			return 1.0;
		else
			return tagsVolume[tag];
	}
	void FmodMusicManager::setLoop(const std::string &channel, bool flag)
	{
		getChannel(channel)->setMode(flag ? FMOD_LOOP_NORMAL: FMOD_LOOP_OFF);
	}
	bool FmodMusicManager::isLoop(const std::string &channel)
	{
		FMOD_MODE mode;
        getChannel(channel)->getMode(&mode);
        return mode & FMOD_LOOP_NORMAL;
	}
	bool FmodMusicManager::isPlaying(const std::string &channel)
	{
		bool bPlay;
		getChannel(channel)->isPlaying(&bPlay);
		return bPlay;
	}
	void FmodMusicManager::setPosition(const std::string &channel,double time)
	{
		uint32 length;
		FMOD::Sound *currentSound = NULL;
		FMOD::Channel *currentChannel = getChannel(channel);
		currentChannel->getCurrentSound(&currentSound);
		currentSound->getLength(&length,FMOD_TIMEUNIT_MS);

		if(time*1000>=length)
		{
			currentChannel->setPosition(length-1,FMOD_TIMEUNIT_MS);
			currentChannel->setPaused(true);
		}
		else
			getChannel(channel)->setPosition(time*1000,FMOD_TIMEUNIT_MS);
	}
	double FmodMusicManager::getPosition(const std::string &channel)
	{
		try
		{
			uint32 position;
			getChannel(channel)->getPosition(&position,FMOD_TIMEUNIT_MS);
			return position/1000.0;
		}
		catch(...)
		{
			return 0;
		}
	}
	double FmodMusicManager::getLength(const std::string &channel)
	{
		uint32 length;
		FMOD::Sound *currentSound = NULL;
		getChannel(channel)->getCurrentSound(&currentSound);
		currentSound->getLength(&length,FMOD_TIMEUNIT_MS);
		return length/1000.0;
	}
	void FmodMusicManager::stop()
	{
		for(MUSICPOOL::iterator ptr = musicPool.begin(); ptr != musicPool.end(); ptr++)
			ptr->second.second->stop();
	}
	void FmodMusicManager::pause()
	{
		for(MUSICPOOL::iterator ptr = musicPool.begin(); ptr != musicPool.end(); ptr++)
			ptr->second.second->setPaused(true);
	}
	void FmodMusicManager::resume()
	{
		for(MUSICPOOL::iterator ptr = musicPool.begin(); ptr != musicPool.end(); ptr++)
			ptr->second.second->setPaused(false);
	}
}