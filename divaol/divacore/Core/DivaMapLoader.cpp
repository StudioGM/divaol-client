/*
 *  DivaMapLoader.cpp
 *
 *  Created by Hyf042 on 1/13/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#include "DivaMapLoader.h"

namespace divacore
{
	uint32 MapLoader::_parserDifficulty(const std::string &diff)
	{
		if(diff=="easy")
			return MapHeader::EASY;
		else if(diff=="normal")
			return MapHeader::NORMAL;
		else if(diff=="hard")
			return MapHeader::HARD;
		else if(diff=="extra")
			return MapHeader::EXTRA;
		else if(diff=="die")
			return MapHeader::DIE;
		else
			DIVA_EXCEPTION_MODULE("difficulty error!","MapLoader");
	}
	uint32 MapLoader::_parserResourceType(const std::string &type)
	{
		if(type=="audio")
			return MapResourceInfo::AUDIO;
		else if(type=="video")
			return MapResourceInfo::VIDEO;
		else if(type=="image")
			return MapResourceInfo::IMAGE;
		else
			DIVA_EXCEPTION_MODULE("resource type error!","MapLoader");
	}
}