/*
 *  DivaStandardDisplay.cpp
 *
 *  Created by Hyf042 on 1/21/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */
#include "DivaStandardDisplay.h"
#include "SoraCore.h"
#include "SoraTextureMap.h"

namespace divacore
{

	void StandardDisplay::playVideo(ARGUMENTS &arg)
	{
		ARGUMENTS argnew = arg;
		Argument::addDefaultArg("x",0.0,argnew);
		Argument::addDefaultArg("y",0.0,argnew);
		Argument::addDefaultArg("width",0.0,argnew);
		Argument::addDefaultArg("height",0.0,argnew);

		if(Argument::isArg("__location__",argnew))
			setVideoTime(Argument::asString("id",argnew),Argument::asDouble("__location__",argnew)+0.2);
		else
			setVideoTime(Argument::asString("id",argnew),0);

		playVideo(Argument::asString("id",argnew),Argument::asFloat("x",argnew),Argument::asFloat("y",argnew),Argument::asFloat("width",argnew),Argument::asFloat("height",argnew));
     }
	void StandardDisplay::displayImage(ARGUMENTS &arg)
	{
		ARGUMENTS argnew = arg;
		Argument::addDefaultArg("x",0.0,argnew);
		Argument::addDefaultArg("y",0.0,argnew);
		Argument::addDefaultArg("width",0.0,argnew);
		Argument::addDefaultArg("height",0.0,argnew);
		Argument::addDefaultArg("tex_x",0.0,argnew);
		Argument::addDefaultArg("tex_y",0.0,argnew);
		Argument::addDefaultArg("label","",argnew);
		//displayImage(Argument::asString("id",argnew),Argument::asFloat("x",argnew),Argument::asFloat("y",argnew),Argument::asFloat("width",argnew),Argument::asFloat("height",argnew),Argument::asString("label",argnew));
	
		std::string ID = Argument::asString("id",argnew);
		std::string label = Argument::asString("label",argnew);
		if(!isExistImage(ID))
			DIVA_EXCEPTION_MODULE("Image "+ID+" does not exists","DivaDisplay");
		SoraSprite *sprite;
		if(label==""||spritePool.find(label)==spritePool.end())
			sprite = new sora::SoraSprite();
		else
			sprite = spritePool[label];

		sora::SoraTextureMap::Instance()->addRf(imagePool[ID]);

		sprite->setTexture(imagePool[ID]);
		sprite->setPosition(Argument::asFloat("x",argnew),Argument::asFloat("y",argnew));
		if(Argument::isArg("tex_width",argnew)&&Argument::isArg("tex_height",argnew))
			sprite->setTextureRect(Argument::asFloat("tex_x",argnew),Argument::asFloat("tex_y",argnew),Argument::asFloat("tex_width",argnew),Argument::asFloat("tex_height",argnew));
		sprite->setScale((Argument::asFloat("width",argnew)>0)?(Argument::asFloat("width",argnew)/sprite->getSpriteWidth()):1.0,
			(Argument::asFloat("height",argnew)>0)?(Argument::asFloat("height",argnew)/sprite->getSpriteHeight()):1.0);

		if(label=="")
			spriteList.push_back(sprite);
		else
			spritePool[label] = sprite;
	}
	void StandardDisplay::registerEvents() 
	{
		registerEvent("playVideo",&StandardDisplay::playVideo);
		registerEvent("displayImage",&StandardDisplay::displayImage);
	}

	void StandardDisplay::gameReset()
	{
		LOGGER->msg("Set","StandardDisplay");
	}
	void StandardDisplay::gameStop()
	{
		clear();
	}
	void StandardDisplay::clearImage()
	{
		for(SPRITELIST::iterator ptr = spriteList.begin(); ptr != spriteList.end(); ptr++)
			SAFE_DELETE_SPRITE((*ptr));
		spriteList.clear();
	}
	void StandardDisplay::clearVideo()
	{
		for(VIDEOPOOL::iterator ptr = videoPool.begin(); ptr != videoPool.end(); ptr++)
		{
			if(ptr->second.first->isStopped())
			{
				//vlc can not replay before stopped
				ptr->second.first->openMedia();
				ptr->second.first->play();
				while(ptr->second.first->isPlaying())
					msleep(1),ptr->second.first->pause();
			}
			//ptr->second.first->setTime(0);
			ptr->second.first->pause();
		}
		videoPlaying.clear();
	}

