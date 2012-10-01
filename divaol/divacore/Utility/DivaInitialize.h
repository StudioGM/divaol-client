#ifndef DIVA_INITIALIZER
#define DIVA_INITIALIZER

#include "Core/DivaCore.h"
#include "State/DivaCorePlayState.h"
//#include "State/DivaCoreLoadState.h"
#include "State/DivaCorePauseState.h"
#include "State/DivaCoreResultState.h"
//#include "State/DivaUnsyncLoad.h"
//#include "State/DivaEditLoad.h"
#include "State/DivaCoreLoader.h"
#include "Component/DivaMapJsonLoader.h"
//#include "Component/DivaMapWJsonLoader.h"
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
//#include "Network/DivaEnetSystem.h"
#include "State/DivaNetLoader.h"
//#include "Network/DivaNetUnsync.h"
#include "Mode/DivaSinglePlay.h"
#include "Mode/DivaMultiplay.h"
#include "Mode/DivaPairPlay.h"
#include "Mode/DivaRelayPlay.h"
#include "Mode/DivaEditMode.h"
#include "MusicManager/DivaFmodMusicManager.h"
//#include "MusicManager/DivaXAudioMusicManager.h"
#include "MusicManager/DivaBassMusicManager.h"
#include "divanetwork/DivaNetworkManager.h"

namespace divacore
{
	namespace standard
	{
		class Initializer
		{
		public:
			enum GameModeFlag{SINGLE,EDIT,MULTI};

		protected:
			divacore::CorePtr core;
			std::string configFolder;
			GameModeFlag gameMode;
			bool isUnsync;

		protected:
			virtual void registerComponents()
			{
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
				//core->registerNetworkSystem(new divacore::EnetSystem);
				core->registerNetworkSystem(STAGE_CLIENT.getSys());
				core->registerEvaluateStrategy(new divacore::CommonEvaluateStrategy);
				core->registerEffectSystem(new divacore::HDEffectSystem);
			}

			void registerGameMode()
			{
				switch(gameMode)
				{
				case SINGLE:
					core->registerGameMode(new divacore::SinglePlay);
					break;
				case EDIT:
					core->registerGameMode(new divacore::EditMode);
					break;
				case MULTI:
					core->registerGameMode(new divacore::MultiPlay);
					break;
				}
			}

			void registerStates()
			{
				core->addState(new divacore::CoreLoader(isUnsync,true),"load");
				core->addState(new divacore::NetLoader(isUnsync,true), "net_load");

				core->addState(new divacore::CorePlayState, "play");
				core->addState(new divacore::CorePauseState, "pause");
				core->addState(new divacore::CoreResultState,"result");

				switch(gameMode)
				{
				case SINGLE:
				case EDIT:
					core->setInitState("load");
					break;
				case MULTI:
					core->setInitState("net_load");
				}
			}

		public:
			Initializer(std::string configFolder = "system", GameModeFlag gameMode = SINGLE, bool isUnsync = true):
			  configFolder(configFolder), gameMode(gameMode), isUnsync(isUnsync)
			{
				core = INITIALIZE_DIVA_CORE;
			}

			CorePtr get()
			{
				registerComponents();
				registerGameMode();
				registerStates();

				//load config
				core->prepare(configFolder+"/common.json");
				EVALUATE_STRATEGY_PTR->prepare(configFolder+"/eval.json");

				return core;
			}
		};
	}
}

#endif