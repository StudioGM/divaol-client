/*
 *  DivaConfigLoader.h
 *
 *  Created by Hyf042 on 1/25/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_CONFIG_LOADER
#define DIVA_CONFIG_LOADER

#include "Core/DivaConfig.h"
#include "json/json.h"

namespace divacore
{
	using namespace sora;

	namespace configloader
	{
		void loadWithJson(Config &config, const std::string &file, bool ignore = false);
		Json::Value loadRawJson(const std::string &file);
	}
}

#endif