	void StandardDisplay::clear()
	{
		for(SPRITELIST::iterator ptr = spriteList.begin(); ptr != spriteList.end(); ptr++)
			SAFE_DELETE_SPRITE((*ptr));
		for(SPRITEPOOL::iterator ptr = spritePool.begin(); ptr != spritePool.end(); ptr++)
			SAFE_DELETE_SPRITE(ptr->second);
		for(VIDEOPOOL::iterator ptr = videoPool.begin(); ptr != videoPool.end(); ptr++)
		{
			ptr->second.first->stop();
			SAFE_DELETE(ptr->second.first);
			SAFE_DELETE_SPRITE_TEXTURE(ptr->second.second);
		}
		for(IMAGEPOOL::iterator ptr = imagePool.begin(); ptr != imagePool.end(); ptr++)
			sora::SoraCore::Ptr->releaseTexture(ptr->second);
		imagePool.clear();
		videoPool.clear();
		spritePool.clear();
		spriteList.clear();
		videoPlaying.clear();
	}
	void StandardDisplay::loadImage(const std::string &file, const std::string &ID)
	{
		if(isExistImage(ID))
			DIVA_EXCEPTION_MODULE("Image "+ID+" alrealdy exists","DivaDisplay");
		imagePool[ID] = sora::SoraCore::Ptr->createTexture(file);
	}
	void StandardDisplay::unloadImage(const std::string &ID)
	{
		if(!isExistImage(ID))
			DIVA_EXCEPTION_MODULE("Image "+ID+" doesn't exist","DivaDisplay");

		sora::SoraTextureHandle texture = imagePool[ID];
		for(SPRITELIST::iterator ptr = spriteList.begin(); ptr != spriteList.end(); ptr++)
			if((*ptr)->getTexture()==texture)
				(*ptr)->setTexture(NULL);
		for(SPRITEPOOL::iterator ptr = spritePool.begin(); ptr != spritePool.end(); ptr++)
			if(ptr->second->getTexture()==texture)
				ptr->second->setTexture(NULL);

		sora::SoraCore::Instance()->releaseTexture(texture);

		imagePool.erase(imagePool.find(ID));
	}
	void StandardDisplay::loadVideo(const std::string &file, const std::string &ID)
	{
		if(isExistVideo(ID))
			DIVA_EXCEPTION_MODULE("Video "+ID+" alrealdy exists","DivaDisplay");
		sora::SoraVlcMoviePlayer *moviePlayer = new sora::SoraVlcMoviePlayer(true);
        if(!moviePlayer->openMedia(file))
			DIVA_EXCEPTION_MODULE("Video "+file+" loading failed","DivaDisplay");
		sora::SoraSprite *sprite = new sora::SoraSprite();
		sora::SoraTextureHandle texture = sora::SoraTexture::CreateEmpty(moviePlayer->getWidth(), moviePlayer->getHeight());
		moviePlayer->bindTexture(texture);
		sprite->setTexture(texture);
		
        moviePlayer->play();
		while(moviePlayer->isPlaying())
			msleep(1),moviePlayer->pause();
		moviePlayer->setTime(0);
		
		videoPool[ID] = std::make_pair<sora::SoraVlcMoviePlayer*,sora::SoraSprite*>(moviePlayer,sprite);
	}
	void StandardDisplay::unloadVideo(const std::string &ID)
	{
		if(!isExistVideo(ID))
			DIVA_EXCEPTION_MODULE("Video "+ID+" doesn't exists","DivaDisplay");

		VIDEOPOOL::iterator ptr = videoPool.find(ID);
		ptr->second.first->stop();
		SAFE_DELETE(ptr->second.first);
		SAFE_DELETE_SPRITE_TEXTURE(ptr->second.second);
		videoPool.erase(ptr);

		if(videoPlaying.find(ID)!=videoPlaying.end())
			videoPlaying.erase(videoPlaying.find(ID));
	}
	void StandardDisplay::displayImage(const std::string &ID, float x, float y, float width, float height, const std::string &label)
	{
		if(!isExistImage(ID))
		{
			LOGGER->notice("Image "+ID+" does not exists");
			return;
			//DIVA_EXCEPTION_MODULE("Image "+ID+" does not exists","DivaDisplay");
		}
		SoraSprite *sprite;
		if(label==""||spritePool.find(label)==spritePool.end())
			sprite = new sora::SoraSprite();
		else
			sprite = spritePool[ID];
		sprite->setTexture(imagePool[ID]);
		sprite->setPosition(x,y);
		sprite->setScale((width>0)?(width/sprite->getSpriteWidth()):1.0,(height>0)?(height/sprite->getSpriteHeight()):1.0);
		
		if(label=="")
			spriteList.push_back(sprite);
		else
			spritePool[label] = sprite;
	}
	bool StandardDisplay::getImageSize(const std::string &label, float &width, float &height)
	{
		if(spritePool.find(label)==spritePool.end())
			return false;

		SoraSprite *sprite = spritePool[label];
		width = sprite->getSpriteWidth()*sprite->getHScale();
		height = sprite->getSpriteHeight()*sprite->getVScale();
		return true;
	}
	bool StandardDisplay::getVideoSize(const std::string &ID, float &width, float &height)
	{
		if(!isExistVideo(ID))
			return false;
		VIDEOPAIR pair = videoPool[ID];
		width = pair.second->getSpriteWidth()*pair.second->getHScale();
		height = pair.second->getSpriteHeight()*pair.second->getVScale();
		return true;
	}
	void StandardDisplay::setTextureRect(const std::string &label, float x, float y, float w, float h)
	{
		if(spritePool.find(label)==spritePool.end())
			return;

		SoraSprite *sprite = spritePool[label];
		sprite->setTextureRect(x,y,w,h);
	}
	void StandardDisplay::clearImage(const std::string &label)
	{
		if(spritePool.find(label)==spritePool.end())
			return;
		SAFE_DELETE(spritePool[label]);
		spritePool.erase(spritePool.find(label));
	}
	void StandardDisplay::playVideo(const std::string &ID, float x, float y, float width, float height)
	{
		if(!isExistVideo(ID))
		{
			LOGGER->notice("Video "+ID+" does not exists");
			return;
			//DIVA_EXCEPTION_MODULE("Video "+ID+" does not exists","DivaDisplay");
		}
		VIDEOPAIR pair = videoPool[ID];
		pair.first->resume();
		pair.second->setPosition(x,y);
		pair.second->setScale((width>0)?(width/pair.second->getSpriteWidth()):1.0,(height>0)?(height/pair.second->getSpriteHeight()):1.0);
		videoPlaying.insert(ID);
	}

