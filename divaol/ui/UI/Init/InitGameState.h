/*
 *  InitGameState.h
 *
 *  Created by Hyf042 on 1/20/13.
 *  Copyright 2013 Hyf042. All rights reserved.
 *
 *  Init Game State
 */

#ifndef DIVA_INIT_GAME_STATE
#define DIVA_INIT_GAME_STATE

#include "SoraCore.h"
#include "SoraSprite.h"

#include "app/SoraGameApp.h"
#include "app/SoraGameState.h"
#include "ui/Config/DivaUIConfig.h"
#include "Utility/DivaSettings.h"
#include "SoraVlcMoviePlayer/SoraVlcMoviePlayer.h"

#include "Lib/Base/Base.h"
#include "Lib/WJson/wjson.h"

namespace diva
{
	class Splash
	{
	protected:
		int state;
		bool canJump;
	public:
		enum {UNINITIALIZED, INITIALIZED, DONE};

		Splash():state(UNINITIALIZED) {}
		virtual ~Splash() {}

		virtual void Start() {}
		virtual void Update(float dt) {}
		virtual void Render() {}
		virtual bool IsDone() {return true;}
		virtual int GetState() const {return state;}
		
		virtual void Jump() {
			if(canJump) 
			{
				state = DONE;
				//image->setColor(CSETA(image->getColor(), 0));
			}
		}
	};

	class ImageSplash : public Splash
	{
	public:
		enum {FADE_IN = 0x80, DETAIN, FADE_OUT};
	private:
		sora::SoraSprite *image;

		float fadeTime;
		float detainTime;
		float nowTime;
		float targetTime;
	public:
		ImageSplash():image(NULL) {}
		virtual ~ImageSplash() {Clear();}
		void Clear() 
		{
			state = UNINITIALIZED;
			if(image)
			{
				delete image;
				image = NULL;
			}
		}
		void setParameters(float fadeTime, float detainTime, bool canJump = false)
		{
			this->fadeTime = fadeTime;
			this->detainTime = detainTime;
			this->canJump = canJump;
		}
		bool Initialize(const Base::String &file)
		{
			Clear();
			image = sora::SoraCore::Ptr->createSprite(file.asUnicode());
			if(image == NULL)
				return false;
			
			image->setScale(float(setConfig[L"windowWidth"].asInt()) / image->getSpriteWidth(), float(setConfig[L"windowHeight"].asInt()) / image->getSpriteHeight());
			image->setColor(CSETA(image->getColor(),0));

			state = INITIALIZED;
			return true;
		}
		void Start() 
		{
			if(state == UNINITIALIZED)
				return;
			state = FADE_IN;
			nowTime = 0;
			targetTime = fadeTime;
		}
		void Update(float dt)
		{
			if(state == UNINITIALIZED || state == INITIALIZED)
				return;

			nowTime += dt;
			if(nowTime >= targetTime)
			{
				nowTime = 0;
				if(state == FADE_IN)
					state = DETAIN, targetTime = detainTime;
				else if(state == DETAIN)
					state = FADE_OUT, targetTime = fadeTime;
				else
					state = DONE, targetTime = 0;
			}
			if(state != DONE)
			{
				float ratio = nowTime / targetTime;
				ratio = Base::Math::Util::Clamp(ratio, 0.0f, 1.0f);
				if(state == FADE_IN)
					image->setColor(CSETA(image->getColor(), int(ratio*255)));
				else if(state == FADE_OUT)
					image->setColor(CSETA(image->getColor(), int(255-ratio*255)));
				else
					image->setColor(CSETA(image->getColor(), 255));
			}
		}
		void Render()
		{
			if(state == UNINITIALIZED || state == INITIALIZED || state == DONE)
				return;
			image->render();
		}
	};

	class VideoSplash : public Splash
	{
		sora::SoraSprite *image;
		SoraVlcMoviePlayer *video;
		float lastTime;
		float nowTime;
		Base::String videoFile;
		Base::Thread<void()> loadTask;
	public:
		enum {DISPLAY_LOADING_IMAGE = 0x80, DISPLAY_LOADING_OVER, LOADING_VIDEO, PLAYING_VIDEO};
	public:
		VideoSplash():image(NULL),video(NULL) {}
		virtual ~VideoSplash() {Clear();}
		void Clear() 
		{
			state = UNINITIALIZED;
			if(image)
			{
				delete image;
				image = NULL;
			}
			if(video)
			{
				delete video;
				video = NULL;
			}
		}

		void Start() 
		{
			if(state == UNINITIALIZED)
				return;
			state = DISPLAY_LOADING_IMAGE;
			nowTime = 0;
		}
		void setParameters(float lastTime, bool canJump)
		{
			this->lastTime = lastTime;
			this->canJump = canJump;
		}
		bool Initialize(const Base::String &video_file, const Base::String &loading_file)
		{
			image = sora::SoraCore::Ptr->createSprite(loading_file.asUnicode());
			if (image)
				image->setScale(float(setConfig[L"windowWidth"].asInt()) / image->getSpriteWidth(), float(setConfig[L"windowHeight"].asInt()) / image->getSpriteHeight());
			
			video = new sora::SoraVlcMoviePlayer();

			loadTask.set(Base::MakeFunction(&VideoSplash::_LoadVideo, this));

			this->videoFile = video_file;

			state = INITIALIZED;
			return true;
		}

