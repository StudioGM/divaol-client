/*
 *  DivaGameModule.h
 *
 *  Created by Hyf042 on 2/5/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_GAME_MODULE
#define DIVA_GAME_MODULE

#include "DivaCommon.h"
#include "DivaConfig.h"
#include "DivaEventManager.h"

namespace divacore
{
	class GameModule : public EventHandler
	{
		Config config;
	public:
		virtual std::string getName() = 0;
		virtual void load(const std::string &path) = 0;
		virtual void distribute() = 0;
		virtual std::string getNoteFile() {return "";}
		virtual std::string getUIFile() {return "";}
		virtual std::string getRenderFile() {return "";}
		virtual std::string getEffectFile() {return "";}
		virtual std::string getResultFile() {return "";}
	};

	typedef GameModule* GameModulePtr;
}

#endif