	void StandardDisplay::render()
	{
		for(VIDEOPLAYING::iterator ptr = videoPlaying.begin(); ptr != videoPlaying.end(); ptr++)
			videoPool[*ptr].second->render();
		for(SPRITEPOOL::iterator ptr = spritePool.begin(); ptr != spritePool.end(); ptr++)
			ptr->second->render();
		for(SPRITELIST::iterator ptr = spriteList.begin(); ptr != spriteList.end(); ptr++)
			(*ptr)->render();
	}

	void StandardDisplay::update(float dt)
	{
		for(SPRITEPOOL::iterator ptr = spritePool.begin(); ptr != spritePool.end(); ptr++)
			ptr->second->update(dt);
		for(SPRITELIST::iterator ptr = spriteList.begin(); ptr != spriteList.end(); ptr++)
			(*ptr)->update(dt);

		for(VIDEOPLAYING::iterator ptr = videoPlaying.begin(); ptr != videoPlaying.end(); ptr++)
			if(videoPool[*ptr].first->isStopped())
			{
				videoPlaying.erase(*ptr);
				break;
			}
	}
	void StandardDisplay::pause()
	{
		for(VIDEOPLAYING::iterator ptr = videoPlaying.begin(); ptr != videoPlaying.end(); ptr++)
			videoPool[*ptr].first->pause();
	}

	void StandardDisplay::resume()
	{
		for(VIDEOPLAYING::iterator ptr = videoPlaying.begin(); ptr != videoPlaying.end(); ptr++)
			videoPool[*ptr].first->resume();
	}

	void StandardDisplay::setVideoTime(const std::string &ID, double time)
	{
		if(!isExistVideo(ID))
			DIVA_EXCEPTION_MODULE("Video "+ID+" does not exists","DivaDisplay");
		VIDEOPAIR pair = videoPool[ID];
		uint64 length = pair.first->getLength();

		pair.first->setTime(time*1000);
	}

	float StandardDisplay::getVideoTime(const std::string &ID)
	{
		return videoPool[ID].first->getTime()/1000.0;
	}
}