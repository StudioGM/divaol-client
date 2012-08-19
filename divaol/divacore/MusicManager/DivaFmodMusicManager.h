/*
 *  DivaFmodMusicManager.h
 *
 *  Created by Hyf042 on 1/19/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_FMOD_MUSIC_MANAGER
#define DIVA_FMOD_MUSIC_MANAGER

#include "fmod/fmod.hpp"
#include "fmod/fmod_errors.h"

#ifdef OS_WIN32
#pragma comment(lib, "fmodex_vc.lib")
#endif

#include "SoraSoundFile.h"
#include "Core/DivaMusicManager.h"

namespace divacore
{
	using namespace sora;
	/*
	FmodMusicManager
	*/
	class FmodMusicManager : public MusicManager
	{
		FMOD::System* pSystem;
		static const int maxchannel = 32;

		typedef std::pair<std::string,FMOD::Channel*> SOUNDPAIR;
		typedef std::map<std::string,SOUNDPAIR> MUSICPOOL;
		typedef std::map<std::string,FMOD::Sound*> SOUNDPOOL;
		typedef std::vector<SOUNDPAIR> MUSICLIST; 
		SOUNDPOOL soundPool;
		MUSICPOOL musicPool;
		MUSICLIST musicList;
		std::map<std::string,float> volumePool;
		std::map<std::string, float> tagsVolume;
	private:
		FMOD::Sound* loadSound(const std::string &file, bool stream);
		FMOD::Sound* getSound(const std::string &ID);
		inline FMOD::Channel* getChannel(const std::string &channel);
	public:
		FmodMusicManager():pSystem(NULL) {}
			 
		void init();
		void update() {pSystem->update();}
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
		bool isPlaying(const std::string &channel);
		void stop();
		void stop(const std::string &channel) {getChannel(channel)->stop();}
		void pause(const std::string &channel) {getChannel(channel)->setPaused(true);}
		void resume(const std::string &channel) {getChannel(channel)->setPaused(false);}
		void setPosition(const std::string &channel,double time);
		double getPosition(const std::string &channel);
		double getLength(const std::string &channel);

		void pause();
		void resume();
	};
}

#endif