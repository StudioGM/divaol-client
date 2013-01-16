/*
 *  DivaSimpleHookManager.h
 *
 *  Created by Hyf042 on 2/3/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_SIMPLE_HOOK_MANAGER
#define DIVA_SIMPLE_HOOK_MANAGER

#include "Core/DivaHookManager.h"
#include "Hook/DivaAutoMode.h"
#include "Hook/DivaCTMode.h"
#include "Hook/DivaPVMode.h"
#include "Hook/DivaFastMode.h"
#include "Hook/DivaSlowMode.h"
#include "Hook/DivaDeathMode.h"
#include "Hook/DivaRandomSwapMode.h"
#include "Hook/DivaChaosMode.h"
#include "Hook/DivaDisappearMode.h"
#include "Hook/DivaBlackHouseMode.h"
#include "Hook/DivaNoFailMode.h"
#include "thread/SoraMutex.h"

namespace divacore
{
	class SimpleHookManager : public HookManager
	{
		typedef std::map<std::string,HookPtr> HOOK_MAP;

		int mHookInfo;
		HookQueue renderHooks;
		HookQueue stateHooks;
		HookQueue inputHooks;
		HookQueue soundHooks;
		HookQueue noteHooks;
		HookQueue hooks;
		HOOK_MAP hookMap;
		Config mConfig;
		sora::SoraMutex mutex;

	public:
		void gameReset()
		{
			LOGGER->msg("Set","SimpleHookManager");
		}

		void gameStop()
		{
			renderHooks.clear();
			stateHooks.clear();
			inputHooks.clear();
			soundHooks.clear();
			noteHooks.clear();
			hooks.clear(true);
			hookMap.clear();
		}

		void update(float dt)
		{
			sora::SoraMutexGuard lock(mutex);
			hooks.update(dt);
		}

		void render()
		{
			sora::SoraMutexGuard lock(mutex);
			hooks.render();
		}

		virtual void insert(HookPtr hook)
		{
			if(hook->getName()!="")
			{
				if(hookMap.find(hook->getName())==hookMap.end())
					hookMap[hook->getName()] = hook;
				else
				{
					LOGGER->notice("Hook "+hook->getName()+" already exists");
					return;
				}
			}

			if(hook->hasAbility(Hook::RENDER))
				renderHooks += hook;
			if(hook->hasAbility(Hook::STATE))
				stateHooks += hook;
			if(hook->hasAbility(Hook::INPUT))
				inputHooks += hook;
			if(hook->hasAbility(Hook::SOUND))
				soundHooks += hook;
			if(hook->hasAbility(Hook::NOTE))
				noteHooks += hook;

			mutex.lock();
			hooks += hook;
			mutex.unlock();
		}
		virtual void del(HookPtr hook)
		{
			if(hook==0)
				return;

			if(hook->hasAbility(Hook::RENDER))
				renderHooks -= hook;
			if(hook->hasAbility(Hook::STATE))
				stateHooks -= hook;
			if(hook->hasAbility(Hook::INPUT))
				inputHooks -= hook;
			if(hook->hasAbility(Hook::SOUND))
				soundHooks -= hook;
			if(hook->hasAbility(Hook::NOTE))
				noteHooks -= hook;
			hooks -= hook;
			for(HOOK_MAP::iterator ptr = hookMap.begin(); ptr != hookMap.end(); ptr++)
				if(ptr->second==hook)
				{
					hookMap.erase(ptr);
					break;
				}
		}

		virtual HookPtr getHook(const std::string &name) 
		{
			if(hookMap.find(name)!=hookMap.end())
				return hookMap[name];
			return NULL;
		}

		virtual bool hook(StateEvent &key) 
		{
			for(int i = 0; i < stateHooks.size(); i++)
				if(stateHooks[i]->isActive())
					if(stateHooks[i]->hook(key))
					{
						mHookInfo = stateHooks[i]->hookInfo();
						return true;
					}
			return false;
		}
		virtual bool hook(float x, float y, SoraSprite*sprite, const std::string&tag) 
		{
			for(int i = 0; i < renderHooks.size(); i++)
				if(renderHooks[i]->isActive())
					if(renderHooks[i]->hook(x, y, sprite, tag))
					{
						mHookInfo = renderHooks[i]->hookInfo();
						return true;
					}
			return false;
		}
		virtual bool hook(KeyEvent &key) 
		{
			for(int i = 0; i < inputHooks.size(); i++)
				if(inputHooks[i]->isActive())
					if(inputHooks[i]->hook(key))
					{
						mHookInfo = inputHooks[i]->hookInfo();
						return true;
					}
			return false;
		}
		virtual bool hook(const std::string &ID, const std::string &channel, const std::string &tag) 
		{
			for(int i = 0; i < soundHooks.size(); i++)
				if(soundHooks[i]->isActive())
					if(soundHooks[i]->hook(ID,channel,tag))
					{
						mHookInfo = soundHooks[i]->hookInfo();
						return true;
					}
			return false;
		}
		virtual bool hook(NotePtr note)
		{
			for(int i = 0; i < noteHooks.size(); i++)
				if(noteHooks[i]->isActive())
					if(noteHooks[i]->hook(note))
					{
						mHookInfo = noteHooks[i]->hookInfo();
						return true;
					}
					return false;
		}
		virtual bool hook(MapNote &note)
		{
			for(int i = 0; i < noteHooks.size(); i++)
				if(noteHooks[i]->isActive())
					if(noteHooks[i]->hook(note))
					{
						mHookInfo = noteHooks[i]->hookInfo();
						return true;
					}
					return false;
		}
		int hookInfo() {return mHookInfo;}

		void gameLoad(const std::string configFile)
		{
			configloader::loadWithJson(mConfig,configFile);
		}
		Config& getConfig()
		{
			return mConfig;
		}

		virtual float getHookScoreBonus()
		{
			float bonus = 0;
			for(int i = 0; i < hooks.size(); i++)
				if(hooks[i]->isActive())
					bonus += hooks[i]->scoreBonusScale();

			return bonus;
		}

		virtual float getHookScoreScale()
		{
			float scale = 1;
			for(int i = 0; i < hooks.size(); i++)
				if(hooks[i]->isActive())
					scale *= hooks[i]->scoreTotalScale();

			return scale;
		}

		HookPtr createHook(const std::string &hookName)
		{
			if(hookName=="PVMode")
				return new PVMode;
			else if(hookName=="CTMode")
				return new CTMode;
			else if(hookName=="AutoMode")
				return new AutoMode;
			else if(hookName=="FastMode")
				return new FastMode;
			else if(hookName=="SlowMode")
				return new SlowMode;
			else if(hookName=="DeathMode")
				return new DeathMode;
			else if(hookName=="RandomSwapMode")
				return new RandomSwapMode;
			else if(hookName=="ChaosMode")
				return new ChaosMode;
			else if(hookName=="DisappearMode")
				return new DisappearMode;
			else if(hookName=="BlackHouseMode")
				return new BlackHouseMode;
			else if(hookName=="NoFailMode")
				return new NoFailMode;

			DIVA_EXCEPTION_MODULE("Hook "+hookName+" not found!","SimpleHookManager");
			return NULL;
		}
	};
};

#endif