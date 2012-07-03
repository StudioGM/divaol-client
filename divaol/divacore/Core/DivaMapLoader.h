/*
 *  DivaMapLoader.h
 *
 *  Created by Hyf042 on 1/13/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_MAP_LOADER
#define DIVA_MAP_LOADER

#include "DivaMapInfo.h"
#include "DivaEventManager.h"

namespace divacore
{
	/*
	MapLoader
	create a mapInfo from specific file
	*/
	class MapLoader : public EventHandler
	{
		std::string mapFileName,songFolderPath;
	protected:
		virtual uint32 _parserDifficulty(const std::string &diff);
		virtual uint32 _parserResourceType(const std::string &type);
	public:
		virtual MapInfoPtr load() = 0;
		virtual void setSong(const std::string &path, const std::string &mapFileName = "") {songFolderPath=path,this->mapFileName=mapFileName;}
		std::string getMapFileName() {return mapFileName;}
		std::string getSongPath() {return songFolderPath;}
	};

	typedef MapLoader* MapLoaderPtr;
}

#endif