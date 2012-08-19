/*
 *  DivaBassMusicManager.cpp
 *
 *  Created by Hyf042 on 1/19/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#include "SoraCore.h"
#include "SoraFileUtility.h"
#include "DivaBassMusicManager.h"
#include "Core/DivaCore.h"

namespace divacore
{
	/*
	Event Functions
	*/
	void BassMusicManager::playEvent(ARGUMENTS &arg)
	{
		ARGUMENTS argnew = arg;
		Argument::addDefaultArg("tag","",argnew);
		if(Argument::isArg("channel",argnew))
			play(Argument::asString("id",argnew),Argument::asString("channel",argnew),Argument::asString("tag",argnew));
		else
		{
			DIVA_EXCEPTION_MODULE("no channel","DivaMusicManager");
			//playDirect(Argument::asString("id",argnew),Argument::asString("tag",argnew));
		}

		if(Argument::isArg("__location__",argnew))
		{
			//LOGGER->log("music reset : %0.2lf",Argument::asDouble("__location__",argnew));
			setPosition(Argument::asString("channel",argnew),Argument::asDouble("__location__",argnew));
		}
	}
	void BassMusicManager::registerEvents() 
	{
		registerEvent("playMusic",&BassMusicManager::playEvent);
	}
	/*
	BassMusicManager
	*/
	void BassMusicManager::gameReset()
	{
		LOGGER->msg("Set","BassMusicManager");
	}
	void BassMusicManager::gameStop()
	{
		clear();
		init();
	}
	HSAMPLE BassMusicManager::loadSound(const std::string &file, bool stream)
	{
		HSAMPLE hSound;

		if(!SoraFileUtility::FileExists(file)) {            
			uint32 size;
			void* data = SoraCore::Ptr->getResourceFile(file, size);
			if(data) {
				if(stream)
					hSound = ::BASS_StreamCreateFile(TRUE,data,0,size,0);
				else
					hSound = ::BASS_SampleLoad(TRUE,data,0,size,MAX_SAMPLE,BASS_SAMPLE_OVER_POS);
				SoraCore::Ptr->freeResourceFile(data);
			} else
				DIVA_EXCEPTION_MODULE("Creating "+file+": File not existed","BassMusicManager");
		} else {
			if(stream)
				hSound = ::BASS_StreamCreateFile(FALSE,file.c_str(),0,0,0);
			else
				hSound = ::BASS_SampleLoad(FALSE,file.c_str(),0,0,MAX_SAMPLE,BASS_SAMPLE_OVER_POS);
		}
		if(hSound==0)
			DIVA_EXCEPTION_MODULE("Creating "+file+": failed","BassMusicManager");
		return hSound;
	}

	HSTREAM BassMusicManager::getSound(const std::string &ID)
	{
		if(soundPool.find(ID)==soundPool.end())
			DIVA_EXCEPTION_MODULE("Sound "+ID+" not exists","BassMusicManager");
		SOURCEPAIR pair = soundPool[ID];
		if(pair.first)
			return soundPool[ID].second;
		else
			return ::BASS_SampleGetChannel(soundPool[ID].second,false);
	}
	HSTREAM BassMusicManager::getChannel(const std::string &channel)
	{
		MUSICPOOL::iterator ptr = musicPool.find(channel);
		if(ptr==musicPool.end())
			DIVA_EXCEPTION_MODULE("Channel: "+channel+" is not existed","BassMusicManager");
		return ptr->second.second;
	}
	void BassMusicManager::init()
	{
		::BASS_Init(-1, 44100, 0, 0, 0);
		/*globalSample = ::BASS_SampleCreate(100000000, 28160, 1, 1, BASS_SAMPLE_LOOP|BASS_SAMPLE_OVER_POS);
		globalChannel = BASS_SampleGetChannel(globalSample,false);
		QWORD length = BASS_ChannelSeconds2Bytes(globalChannel,120);
		double time = BASS_ChannelBytes2Seconds(globalChannel,length);
		::BASS_StreamFree(globalSample);
		globalSample = ::BASS_SampleCreate(length, 28160, 1, 1, BASS_SAMPLE_LOOP|BASS_SAMPLE_OVER_POS);
		globalChannel = BASS_SampleGetChannel(globalSample,false);
		::BASS_ChannelPlay(globalChannel,true);*/
	}
	void BassMusicManager::destroy()
	{
		clear();
		::BASS_Free();
	}
	void BassMusicManager::clear()
	{
		//for(MUSICPOOL::iterator ptr = musicPool.begin(); ptr != musicPool.end(); ptr++)
		::BASS_Free();
		soundPool.clear();
		musicPool.clear();
		volumePool.clear();
		tagsVolume.clear();
		musicList.clear();
	}
	void BassMusicManager::stop()
	{
		//BASS_SampleStop(soundPool["res"].second);
		for(MUSICPOOL::iterator ptr = musicPool.begin(); ptr != musicPool.end(); ptr++)
			BASS_ChannelStop(ptr->second.second);
		//BASS_Pause();
		//BASS_Stop(); /*BASS_Start();*/
	}
	void BassMusicManager::load(const std::string &file, const std::string &ID, bool stream)
	{
		if(soundPool.find(ID)!=soundPool.end())
			DIVA_EXCEPTION_MODULE("Sound "+ID+" already exists","BassMusicManager");

		soundPool[ID] = std::make_pair<bool,HSTREAM>(stream,loadSound(file,stream));
	}
	void BassMusicManager::unload(const std::string &ID) 
	{
		SOUNDPOOL::iterator soundPtr = soundPool.find(ID);
		if(soundPtr==soundPool.end())
			return;

		if(soundPtr->second.first)
			::BASS_StreamFree(soundPtr->second.second);
		else
			::BASS_SampleFree(soundPtr->second.second);
		soundPool.erase(soundPtr);
	}
	void BassMusicManager::play(const std::string &ID, const std::string &channel, const std::string &tag)
	{
		if(HOOK_MANAGER_PTR->hook(ID,channel,tag)&&HOOK_MANAGER_PTR->hookInfo())
			return;
		
		HSTREAM hSound = getSound(ID);
		HSTREAM hChannel;
		if(musicPool.find(channel)!=musicPool.end())
		{
			hChannel = musicPool[channel].second;
			::BASS_ChannelStop(hChannel);
		}
		else
			volumePool[channel] = 1.0;
		hChannel = hSound;
		if(hSound) {
			::BASS_ChannelPlay(hChannel,true);
			::BASS_ChannelSetAttribute(hChannel,BASS_ATTRIB_VOL,getTagVolume(tag)*volumePool[channel]);
			::BASS_ChannelUpdate(hChannel,0);
        }

		musicPool[channel] = std::make_pair<std::string,HSTREAM>(tag,hChannel);
	}
	void BassMusicManager::playDirect(const std::string &ID, const std::string &tag)
	{
		if(HOOK_MANAGER_PTR->hook(ID,"",tag)&&HOOK_MANAGER_PTR->hookInfo())
			return;

		HCHANNEL hSound = getSound(ID);
		if(hSound) {
            ::BASS_ChannelPlay(hSound,true);
			::BASS_ChannelSetAttribute(hSound,BASS_ATTRIB_VOL,getTagVolume(tag));
			::BASS_ChannelUpdate(hSound,0);
        }
		
	}
	void BassMusicManager::playDirectWithFile(const std::string &file, bool stream, const std::string &tag)
	{
		HCHANNEL hSound = loadSound(file,stream);
		if(!stream)
			hSound = ::BASS_SampleGetChannel(hSound,false);
		if(hSound) {
            ::BASS_ChannelPlay(hSound,true);
			::BASS_ChannelSetAttribute(hSound,BASS_ATTRIB_VOL,getTagVolume(tag));
			::BASS_ChannelUpdate(hSound,0);
        }
	}
	void BassMusicManager::setVolume(const std::string &channel, float volume)
	{
		if(musicPool.find(channel)==musicPool.end())
			DIVA_EXCEPTION_MODULE("Channel: "+channel+" is not existed","MusicManager");
		volumePool[channel] = volume;
		SOUNDPAIR pair = musicPool[channel];
		::BASS_ChannelSetAttribute(musicPool[channel].second,BASS_ATTRIB_VOL,getTagVolume(pair.first)*volume);
		::BASS_ChannelUpdate(musicPool[channel].second,0);
	}
	void BassMusicManager::setTagVolume(const std::string &tag, float volume)
	{
		tagsVolume[tag] = volume;
		for(MUSICPOOL::iterator ptr = musicPool.begin(); ptr != musicPool.end(); ptr++)
			if(ptr->second.first==tag)
			{
				::BASS_ChannelSetAttribute(ptr->second.second,BASS_ATTRIB_VOL,getVolume(ptr->first)*volume);
				::BASS_ChannelUpdate(ptr->second.second,0);
			}
	}
	float BassMusicManager::getVolume(const std::string &channel)
	{
		return volumePool[channel];
	}
	float BassMusicManager::getTagVolume(const std::string &tag)
	{
		if(tagsVolume.find(tag)==tagsVolume.end())
			return 1.0;
		else
			return tagsVolume[tag];
	}
	void BassMusicManager::setLoop(const std::string &channel, bool flag)
	{
		if(flag)
			 ::BASS_ChannelFlags(getChannel(channel), 0, BASS_SAMPLE_LOOP); 
		else
			BASS_ChannelFlags(getChannel(channel), BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP); 
	}
	bool BassMusicManager::isLoop(const std::string &channel)
	{
        return (BASS_ChannelFlags(getChannel(channel), 0, 0)&BASS_SAMPLE_LOOP)>0;
	}
	void BassMusicManager::pause()
	{
		for(MUSICPOOL::iterator ptr = musicPool.begin(); ptr != musicPool.end(); ptr++)
			BASS_ChannelPause(ptr->second.second);
		//BASS_Pause();
	}
	void BassMusicManager::resume()
	{
		for(MUSICPOOL::iterator ptr = musicPool.begin(); ptr != musicPool.end(); ptr++)
			BASS_ChannelPlay(ptr->second.second,false);
		//BASS_Start();
	}
	void BassMusicManager::setPosition(const std::string &channel,double time)
	{
		//8-19
		double totalTime = getLength(channel);
		//when posite at final, bass won't do anything, so deal it myself
		if(time>=totalTime)
		{
			//can't posite at totalTime
			BASS_ChannelSetPosition(getChannel(channel),BASS_ChannelSeconds2Bytes(getChannel(channel),totalTime-0.01),BASS_POS_BYTE);
			if((BASS_ChannelFlags(getChannel(channel), 0, 0)&BASS_SAMPLE_LOOP)<=0)
				::BASS_ChannelStop(getChannel(channel));
		}
		else
			BASS_ChannelSetPosition(getChannel(channel),BASS_ChannelSeconds2Bytes(getChannel(channel),time),BASS_POS_BYTE);
	}
	double BassMusicManager::getPosition(const std::string &channel) {
		try
		{
			HCHANNEL hChannel = getChannel(channel);
			return ::BASS_ChannelBytes2Seconds(hChannel,BASS_ChannelGetPosition(hChannel,BASS_POS_BYTE));
		}
		catch(...)
		{
			return 0;
		}
	}
}