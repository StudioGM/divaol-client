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
#include "divanetwork/DivaNetInfo.h"
#include "json/json.h"
#include "Lib/Base/Base.h"

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
			mConfigFile = configFile;

			if(configFile != "" && Base::FileUtil::FileExist(configFile))
			{
				Config config;
				configloader::loadWithJson(config,configFile);

				mUserName = config.getAsString("username");
			}

			/*mUid = config.getAsString("uid");
			mModule = config.getAsString("module");

			mHooks.clear();
			if(config.has("hook"))
			{
				AnyList list = config.getAsList("hook");
				for(AnyList::iterator ptr = list.begin(); ptr != list.end(); ptr++)
					mHooks.push_back(AnyCast<std::string>(*ptr));
			}*/
		}
		void saveSettings(const std::string &configFile) {
			Json::StyledWriter writer;
			Json::Value save;

			save["username"] = mUserName;
			Base::base_string p = writer.write(save);
			
			FILE* file = fopen(configFile.c_str(), "wb");
			fwrite(p.c_str(), p.length(), 1, file);
			fclose(file);
		}

		void loadFromNetInfo(const std::string &username)
		{
			mUserName = username;
	
			if(checkNeedStore())
				saveSettings(mConfigFile);
		}

		bool checkNeedStore()
		{
			return mConfigFile != "" && mUserName != "";
		}

		void clear() {
			mUserName = "";
			mUid = "";
			mModule = "";
			mHooks.clear();
		}

		inline std::string username() {return mUserName;}
		inline std::string uid() {return mUid;}
		inline std::string module() 
		{
			return mModule;
		}
		inline void setUid(const std::string &uid) {mUid = uid;} 
		inline std::vector<std::string> hooks() {return mHooks;}
	private:
		std::string mUserName;
		std::string mUid;
		std::string mModule;
		std::string mConfigFile;
		std::vector<std::string> mHooks;
	};
}

#endif