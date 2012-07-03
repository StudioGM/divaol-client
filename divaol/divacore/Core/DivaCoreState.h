/*
 *  DivaCoreState.h
 *
 *  Created by Hyf042 on 1/14/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_CORE_STATE
#define DIVA_CORE_STATE

#include "DivaCommon.h"
#include "DivaCore.h"
#include "app/SoraGameState.h"
#include "SoraLogger.h"
#include "DivaEventManager.h"

namespace divacore
{
	using namespace sora;

	class Core;
	class MapLoader;
	class MapParser;
	class CoreFlow;
	class ItemFacotry;
	typedef MapLoader* MapLoaderPtr;
	typedef MapParser* MapParserPtr;
	typedef CoreFlow* CoreFlowPtr;
	typedef ItemFactory* ItemFactoryPtr;
	typedef Core* CorePtr;

	/*
	CoreState
	A game state, standing for a stage
	*/
	class CoreState : public SoraGameState, public sora::SoraEventHandler, public EventHandler
	{
	protected:
		CorePtr core;

		void setCoreState(int state);
		void setMapInfo(MapInfoPtr info);
	public:
		void setCore(CorePtr core) {this->core = core;}
	};

	typedef CoreState* CoreStatePtr;
}

#endif