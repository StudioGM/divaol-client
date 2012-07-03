/*
 *  DivaDefault.h
 *
 *  Created by Hyf042 on 1/13/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_DEFAULT
#define DIVA_DEFAULT

#include "SoraCore.h"
#include "DivaConfig.h"

namespace divacore
{
	#define DEFAULT_INFO_PTR (DefaultInfo::instance())
	/*
	DefaultInfo
	default information
	*/
	class DefaultInfo
	{
		Config config;
	public:
		static DefaultInfo* instance() {static DefaultInfo instance; return &instance;}
		DefaultInfo() {setDefaultInfo();}
		//void getDefaultFile() {return "default"}
		void setDefaultInfo();
		std::string getDefaultFile() {return "default";}
		std::string getAsString(const std::string &key) {return config.getAsString(key);}
		int getAsInt(const std::string &key) {return config.getAsInt(key);}
		double getAsDouble(const std::string &key) {return config.getAsDouble(key);}
		bool getAsBool(const std::string &key) {return config.getAsBool(key);}
		Rect getAsRect(const std::string &key) {return config.getAsRect(key);}
		Point getAsPoint(const std::string &key) {return config.getAsPoint(key);}
		//const std::string& getModuleFile() {return moduleFile;}
	};
}

#endif