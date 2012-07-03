/*
 *  DivaEffectSystem.h
 *
 *  Created by Hyf042 on 2/10/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_EFFECT_SYSTEM
#define DIVA_EFFECT_SYSTEM

#include "DivaCommon.h"
#include "DivaMapInfo.h"
#include "DivaEventManager.h"
#include "DivaParticleSystem.h"

namespace divacore
{
	using namespace sora;

	class BaseEffect
	{
	protected:
		float x,y,totalTime,nowTime;
		int state;
		sora::SoraTextureHandle source;
		std::string tag;

	protected:
		virtual void onStart() {}
		virtual void onInitialize() {}
		virtual void onDestroy() {}
		virtual void onUpdate(float dt) {}
		virtual void onRender() {}
		virtual void onConstruct(Config &config, const std::string &head) {}
		void setState(int state) {this->state=state;}
	public:
		enum{INIT,READY,RUN,STOP,OVER};

		BaseEffect():x(x),y(y),state(INIT),source(0),tag("") {}
		virtual std::string getName() {return "BaseEffect";}
		
		void over() {setState(OVER);}
		bool isOver() {return getState()==OVER;}
		void setPosition(float x, float y) {this->x=x,this->y=y;}
		int getState() {return state;}
		void setTag(const std::string &tag) {this->tag=tag;}
		std::string getTag() {return tag;}

		void init(float x, float y, float t, sora::SoraTextureHandle source = 0) {
			totalTime = t;
			this->source = source;
			setPosition(x,y);
			setState(READY);
			onInitialize();
		}
		void construct(Config &config) {onConstruct(config,getName()+"_");}
		void destroy() {onDestroy();}
		void start() {
			nowTime = 0;
			setState(RUN);
			onStart();
			flush();
		}
		void flush() {update(0);}
		void update(float dt) {
			nowTime += dt;
			if(nowTime>=totalTime)
			{
				nowTime = totalTime;
				setState(STOP);
			}
			onUpdate(dt);
		}
		void render() {onRender();}
	};
	/*
	EffectSystem
	*/
	class EffectSystem : public EventHandler
	{
	protected:
		Config config;
	public:
		//the function will be called at first when restart game
		//virtual void gameReset();
		//the function will be called when game is stopped
		//virtual void gameStop();
		//the function will be called when 
		//virtual void gameStart();
		//the function will be called while loading phase, and parameter is the config file of json 
		//virtual void gameLoad(const std::string configFile);
		//the function will be called when Core is initializing
		//virtual void init();

		virtual void insert(BaseEffect*) = 0;
		virtual void del(BaseEffect*) = 0;
		virtual void clear() = 0;

		virtual void update(float dt) {}
		virtual void render() {}

		virtual BaseEffect* createEffect(const std::string &) = 0;
		virtual float getTime(const std::string &) = 0;
		virtual sora::SoraTextureHandle getSource(const std::string &) = 0;

		virtual void addEffect(const std::string &name, float x, float y, const std::string &tag="") {
			BaseEffect *effect = createEffect(name);
			if(effect)
			{
				effect->init(x,y,getTime(name),getSource(name));
				effect->construct(config);
				effect->setTag(tag);
			}
			insert(effect);
			effect->start();
		}
		virtual void addParticle(std::string type, ParticleData data, NotePtr note) = 0;
		virtual void clearParticle(NotePtr note) = 0;
	};

	typedef EffectSystem* EffectSystemPtr;
}

#endif