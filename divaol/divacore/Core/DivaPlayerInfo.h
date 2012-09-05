/*
 *  DivaPlayerInfo.h
 *
 *  Created by Hyf042 on 9/5/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_PLAYER_INFO_H
#define DIVA_PLAYER_INFO_H

#include "SoraCore.h"
#include "DivaConfig.h"
#include "Utility/DivaConfigLoader.h"

namespace divacore
{
	/*
	DefaultInfo
	default information
	*/
	class CorePlayerInfo
	{
	public:
		CorePlayerInfo() {}
		CorePlayerInfo(const std::string &configFile);

		void loadFromFile(const std::string &configFile) {
			Config config;
			configloader::loadWithJson(config,configFile);

			mName = config.getAsString("name");
			mUid = config.getAsString("uid");
			mModule = config.getAsString("module");

			AnyList list = config.getAsList("hooks");
			mHooks.clear();
			for(AnyList::iterator ptr = list.begin(); ptr != list.end(); ptr++)
				mHooks.push_back(AnyCast<std::string>(*ptr));
		}

		void clear() {
			mName = "";
			mUid = "";
			mModule = "";
			mHooks.clear();
		}

		inline std::string name() {return mName;}
		inline std::string uid() {return mUid;}
		inline std::string module() {return mModule;}
		inline std::vector<std::string> hooks() {return mHooks;}
	private:
		std::string mName;
		std::string mUid;
		std::string mModule;
		std::vector<std::string> mHooks;
	};
}

#endif