/*
 *  DivaJsonGameModule.h
 *
 *  Created by Hyf042 on 2/7/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_JSON_GAME_MODULE
#define DIVA_JSON_GAME_MODULE

#include "Core/DivaGameModule.h"

namespace divacore
{
	class JsonGameModule : public GameModule
	{
		Config config;
		std::string name,path;
		std::string noteFile;
		std::string uiFile;
		std::string renderFile;
		std::string effectFile;
		std::string resultFile;
		std::vector<std::string> hooks;

		sora::SoraResourceHandle moduleFolder,defaultFolder;
	public:
		std::string getName() {return name;}
		void gameStop();
		void init() {}
		void load(const std::string &path);
		void registerHook();
		void distribute();
		void clear();
		std::string getNoteFile() {return noteFile;}
		std::string getUIFile() {return uiFile;}
		std::string getRenderFile() {return renderFile;}
		std::string getEffectFile() {return effectFile;}
		std::string getResultFile() {return resultFile;}
	};
}

#endif