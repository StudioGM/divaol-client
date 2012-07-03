/*
 *  DivaHook.h
 *
 *  Created by Hyf042 on 2/3/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_HOOK
#define DIVA_HOOK

#include "DivaGameMode.h"
#include "SoraSprite.h"

namespace divacore
{
	/*
	 * Hook , a object to hook the render, state, input, sound or other key event 
	 */
	class Hook
	{
		int info;
		int priority;
		bool bActive;
	public:
		enum{LOW = 0, FLAT = 0x80, NORMAL = 0xff, HIGH = 0x8000,TOP = 0xffff};
		enum{STATE = 1,RENDER = 2,INPUT = 4,SOUND = 8,NOTE = 16};

		Hook():priority(NORMAL),bActive(false) {}
		Hook(int priority):priority(priority),bActive(false) {}

		inline bool hasAbility(int ability) {return (getAbility()&ability)==ability;}
		inline void active() {bActive = true;onAwake();}
		inline void sleep() {bActive = false;onSleep();}
		inline bool isActive() {return bActive;}
		inline int getPriority() {return priority;}
		inline void setPriority(int priority) {this->priority=priority;}
		inline void setHookInfo(int info) {this->info = info;}
		inline int hookInfo() {return info;}

		virtual std::string getName() {return "";}
		virtual int getAbility() = 0;
		virtual void onAwake() {};
		virtual void onSleep() {};
		virtual void update(float dt) {};
		virtual bool condition() = 0;

		virtual bool hook(StateEvent &key) {return false;}
		virtual bool hook(float x, float y, SoraSprite*sprite, const std::string&tag) {return false;}
		virtual bool hook(KeyEvent &key) {return false;}
		virtual bool hook(NotePtr note) {return false;}
		virtual bool hook(const std::string &ID, const std::string &channel, const std::string &tag) {return false;}

		virtual void render() {}

		friend bool operator<(const Hook &a, const Hook &b) {return a.priority>b.priority;}
	};

	typedef Hook* HookPtr;

	/*
	 * HookQueue, a queue to maintain a sorted hook list
	 */
	class HookQueue
	{
		typedef std::vector<HookPtr> HOOK_QUEUE;

		HOOK_QUEUE hooks;
	public:
		int size() {return hooks.size();}
		void insert(HookPtr hook)
		{
			for(HOOK_QUEUE::iterator ptr = hooks.begin(); ptr != hooks.end(); ptr++)
				if((*hook)<*(*ptr))
				{
					hooks.insert(ptr,hook);
					return;
				}
			hooks.insert(hooks.end(),hook);
		}
		void del(HookPtr hook)
		{
			for(HOOK_QUEUE::iterator ptr = hooks.begin(); ptr != hooks.end(); ptr++)
				if(hook==*ptr)
				{
					if(hook->isActive())
						hook->sleep();
					hooks.erase(ptr);
					break;
				}
		}
		void clear(bool isDelete = false)
		{
			for(HOOK_QUEUE::iterator ptr = hooks.begin(); ptr != hooks.end(); ptr++)
				if(((*ptr))->isActive())
				{
					(*ptr)->sleep();
					if(isDelete)
						delete (*ptr);
				}
			hooks.clear();
		}
		HookPtr operator[](int index) 
		{
			return get(index);
		}
		HookPtr get(int index)
		{
			if(index<0||index>=hooks.size())
				DIVA_EXCEPTION_MODULE("index out of range","HookQueue");
			return hooks[index];
		}
		void update(float dt)
		{
			for(HOOK_QUEUE::iterator ptr = hooks.begin(); ptr != hooks.end(); ptr++)
			{
				if((*ptr)->isActive())
					(*ptr)->update(dt);
				else if((*ptr)->condition())
					(*ptr)->active();
			}
		}
		void render()
		{
			for(HOOK_QUEUE::iterator ptr = hooks.begin(); ptr != hooks.end(); ptr++)
			{
				if((*ptr)->isActive())
					(*ptr)->render();
			}
		}
		HookQueue& operator+=(HookPtr hook) {insert(hook);return*this;}
		HookQueue& operator-=(HookPtr hook) {del(hook);return*this;}
	};
}

#endif