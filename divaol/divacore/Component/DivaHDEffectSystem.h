/*
 *  DivaHDEffectSystem.h
 *
 *  Created by Hyf042 on 3/20/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_HD_EFFECT_SYSTEM
#define DIVA_HD_EFFECT_SYSTEM

#include "Core/DivaEffectSystem.h"
#include "Utility/DivaConfigLoader.h"
#include "Utility/DivaHDPSystem.h"
#include "SoraSprite.h"
#include "SoraTextureMap.h"

namespace divacore
{
	class Effect_PressSafe : public BaseEffect
	{
		sora::SoraSprite *image;
		float scale;

	protected:
		std::string getName() {return "PressSafe";}
		virtual void onInitialize()
		{
			scale = 0;
		}
		virtual void onUpdate(float dt) 
		{
			if(nowTime<totalTime/3)
				scale += dt*480;
			else
				scale += dt*240;

			image->setScale(scale/image->getSpriteWidth(),scale/image->getSpriteHeight());

			int alpha = (totalTime-nowTime)*150/totalTime;
			image->setColor(CSETA(image->getColor(),alpha));

			if(getState()==STOP)
				over();
		}
		virtual void onRender() 
		{
			Core::Ptr->render(image,"+effect"+getTag());
		}
		virtual void onConstruct(Config &config, const std::string &head)
		{
			image = new sora::SoraSprite();
			image->setTexture(source);
			//sora::SoraTextureMap::Instance()->addRf(source);
			Rect imageRect = config.getAsRect(head+"textureRect");
			image->setTextureRect(imageRect.x,imageRect.y,imageRect.w,imageRect.h);
			image->setCenter(imageRect.w/2,imageRect.h/2);

			image->setPosition(x,y);
		}
		virtual void onDestroy()
		{
			SAFE_DELETE(image);
		}
	};

	class Effect_PressFine : public BaseEffect
	{
		Rect imageRect;
		sora::SoraSprite *image;
	protected:
		std::string getName() {return "PressFine";}
		virtual void onInitialize()
		{
		}
		virtual void onUpdate(float dt) 
		{
			int cnt = nowTime/totalTime*8;
			image->setTextureRect(imageRect.x+cnt%4*imageRect.w,imageRect.y+cnt/4*imageRect.h,imageRect.w,imageRect.h);
			
			if(getState()==STOP)
				over();
		}
		virtual void onRender() 
		{
			Core::Ptr->render(image,"+effect"+getTag());
		}
		virtual void onConstruct(Config &config, const std::string &head)
		{
			image = new sora::SoraSprite();
			image->setTexture(source);
			//sora::SoraTextureMap::Instance()->addRf(source);
			imageRect = config.getAsRect(head+"textureRect");
			image->setTextureRect(imageRect.x,imageRect.y,imageRect.w,imageRect.h);
			image->setCenter(imageRect.w/2,imageRect.h/2);
			
			image->setPosition(x,y);

			image->setColor(CSETA(image->getColor(),200));
			image->setScale(1,1);
		}
		virtual void onDestroy()
		{
			SAFE_DELETE(image);
		}
	};

	class Effect_PressFlame : public BaseEffect
	{
		Rect imageRect;
		sora::SoraSprite *image;
	protected:
		std::string getName() {return "PressFlame";}
		virtual void onInitialize()
		{
		}
		virtual void onUpdate(float dt) 
		{
			float scale = nowTime/totalTime*760/*1720*/;

			image->setScale(scale/image->getSpriteWidth(),scale/image->getSpriteHeight());

			int alpha = (totalTime-nowTime)*255/totalTime;
			image->setColor(CSETA(image->getColor(),alpha));

			if(getState()==STOP)
				over();
		}
		virtual void onRender() 
		{
			Core::Ptr->render(image,"+effect"+getTag());
		}
		virtual void onConstruct(Config &config, const std::string &head)
		{
			image = new sora::SoraSprite();
			image->setTexture(source);
			//sora::SoraTextureMap::Instance()->addRf(source);
			

			imageRect = config.getAsRect(head+"textureRect");
			image->setTextureRect(imageRect.x,imageRect.y,imageRect.w,imageRect.h);
			image->setCenter(imageRect.w/2,imageRect.h/2);

			image->setPosition(x,y);
		}
		virtual void onDestroy()
		{
			SAFE_DELETE(image);
		}
	};

	class Effect_PressCool : public BaseEffect
	{
		Rect imageRect;
		sora::SoraSprite *image;
	protected:
		std::string getName() {return "PressCool";}
		virtual void onInitialize()
		{
		}
		virtual void onUpdate(float dt) 
		{
			int cnt = nowTime/totalTime*12;
			image->setTextureRect(imageRect.x+cnt%3*imageRect.w,imageRect.y+cnt/3*imageRect.h,imageRect.w,imageRect.h);

			if(getState()==STOP)
				over();
		}
		virtual void onRender() 
		{
			Core::Ptr->render(image,"+effect"+getTag());
		}
		virtual void onConstruct(Config &config, const std::string &head)
		{
			image = new sora::SoraSprite();
			image->setTexture(source);
			//sora::SoraTextureMap::Instance()->addRf(source);

			imageRect = config.getAsRect(head+"textureRect");
			image->setTextureRect(imageRect.x,imageRect.y,imageRect.w,imageRect.h);
			image->setCenter(imageRect.w/2,imageRect.h/2);

			image->setPosition(x,y);

			image->setColor(CSETA(image->getColor(),150));
			image->setScale(4,4);
		}
		virtual void onDestroy()
		{
			SAFE_DELETE(image);
		}
	};
	/*
	HDEffectSystem
	the effects pull from diva HD
	*/
	class HDEffectSystem : public EffectSystem
	{
		typedef std::map<std::string,sora::SoraTextureHandle> SOURCES;
		typedef std::vector<BaseEffect*> EFFECTS;

		SOURCES sources;
		EFFECTS effects;

		PSystem pSystem;
		int level;
	
	private:
		void delEffect(BaseEffect* effect)
		{
			effect->destroy();
			SAFE_DELETE(effect);
		}
	public:
		HDEffectSystem():level(2) {}

		void gameReset() 
		{
			sources.clear();
			effects.clear();
		}
		void gameStop()
		{
			for(SOURCES::iterator ptr = sources.begin(); ptr != sources.end(); ptr++)
				sora::SoraTextureMap::Instance()->decRf(ptr->second);
			//	sora::SoraCore::Ptr->releaseTexture(ptr->second);
			sources.clear();
			clear();
		}
		void gameLoad(const std::string &configFile)
		{
			configloader::loadWithJson(config,configFile);
		}
		void gameLoadFromConfig(Config &config)
		{
			this->config = config;
		}

		void clear()
		{
			for(EFFECTS::iterator ptr = effects.begin(); ptr != effects.end(); ptr++)
				delEffect(*ptr);
			effects.clear();
			pSystem.Clear();
		}

		virtual void insert(BaseEffect* effect) 
		{
			effects.push_back(effect);
		}
		virtual void del(BaseEffect* effect) 
		{
			for(EFFECTS::iterator ptr = effects.begin(); ptr != effects.end(); ptr++)
				if((*ptr)==effect)
				{
					effects.erase(ptr);
					break;
				}
				delEffect(effect);
		}

		virtual void update(float dt)
		{
			pSystem.Update(dt*1000);

			for(EFFECTS::iterator ptr = effects.begin(); ptr != effects.end(); ptr++)
				if(!(*ptr)->isOver())
					(*ptr)->update(dt);

			EFFECTS tmpEffects = effects;
			effects.clear();
			for(EFFECTS::iterator ptr = tmpEffects.begin(); ptr != tmpEffects.end(); ptr++)
			{
				if((*ptr)->isOver())
					delEffect(*ptr);
				else
					effects.push_back(*ptr);
			}
		}
		virtual void render()
		{
			for(EFFECTS::iterator ptr = effects.begin(); ptr != effects.end(); ptr++)
				(*ptr)->render();

			pSystem.Draw();
		}

		virtual BaseEffect* createEffect(const std::string &effectName)
		{
			BaseEffect *effect = NULL;
			std::string type = config.getAsString(effectName+"_type");
			if(type=="PressSafe")
				effect = new Effect_PressSafe();
			else if(type=="PressFine")
				effect = new Effect_PressFine();
			else if(type=="PressFlame")
				effect = new Effect_PressFlame();
			else if(type=="PressCool")
				effect = new Effect_PressCool();
			return effect;
		}

		float getTime(const std::string &effectName) 
		{
			return config.getAsDouble(effectName+"_"+"time");
		}

		sora::SoraTextureHandle getSource(const std::string &effectName)
		{
			if(sources.find(effectName)==sources.end())
			{
				SoraTextureHandle tex = sora::SoraCore::Ptr->createTexture(config.getAsString(effectName+"_"+"image"));
				sources[effectName] = tex;
				sora::SoraTextureMap::Instance()->addRf(tex);
			}
			return sources[effectName];
		}

		void addParticle(std::string type, ParticleData data, NotePtr note)
		{
			if(CORE_PTR->getMode()=="editMode")
				return;

			if(type=="comet")
				pSystem.AddCometParticle(data.x,data.y,data.dx,data.dy,note);
			else if(type=="star")
				pSystem.AddStarParticle(data.x,data.y,data.size,note);
		}

		void clearParticle(NotePtr note)
		{
			pSystem.SetState(0x8000,note->getID());
		}

		void setEffectLevel(int level) 
		{
			this->level = level;
		}
		int getEffectLevel() 
		{
			return level;
		} 
	};
}

#endif