		void _LoadVideo()
		{
			video->openMedia(videoFile.asUnicode());
		}

		void Update(float dt)
		{
			if(state == UNINITIALIZED || state == INITIALIZED)
				return;
			
			if(state == DISPLAY_LOADING_IMAGE)
			{
				state = DISPLAY_LOADING_OVER;
			}
			else if(state == DISPLAY_LOADING_OVER)
			{
				state = LOADING_VIDEO;
				_LoadVideo();
			}
			else if(state == LOADING_VIDEO)
			{
				if(!loadTask.active())
				{
					nowTime = 0;
					state = PLAYING_VIDEO;
					if(nowTime >= lastTime)
						state = DONE;
					else
					{
						if(image)
							delete image;
						sora::SoraSprite *image = new sora::SoraSprite();
						sora::SoraTextureHandle texture = sora::SoraTexture::CreateEmpty(video->getWidth(), video->getHeight());
						video->bindTexture(texture);
					}
				}
			}
			else if(state == PLAYING_VIDEO)
			{
				nowTime += dt;
				if(nowTime >= lastTime)
					state = DONE;
			}
		}

		void Render()
		{
			if(state == DISPLAY_LOADING_IMAGE || state == DISPLAY_LOADING_OVER || state == LOADING_VIDEO || state == PLAYING_VIDEO)
				if(image)
					image->render();
		}

		void Jump() {
			if(canJump && state == PLAYING_VIDEO) 
				state = DONE;
		}
	};

	class InitGameState: public sora::SoraGameState, public sora::SoraEventHandler {
		enum State{UNINITIALIZED, INITIALIEZD, SPLASH,DONE,LEFT};
	public:
		InitGameState():state(UNINITIALIZED) {
		}

		void onRender() {
			sora::SoraGameApp::BeginScene();

			if(state == SPLASH)
				nowSplash->Render();

			sora::SoraGameApp::EndScene();
		}

		void onUpdate(float dt) {   
			if(state == SPLASH)
			{
				nowSplash->Update(dt);
				if(nowSplash->GetState() == Splash::DONE)
				{
					if((++nowSplashCnt) == splashList.size())
						state = DONE;
					else
					{
						nowSplash = splashList[nowSplashCnt];
						nowSplash->Start();
					}
				}
			}
			else if(state == DONE)
			{
				getGameApp()->setState(nextState);
				state = LEFT;
			}
		}

		void onEnter() {
			if(state == UNINITIALIZED)
			{
				_ReadConfig("config/init.json");
				// init guichan (must before SoraKeyPoll, or we can not get the queue of event)
				sora::GCN_GLOBAL->initGUIChan(SETTINGS.getGlobalFontName().unicode_str(), 20);
			}
			
			state = INITIALIEZD;

			if(splashList.size() > 0)
			{
				nowSplashCnt = 0;
				nowSplash = splashList[nowSplashCnt];
				nowSplash->Start();

				state = SPLASH;
			}
			else
				state = DONE;
		}

		void onKeyPressed(sora::SoraKeyEvent& keyEvent) {
			if(state == SPLASH)
				nowSplash->Jump();
		}

		void onMouseClicked(SoraMouseEvent& event) {
			if(state == SPLASH)
				nowSplash->Jump();
		}
	private:
		void _ReadConfig(const Base::String &filename)
		{
			splashList.clear();

			WJson::Reader reader;
			WJson::Value config;
			reader.parse(ReadJsonFile(filename), config);
			if(config.isMember(L"splash"))
			{
				WJson::Value splashConf = config[L"splash"];
				for(WJson::Value::iterator ptr = splashConf.begin(); ptr != splashConf.end(); ptr++)
				{
					WJson::Value item = *ptr;
					if(!item.isMember(L"type") || item[L"type"].asString() == L"image")
					{
						Base::SharedPtr<ImageSplash> splash = new ImageSplash();
						splash->Initialize(item[L"texture"].asString());
						splash->setParameters(item[L"fadeTime"].asDouble(), item[L"detainTime"].asDouble(), item[L"canJump"].asBool());
						splashList.push_back(splash);
					}
					else if(item[L"type"].asString() == L"video")
					{
						Base::SharedPtr<VideoSplash> splash = new VideoSplash();
						splash->Initialize(item[L"video"].asString(), item[L"loading"].asString());
						splash->setParameters(item[L"lastTime"].asDouble(), item[L"canJump"].asBool());
						splashList.push_back(splash);
					}
				}
			}
			nextState = config[L"nextState"].asString();
		}
	private:
		int state;
		int nowSplashCnt;
		std::vector<Base::SharedPtr<Splash>> splashList;
		Base::SharedPtr<Splash> nowSplash;
		
		Base::String nextState;
	};
}
#endif