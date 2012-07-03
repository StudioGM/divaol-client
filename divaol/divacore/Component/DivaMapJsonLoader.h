/*
 *  DivaMapJsonaLoader.h
 *
 *  Created by Hyf042 on 1/13/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_MAP_JSON_LOADER
#define DIVA_MAP_JSON_LOADER

#include "Core/DivaMapLoader.h"
#include "Utility/DivaJsonHelper.h"
#include "json/json.h"

namespace divacore
{
	/*
	MapJsonLoader
	load file with json format
	*/
	class MapJsonLoader : public MapLoader
	{
		MapInfoPtr mapInfo;
		sora::SoraResourceHandle songFolder;
		void _parserHeader(const Json::Value& val);
		void _parserResource(const Json::Value& val);
		void _parserNotes(const Json::Value& val);
		NotePoint _parserNote(const Json::Value& val);
		void _parserEvents(const Json::Value& val);
	public:
		MapJsonLoader():mapInfo(NULL) {}
		virtual void gameReset();
		virtual void gameStop();
		virtual MapInfoPtr load();
	};
}

#endif