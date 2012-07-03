/*
 *  DivaMapParser.h
 *
 *  Created by Hyf042 on 1/13/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_MAP_PARSER
#define DIVA_MAP_PARSER

#include "DivaMapInfo.h"
#include "DivaCore.h"
#include "DivaEventManager.h"

namespace divacore
{
	class Core;
	typedef Core* CorePtr;

	/*
	MapLoader
	create a mapInfo from specific file
	*/
	class MapParser : public EventHandler
	{
	protected:
		CorePtr core;
	public:
		void setCore(CorePtr core) {this->core = core;}
		virtual void parser(MapInfoPtr mapInfo) = 0;
	};

	typedef MapParser* MapParserPtr;
}

#endif