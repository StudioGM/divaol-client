/*
 *  DivaStandardRenderSystem.h
 *
 *  Created by Hyf042 on 1/27/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_STANDARD_RENDER_SYSTEM
#define DIVA_STANDARD_RENDER_SYSTEM

#include "Core/DivaRenderSystem.h"
#include "Core/DivaConfig.h"
#include "Utility/DivaConfigLoader.h"
#include "thread/SoraMutex.h"
#include "SoraShader.h"

namespace divacore
{
	class StandardRenderSystem : public RenderSystem
	{
		//canvas
		sora::SoraBaseCanvas *coreCanvas;
		sora::SoraBaseCanvas *innerCanvas;

		sora::SoraSprite *preview,*white;
		//info
		int gameWidth,gameHeight,windowWidth,windowHeight;
		Config config;

		sora::SoraMutex mutex;
		bool bFade;
		float fadeTime;
	public:
		void prepare(Config &config)
		{
			//gameWidth = config.getAsInt("gameWidth");
			//gameHeight = config.getAsInt("gameHeight");
			windowWidth = windowHeight = 0;
			//windowWidth = config.getAsInt("windowWidth");
			//windowHeight = config.getAsInt("windowHeight");

			fadeTime = config.getAsDouble("fadeTime");
		}
		sora::SoraBaseCanvas *getInnerCanvas() {return innerCanvas;}
		sora::SoraBaseCanvas *getCoreCanvas() {return coreCanvas;}
		void init()
		{
			coreCanvas = innerCanvas = NULL;
			preview = white = NULL;
			bFade = false;
		}
		void gameLoadFromConfig(Config &config)
		{
			sora::SoraMutexGuard lock(mutex);

			this->config = config;

			// remove game width&height from render config, to ensure the resolutions of UI & core are the same.
			//gameWidth = config.getAsInt("gameWidth");
			//gameHeight = config.getAsInt("gameHeight");

			SAFE_DELETE(coreCanvas);
			SAFE_DELETE(innerCanvas);
			coreCanvas = new sora::SoraBaseCanvas(gameWidth,gameHeight);
			innerCanvas = new sora::SoraBaseCanvas(gameWidth,gameHeight);
			
			if(preview->getTexture())
				preview->setScale(double(gameWidth)/preview->getSpriteWidth(),
				double(gameHeight)/preview->getSpriteHeight());
			white->setScale(double(gameWidth)/white->getSpriteWidth(),
				double(gameHeight)/white->getSpriteHeight());
		}
		void gameLoad(const std::string &configFile)
		{
			sora::SoraMutexGuard lock(mutex);

			configloader::loadWithJson(config,configFile);

			gameLoadFromConfig(config);
		}
		bool InsideDrawRange(const Point &p)
		{
			if(p.x<0||p.y<0||p.x>gameWidth||p.y>gameHeight)
				return false;
			return true;
		}
		void render(sora::SoraSprite *sprite, const std::string &tag = "") 
		{
			if(!HOOK_MANAGER_PTR->hook(sprite->getPositionX(),sprite->getPositionY(),sprite,tag)||!HOOK_MANAGER_PTR->hookInfo())
				sprite->render();
		}
		void render(float x, float y, sora::SoraSprite *sprite, const std::string &tag = "") 
		{
			if(x<0||y<0||x>gameWidth||y>gameHeight)
				return;
			if(!HOOK_MANAGER_PTR->hook(x,y,sprite,tag)||!HOOK_MANAGER_PTR->hookInfo())
				sprite->render(x,y);
		}
		sora::SoraSprite* renderToCanvas(float width = 0, float height = 0, uint32 mask = RS_RENDER_ALL)
		{
			sora::SoraMutexGuard lock(mutex);
			draw(mask);
			if(coreCanvas==NULL)
				return NULL;
			SoraSprite *canvas = coreCanvas->getCanvasSprite();
			//if(canvas->getFragmentShader())
			//	canvas->getFragmentShader()->setTexture("test",coreCanvas->getCanvasSprite()->getTexture());

#ifdef OS_WIN32
			canvas->setScale(width==0?1:(width/double(canvas->getTextureWidth(false))),
				height==0?1:(height/double(canvas->getTextureHeight(false))));
		
#else
            
#endif // OS_WIN32
			return canvas;
		}
		void draw(uint32 mask = RS_RENDER_ALL)
		{
		//	sora::SoraMutexGuard lock(mutex);

			if(!coreCanvas)
				return;

			//render canvas

			if(CORE_PTR->getState()==Core::PREPARE)
			{
				coreCanvas->beginRender();
				if(preview->getTexture())
					preview->render();
			}
			else if(CORE_PTR->getState()==Core::RUN)
			{
//				sora::SoraMutexGuard lock(mutex);

				innerCanvas->beginRender();

				if(mask&RS_RENDER_BACKGROUND)
					DISPLAY_PTR->render();
				if(mask&RS_RENDER_NOTE)
				{
					CORE_FLOW_PTR->render();
					HOOK_MANAGER_PTR->render();
				}
				innerCanvas->finishRender();

				coreCanvas->beginRender();

				innerCanvas->render();
				if(mask&RS_RENDER_UI)
				{
					UI_PAINTER_PTR->render();
					GAME_MODE_PTR->render();
				}
				if((mask&RS_RENDER_EFFECT)&&EFFECT_SYSTEM_PTR)
					EFFECT_SYSTEM_PTR->render();
			}
			else if(CORE_PTR->getState()==Core::RESULT)
			{
				coreCanvas->beginRender();

				if(preview && preview->getTexture())
				{
					//preview->setScale(double(gameWidth)/preview->getSpriteWidth(),
					//	double(gameHeight)/preview->getSpriteHeight());
					preview->render();
				}
				
				UI_PAINTER_PTR->render();
			}

			if(bFade)
				white->render();

			coreCanvas->finishRender();
		}

		void update(float dt)
		{
			if(white&&bFade)
			{
				white->update(dt);
				if(!white->hasEffect())
					bFade = false;
			}
		}

		void render()
		{
			if(coreCanvas==NULL)
				return;

			if(CORE_PTR->getState()==Core::PREPARE||CORE_PTR->getState()==Core::RUN||CORE_PTR->getState()==Core::RESULT)
			{
				sora::SoraMutexGuard lock(mutex);

				SoraSprite *canvasSprite = coreCanvas->getCanvasSprite();       
                            
				canvasSprite->setScale(sora::SoraCore::Ptr->getScreenWidth()/double(canvasSprite->getTextureWidth(false)),
					sora::SoraCore::Ptr->getScreenHeight()/double(canvasSprite->getTextureHeight(false)));

				canvasSprite->render();
			}
		}

		void gameReset()
		{
			LOGGER->msg("Set","StandardRenderSystem");

			config.clear();

			windowWidth = sora::SoraCore::Ptr->getScreenWidth();
			windowHeight = sora::SoraCore::Ptr->getScreenHeight();

			//coreCanvas = NULL;
 			coreCanvas = new sora::SoraBaseCanvas(gameWidth,gameHeight);

			if(MAPMGR.GetSelectedMaps().size()>0) {
				preview = sora::SoraCore::Ptr->createSprite(MAPMGR.GetThumbFilePath(MAPMGR.GetSelectedMaps()[0].id));
			}
			white = sora::SoraCore::Ptr->createSprite("Data/white.png");
			if(!white->getTexture())
				DIVA_EXCEPTION_MODULE("Do not found white.png!","RenderSystem");
			if(preview->getTexture())
				preview->setScale(double(gameWidth)/preview->getSpriteWidth(),
				double(gameHeight)/preview->getSpriteHeight());
			white->setScale(double(gameWidth)/white->getSpriteWidth(),
				double(gameHeight)/white->getSpriteHeight());
			//coreCanvas = new sora::SoraBaseCanvas(gameWidth,gameHeight);
		}

		void gameStop()
		{
			SAFE_DELETE(coreCanvas);
			SAFE_DELETE(preview);
			SAFE_DELETE(white);
		}

		virtual void fadeOut(uint32 color) 
		{
			if(white==NULL)
				return;
			bFade = true;
			white->clearEffects();
			white->setColor(color);
			white->addEffect(sora::CreateEffectFade(0,1,fadeTime));
			white->update(0);
		}
		virtual void fadeIn(uint32 color) 
		{
			if(white==NULL)
				return;
			bFade = true;
			white->clearEffects();
			white->setColor(color);
			white->addEffect(sora::CreateEffectFade(1,0,fadeTime));
			white->update(0);
		}
		virtual bool isFade() {return bFade;}

		void setGameResolution(int width, int height) {this->gameWidth = width, this->gameHeight = height;}
		int getGameWidth() {return gameWidth;}
		int getGameHeight() {return gameHeight;}
		int getWindowWidth() {return windowWidth;}
		int getWindowHeight() {return windowHeight;}
	};
}

#endif