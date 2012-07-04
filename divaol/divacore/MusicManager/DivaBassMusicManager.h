/*
 *  DivaFmodMusicManager.h
 *
 *  Created by Hyf042 on 1/19/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_BASS_MUSIC_MANAGER
#define DIVA_BASS_MUSIC_MANAGER

#include "bass/bass.h"
#include "Core/DivaMusicManager.h"

#ifdef OS_WIN32
#pragma comment(lib, "bass.lib")
#endif

namespace divacore
{
	using namespace sora;
	/*
	FmodMusicManager
	*/
	class BassMusicManager : public MusicManager
	{
		static const int MAX_SAMPLE = 32;

		typedef std::pair<std::string,HSTREAM> SOUNDPAIR;
		typedef std::pair<bool,HSAMPLE> SOURCEPAIR;
		typedef std::map<std::string,SOUNDPAIR> MUSICPOOL;
		typedef std::map<std::string,SOURCEPAIR> SOUNDPOOL;
		typedef std::vector<SOUNDPAIR> MUSICLIST; 
		SOUNDPOOL soundPool;
		MUSICPOOL musicPool;
		MUSICLIST musicList;
		std::map<std::string,float> volumePool;
		std::map<std::string, float> tagsVolume;
	private:
		HSTREAM loadSound(const std::string &file, bool stream);
		HSTREAM getSound(const std::string &ID);
		HSTREAM getChannel(const std::string &channel);
	
	    //event functions
		void playEvent(ARGUMENTS &arg);
	public:
		void registerEvents();
		
		void gameReset();
		void gameStop();

		void init();
		void update() {}
		void clear();
		void destroy();
		void load(const std::string &file, const std::string &ID, bool stream);
		void unload(const std::string &ID);
		void play(const std::string &ID, const std::string &channel, const std::string &tag="");
		void playDirect(const std::string &ID, const std::string &tag="");
		void playDirectWithFile(const std::string &file, bool stream, const std::string &tag="");
		void setVolume(const std::string &channel, float volume);
		void setTagVolume(const std::string &tag, float volume);
		float getVolume(const std::string &channel);
		float getTagVolume(const std::string &tag);
		void setLoop(const std::string &channel, bool flag);
		bool isLoop(const std::string &channel);
		void stop() {BASS_Stop(); BASS_Start();}
		void stop(const std::string &channel) {BASS_ChannelStop(getChannel(channel));}
		void pause(const std::string &channel) {BASS_ChannelPause(getChannel(channel));}
		void resume(const std::string &channel) {BASS_ChannelPlay(getChannel(channel),false);}
		void setPosition(const std::string &channel,double time);
		float getPosition(const std::string &channel) {return ::BASS_ChannelBytes2Seconds(getChannel(channel),BASS_ChannelGetPosition(getChannel(channel),BASS_POS_BYTE));}
		void pause();
		void resume();
	};
}

#endif