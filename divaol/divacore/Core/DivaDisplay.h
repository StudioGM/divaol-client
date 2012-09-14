/*
 *  DivaDisplay.h
 *
 *  Created by Hyf042 on 1/21/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_DISPLAY
#define DIVA_DISPLAY

#include "DivaCommon.h"
#include "DivaMapInfo.h"
#include "DivaEventManager.h"

namespace divacore
{
	using namespace sora;
	/*
	Display Component
	It's to display the image, video or other things while core game running
	*/
	class Display : public EventHandler
	{
	public:

		virtual void destory() {}
		virtual void clear() {}
		virtual void clearImage() {}
		virtual void clearVideo() {}
		virtual void loadImage(const std::string &file, const std::string &ID) = 0;
		virtual void unloadImage(const std::string &ID) = 0;
		virtual void loadVideo(const std::string &file, const std::string &ID) = 0;
		virtual void unloadVideo(const std::string &ID) = 0;
		virtual void displayImage(const std::string &ID, float x = 0, float y = 0, float width = 0, float height = 0, const std::string &label="") = 0;
		virtual void clearImage(const std::string &label) = 0;
		virtual void playVideo(const std::string &ID, float x = 0, float y = 0, float width = 0, float height = 0) = 0;
		virtual void render() {}
		virtual void update(float dt) {}
		virtual void pause() {}
		virtual void resume() {}

		virtual void setSpeedScale(float scale) {}
		virtual void setVideoTime(const std::string &ID, double time) {}
		virtual float getVideoTime(const std::string &ID) {return 0;}
		virtual bool getImageSize(const std::string &label, float &width, float &height) = 0;
		virtual bool getVideoSize(const std::string &ID, float &width, float &height) = 0;
	};

	typedef Display* DisplayPtr;
}

#endif