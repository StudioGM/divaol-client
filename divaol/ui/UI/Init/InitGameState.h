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

#include "Lib/Base/Base.h"
#include "Lib/WJson/wjson.h"

namespace diva
{
	class Splash
	{
	public:
		enum {UNINITIALIZED, INITIALIZED, FADE_IN, DETAIN, FADE_OUT, DONE};
	private:
		sora::SoraSprite *image;

		float fadeTime;
		float detainTime;
		float nowTime;
		float targetTime;
		bool canJump;
		
		int state;
	public:
		Splash():image(NULL),state(UNINITIALIZED) {}
		~Splash() {Clear();}
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
		int GetState() const {return state;}
		void Jump() {
			if(canJump) 
			{
				state = DONE;
				//image->setColor(CSETA(image->getColor(), 0));
			}
		}
	};
	class InitGameState: public sora::SoraGameState, public sora::SoraEventHandler {
		enum State{SPLASH,DONE,LEFT};
	public:
		InitGameState() {
		}

		~InitGameState()
		{
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
			_ReadConfig("uiconfig/init.json");
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
					Base::SharedPtr<Splash> splash = new Splash();
					splash->Initialize(item[L"texture"].asString());
					splash->setParameters(item[L"fadeTime"].asDouble(), item[L"detainTime"].asDouble(), item[L"canJump"].asBool());
					splashList.push_back(splash);
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