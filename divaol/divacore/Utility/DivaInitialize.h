#ifndef DIVA_INJECT
#define DIVA_INJECT

#include "Core/DivaCore.h"
#include "State/DivaCorePlayState.h"
#include "State/DivaCoreLoadState.h"
#include "State/DivaCorePauseState.h"
#include "State/DivaCoreResultState.h"
#include "State/DivaUnsyncLoad.h"
#include "State/DivaEditLoad.h"
#include "Component/DivaMapJsonLoader.h"
#include "Component/DivaMapStandardParser.h"
#include "Component/DivaStandardCoreFlow.h"
#include "Component/DivaStandardFactory.h"
#include "Component/DivaStandardDisplay.h"
#include "Component/DivaStandardRenderSystem.h"
#include "Component/DivaStandardInputManager.h"
#include "Component/DivaSimpleHookManager.h"
#include "Component/DivaSimpleUIPainter.h"
#include "Component/DivaJsonGameModule.h"
#include "Component/DivaCommonEvaluateStrategy.h"
#include "Component/DivaHDEffectSystem.h"
//#include "Component/DivaSocketNetworkSystem.h"
#include "Network/DivaEnetSystem.h"
#include "Network/DivaNetLoader.h"
#include "Network/DivaNetUnsync.h"
#include "Mode/DivaSinglePlay.h"
#include "Mode/DivaMultiplay.h"
#include "Mode/DivaTeamPlay.h"
#include "Mode/DivaRelayPlay.h"
#include "Mode/DivaEditMode.h"
#include "MusicManager/DivaFmodMusicManager.h"
//#include "MusicManager/DivaXAudioMusicManager.h"
#include "MusicManager/DivaBassMusicManager.h"

//#define NET
#define UNSYNC
//#define EDIT

namespace divacore
{
	namespace standard
	{
		void pauseTheGame(void* args)
		{
			CORE_PTR->pause();
		}

		void readyEvent(void *args)
		{
			CORE_PTR->setState("play");
		}

		CorePtr initialize(std::string configFolder)
		{
			divacore::CorePtr core = INITIALIZE_DIVA_CORE;
			core->registerMapLoader(new divacore::MapJsonLoader);
			core->registerMapParser(new divacore::MapStandardParser);
			core->registerCoreFlow(new divacore::StandardCoreFlow);
			core->registerItemFactory(new divacore::StandardItemFactory);
			core->registerMusicManager(new divacore::BassMusicManager);
			core->registerDisplay(new divacore::StandardDisplay);
			core->registerRenderSystem(new divacore::StandardRenderSystem);
			core->registerInputManager(new divacore::StandardInputManager);
			core->registerHookManager(new divacore::SimpleHookManager);
			core->registerUIPainter(new divacore::SimpleUIPainter);
			core->registerGameModule(new divacore::JsonGameModule);
			core->registerNetworkSystem(new divacore::EnetSystem);
			core->registerEvaluateStrategy(new divacore::CommonEvaluateStrategy);
			core->registerEffectSystem(new divacore::HDEffectSystem);

#ifdef NET
			core->registerStateManager(new divacore::MultiPlay);
#else
#ifdef EDIT
			core->registerGameMode(new divacore::EditMode);
#else
			core->registerGameMode(new divacore::SinglePlay);
#endif
#endif
			//core->registerStateManager(new divacore::SinglePlay);
			//core->registerStateManager(new divacore::RelayPlay);
#ifndef UNSYNC
			core->addState(new divacore::CoreLoadState, "load");
			core->addState(new divacore::NetLoadState, "net_load");
#else
			core->addState(new divacore::UnsyncLoad,"load");
			core->addState(new divacore::NetUnsync, "net_load");
#endif
			core->addState(new divacore::CorePlayState, "play");
			core->addState(new divacore::CorePauseState, "pause");
			core->addState(new divacore::CoreResultState,"result");

#ifdef NET
			core->setInitState("net_load");
#else

#ifndef EDIT
			core->setInitState("load");
#else
			divacore::EditLoadState *load = new divacore::EditLoadState;
			core->addState(load, "edit_load");
			core->setInitState("edit_load");
			load->registerReayCallback(Task(&readyEvent));
			load->makeEmptySong(1,175);
			//core->setInitState("load");
			CORE_FLOW_PTR->registerBeginCallback(Task(&pauseTheGame));
#endif

#endif
			
			//load config
			core->prepare(configFolder+"/common.json");
			EVALUATE_STRATEGY_PTR->prepare(configFolder+"/eval.json");

			return core;
		}

	}
}

#endif