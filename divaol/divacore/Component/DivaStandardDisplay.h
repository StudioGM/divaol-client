/*
 *  DivaStandardDisplay.h
 *
 *  Created by Hyf042 on 1/21/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_STANDARD_DISPLAY
#define DIVA_STANDARD_DISPLAY

#include "Core/DivaDisplay.h"
#include "SoraSprite.h"
#include "SoraVlcMoviePlayer/SoraVlcMoviePlayer.h"

namespace divacore
{
	class DisplayNode
	{
	public:
		enum{IMAGE,VIDEO};
		std::string mLabel;
		double z;
		int mType;
		sora::SoraSprite *mSprite;
		DisplayNode():mLabel(""),z(0),mType(IMAGE),mSprite(NULL) {}
		DisplayNode(int type, sora::SoraSprite *sprite):mLabel(""),z(0),mType(type),mSprite(sprite) {}

		inline int type() const {return mType;}
		inline virtual void render() {mSprite->render();}
	};
	typedef std::vector<DisplayNode> DISPLAY_LIST;

	class StandardDisplay : public Display
	{
		typedef std::map<std::string,sora::SoraTextureHandle> IMAGEPOOL;
		typedef std::pair<sora::SoraVlcMoviePlayer*,sora::SoraSprite*> VIDEOPAIR;
		typedef std::map<std::string,VIDEOPAIR> VIDEOPOOL;
		typedef std::map<std::string,sora::SoraSprite*> SPRITEPOOL;
		typedef std::vector<sora::SoraSprite*> SPRITELIST;
		typedef std::set<std::string> VIDEOPLAYING;

		IMAGEPOOL imagePool;
		VIDEOPOOL videoPool;
		SPRITEPOOL spritePool;
		SPRITELIST spriteList;
		VIDEOPLAYING videoPlaying;
		DISPLAY_LIST displayList;
	private:
		void playVideo(ARGUMENTS &arg);
		void displayImage(ARGUMENTS &arg);
	public:
		void registerEvents();

	public:
		bool isExistImage(const std::string &ID) {return imagePool.find(ID)!=imagePool.end();}
		bool isExistVideo(const std::string &ID) {return videoPool.find(ID)!=videoPool.end();}

		void gameReset();
		void gameStop();

		void clear();
		void clearImage();
		void clearVideo();
		void destory() {clear();}
		void loadImage(const std::string &file, const std::string &ID);
		void loadVideo(const std::string &file, const std::string &ID);
		void unloadImage(const std::string &ID);
		void unloadVideo(const std::string &ID);
		void displayImage(const std::string &ID, float x = 0, float y = 0, float width = 0, float height = 0, const std::string &label="");
		bool getImageSize(const std::string &label, float &width, float &height);
		bool getVideoSize(const std::string &ID, float &width, float &height);
		void setTextureRect(const std::string &label, float x, float y, float w, float h);
		void clearImage(const std::string &label);
		void playVideo(const std::string &ID, float x = 0, float y = 0, float width = 0, float height = 0);
		void pause();
		void resume();
		void render();
		void update(float dt);

		void setSpeedScale(float scale);
		void setVideoTime(const std::string &ID, double time);
		float getVideoTime(const std::string &ID);
	};
}

#endif