/*
 *  DivaCore.h
 *
 *  Created by Hyf042 on 1/14/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_CORE
#define DIVA_CORE

#include "DivaCommon.h"

#include "DivaMapLoader.h"
#include "DivaMapParser.h"
#include "DivaCoreFlow.h"
#include "DivaFactory.h"
#include "DivaMusicManager.h"
#include "DivaDisplay.h"
#include "DivaRenderSystem.h"
#include "DivaGameMode.h"
#include "DivaInputManager.h"
#include "DivaHookManager.h"
#include "DivaUIPainter.h"
#include "DivaGameModule.h"
#include "DivaEffectSystem.h"
#include "Network/DivaNetworkSystem.h"
#include "DivaEvaluateStrategy.h"
#include "DivaEventManager.h"
#include "DivaParticleSystem.h"

#include "DivaConfig.h"
#include "SoraFSM.h"
#include "SoraInputListeners.h"
#include "SoraCanvas.h"
#include "DivaCoreState.h"
#include "app/SoraGameState.h"

#include "network/DivaGNetSystem.h"

namespace divacore
{
	using namespace sora;

	class CoreState;
	class MapLoader;
	class MapParser;
	class CoreFlow;
	class ItemFacotry;
	class NetworkSystem;
	typedef CoreState* CoreStatePtr;
	typedef MapLoader* MapLoaderPtr;
	typedef MapParser* MapParserPtr;
	typedef CoreFlow* CoreFlowPtr;
	typedef ItemFactory* ItemFactoryPtr;
	typedef NetworkSystem* NetworkSystemPtr;

	/*
	Core
	manage the flow of core game, and plugins
	*/
	class Core : public sora::SoraGameState, public sora::SoraEventHandler, public EventHandler
	{
	public:
		friend class CoreState;

		static const float NORMAL_SPEED;

	private:
	    /*
		 * Common Variables
		 */
		//total state
		int mState,mMode;
		//speed scale
		float mSpeedScale;
		//map info
		MapInfoPtr mapInfo;
		//the initialize state name
		std::string initStateTag;
		//event manager
		EventManager eventManager;
		//state manager
		SoraFSMManager mFSMManager;
		//global config
		Config config;

	    /*
		 * Components
		 */
		typedef std::map<std::string,EventHandler*> EVENTHANDLERS;

		//list of all components
		EVENTHANDLERS components;

		MapLoaderPtr mapLoader;
		MapParserPtr mapParser;
		CoreFlowPtr coreFlow;
		ItemFactoryPtr itemFactory;
		MusicManagerPtr musicManager;
		RenderSystemPtr renderSystem;
		GameModePtr gameMode;
		InputManagerPtr inputManager;
		DisplayPtr display;
		HookManagerPtr hookManager;
		UIPainterPtr uiPainter;
		GameModulePtr gameModule;
		EffectSystemPtr effectSystem;
		GNetworkSystemPtr networkSystem;
		EvaluateStrategyPtr evaluateStrategy;

	private:
		Core():initStateTag(""),mapLoader(NULL),mapParser(NULL),effectSystem(NULL),mState(INIT),mMode(SINGLE) {}
	    /*
		 * event functions
		 */
		void init();
		void gameReset();
		void gameStop();
		bool checkModules();
		void registerEvent();
		void setState(int state) {mState=state;}
	public:
	    /*
		 * singleton
		 */
		enum State{INIT,PREPARE,RUN,RESULT,END,OVER};
		enum Mode{SINGLE,MULTI,EDIT};

		typedef Core* CorePtr;
		static CorePtr Ptr;
		static CorePtr instance() {static Core instance; return (Ptr=&instance);}
	    /*
		 * register and get
		 */
		void registerMapLoader(MapLoaderPtr mapLoader);
		void registerMapParser(MapParserPtr mapParser);
		void registerCoreFlow(CoreFlowPtr coreFlow);
		void registerItemFactory(ItemFactoryPtr ItemFactory);
		void registerMusicManager(MusicManagerPtr musicManager);
		void registerDisplay(DisplayPtr display);
		void registerRenderSystem(RenderSystemPtr renderSystem);
		void registerGameMode(GameModePtr gameMode);
		void registerInputManager(InputManagerPtr inputManager);
		void registerHookManager(HookManagerPtr hookManager);
		void registerUIPainter(UIPainterPtr uiPainter);
		void registerGameModule(GameModulePtr gameModule);
		void registerEffectSystem(EffectSystemPtr effectSystem);
		void registerNetworkSystem(GNetworkSystemPtr networkSystem);
		void registerEvaluateStrategy(EvaluateStrategyPtr evaluateStrategy);
	//protected:
		inline MapInfoPtr getMapInfo() {return mapInfo;}
		inline MapLoaderPtr getMapLoader() {return mapLoader;}
		inline MapParserPtr getMapParser() {return mapParser;}
		inline CoreFlowPtr getCoreFlow() {return coreFlow;}
		inline ItemFactoryPtr getItemFactory() {return itemFactory;}
		inline MusicManagerPtr getMusicManager() {return musicManager;}
		inline DisplayPtr getDisplay() {return display;}
		inline RenderSystemPtr getRenderSystem() {return renderSystem;}
		inline GameModePtr getGameMode() {return gameMode;}
		inline HookManagerPtr getHookManager() {return hookManager;}
		inline UIPainterPtr getUIPainter() {return uiPainter;}
		inline GameModulePtr getGameModule() {return gameModule;}
		inline EffectSystemPtr getEffectSystem() {return effectSystem;}
		inline GNetworkSystemPtr getNetworkSystem() {return networkSystem;}
		inline EvaluateStrategyPtr getEvaluateStrategy() {return evaluateStrategy;}
	public:
		 /*
		  * GameState
		  */
		void onStart();
		void onInitiate();
		void onDestroy();
		void onEnter();
		void onLeave();
		void onRender();
		void onUpdate(float dt);
		void onKeyPressed(SoraKeyEvent& event);
		void onKeyReleased(SoraKeyEvent& event);
		void onMouseClicked(SoraMouseEvent& event);
		void onMouseReleased(SoraMouseEvent& event);
		void onMouseMoved(SoraMouseEvent& event);

		/*
		 * CoreState
		 */
        Core& addState(CoreStatePtr state, const std::string& tag);
        CoreStatePtr getState(const std::string& tag) const;
        void setState(const std::string& tag);
		void setInitState(const std::string& tag) {initStateTag = tag;}
		void setMode(GameModePtr gameMode) {registerGameMode(gameMode);}
		void setSpeedScale(float scale=NORMAL_SPEED);
		std::string getMode() {return gameMode->getName();}
	    /*
		 * Control funcions
		 */
		 void prepare(const std::string &configFile);
		 void restart();
		 void pause();
		 void resume();
		 void over();
		 void ready();
		 void flowOver();
		 void render(sora::SoraSprite *sprite, const std::string &tag = "");
		 void render(float x, float y, sora::SoraSprite *sprite, const std::string &tag = "");
		 sora::SoraSprite* renderToCanvas(float width = 0, float height = 0, uint32 mask = RenderSystem::RS_RENDER_ALL);
		 //void holdOnConnection(float dt);

		/*
		 * set functions
		 */
		 void setSong(const std::wstring &path, const std::wstring &mapFileName = L"");

		/*
		 * Get functions
		 */
		 std::string getVersion() {return "3.0.0.0";}
		 double getRunTime();
		 double getRealTime();
		 double getRunPosition();
		 int32 getScore();
		 int32 getHP();
		 int getState() {return mState;}
		 float getSpeedScale() {
			 return mSpeedScale;}
		 SoraFSMManager* getManager() {return &mFSMManager;}
	};

	typedef Core* CorePtr;

	#define INITIALIZE_DIVA_CORE divacore::Core::instance()
	#define CORE_PTR (divacore::Core::Ptr)
	#define MAP_INFO (divacore::Core::Ptr->getMapInfo())
	#define MAP_LOADER_PTR (divacore::Core::Ptr->getMapLoader())
	#define MAP_PARSER_PTR (divacore::Core::Ptr->getMapParser())
	#define GAME_MODE_PTR (divacore::Core::Ptr->getGameMode())
	#define CORE_FLOW_PTR (divacore::Core::Ptr->getCoreFlow())
	#define ITEM_FACTORY_PTR (divacore::Core::Ptr->getItemFactory())
	#define MUSIC_MANAGER_PTR (divacore::Core::Ptr->getMusicManager())
	#define DISPLAY_PTR (divacore::Core::Ptr->getDisplay())
	#define RENDER_SYSTEM_PTR (divacore::Core::Ptr->getRenderSystem())
	#define INPUT_SYSTEM_PTR (divacore::Core::Ptr->getInputManager())
	#define HOOK_MANAGER_PTR (divacore::Core::Ptr->getHookManager())
	#define UI_PAINTER_PTR (divacore::Core::Ptr->getUIPainter())
	#define GAME_MODULE_PTR (divacore::Core::Ptr->getGameModule())
	#define EFFECT_SYSTEM_PTR (divacore::Core::Ptr->getEffectSystem())
	#define NETWORK_SYSTEM_PTR (divacore::Core::Ptr->getNetworkSystem())
	#define EVALUATE_STRATEGY_PTR (divacore::Core::Ptr->getEvaluateStrategy())
}

#endif