/*
 *  DivaHookManager.h
 *
 *  Created by Hyf042 on 2/3/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_HOOK_MANAGER
#define DIVA_HOOK_MANAGER

#include "DivaEventManager.h"
#include "DivaHook.h"

namespace divacore
{
	/*
	 * HookManager, involved all the hooks and interact to other components
	 */
	class HookManager : public EventHandler
	{
	public:
		virtual void insert(HookPtr hook) = 0;
		virtual void del(HookPtr hook) = 0;
		virtual void del(const std::string &name) {del(getHook(name));}
		virtual HookPtr getHook(const std::string &name) {return NULL;}

		virtual bool hook(StateEvent &key) {return false;}
		virtual bool hook(float x, float y, SoraSprite*sprite, const std::string&tag) {return false;}
		virtual bool hook(KeyEvent &key) {return false;}
		virtual bool hook(const std::string &ID, const std::string &channel, const std::string &tag) {return false;}
		virtual bool hook(NotePtr note) {return false;}
		virtual bool hook(MapNote &note) {return false;}
		virtual int hookInfo() {return 0;}

		virtual void update(float dt) {}
		virtual void render() {}

		virtual HookPtr createHook(const std::string &hookName) {return NULL;}
	};

	typedef HookManager* HookManagerPtr;
};

#endif