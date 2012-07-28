/*
 *  DivaMusicManager.h
 *
 *  Created by Hyf042 on 1/19/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_MUSIC_MANAGER
#define DIVA_MUSIC_MANAGER

#include "DivaCommon.h"
#include "DivaEventManager.h"

namespace divacore
{
	/*
	MusicManager
	core music play
	*/
	class MusicManager : public EventHandler
	{
	public:
		virtual void init() {}
		virtual void update(){}
		virtual void clear(){}
		virtual void destroy(){}
		virtual void load(const std::string &file, const std::string &ID, bool stream) = 0;
		virtual void unload(const std::string &ID) {}
		virtual void play(const std::string &ID, const std::string &channel, const std::string &tag="") = 0;
		virtual void playDirect(const std::string &ID, const std::string &tag="") = 0;
		virtual void playDirectWithFile(const std::string &file, bool stream, const std::string &tag="") = 0;
		virtual void setVolume(const std::string &channel, float volume) = 0;
		virtual void setTagVolume(const std::string &tag, float volume) = 0;
		virtual float getVolume(const std::string &channel) = 0;
		virtual float getTagVolume(const std::string &tag) = 0;
		virtual void setLoop(const std::string &channel, bool flag) = 0;
		virtual bool isLoop(const std::string &channel) = 0;
		virtual bool isPlaying(const std::string &channel) {return false;}
		virtual void stop(const std::string &channel) = 0;
		virtual void stop() {clear();}
		virtual void pause(const std::string &channel)= 0;
		virtual void resume(const std::string &channel) = 0;
		virtual void setPosition(const std::string &channel,double time) {}
		virtual float getPosition(const std::string &channel) {return 0;}
		virtual double getLength(const std::string &channel) {return 0;}
		virtual void pause() = 0;
		virtual void resume() = 0;
	};

	typedef MusicManager* MusicManagerPtr;
}

#endif