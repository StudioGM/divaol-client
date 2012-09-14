/*
 *  DivaCore.cpp
 *
 *  Created by Hyf042 on 1/14/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#include "DivaCore.h"
#include "SoraInputQueue.h"
#include "app/SoraGameApp.h"
#include "Utility/DivaConfigLoader.h"
#include "task/SoraTaskManager.h"
//#include "Mode/DivaRelayPlay.h"

namespace divacore
{
	CorePtr Core::Ptr = NULL;
	const float Core::NORMAL_SPEED = 1.0;
	/*
	 * Event functions
	 */

	//check all components
	bool Core::checkModules()
	{
		LOGGER->msg("Check all the components..","Core");

		if(!gameMode)
			DIVA_EXCEPTION_MODULE("missing gameMode!","DivaCore");
		if(!mapLoader)
			DIVA_EXCEPTION_MODULE("missing mapLoader!","DivaCore");
		if(!mapParser)
			DIVA_EXCEPTION_MODULE("missing mapParser!","DivaCore");
		if(!coreFlow)
			DIVA_EXCEPTION_MODULE("missing coreFlow!","DivaCore");
		if(!musicManager)
			DIVA_EXCEPTION_MODULE("missing musicManager!","DivaCore");
		if(!display)
			DIVA_EXCEPTION_MODULE("missing display!","DivaCore");
		if(!renderSystem)
			DIVA_EXCEPTION_MODULE("missing renderSystem!","DivaCore");
		if(!hookManager)
			DIVA_EXCEPTION_MODULE("missing hookManager!","DivaCore");

		LOGGER->msg("No problem!","Core");

		return true;
	}
	//register eventManager and event functions
	void Core::registerEvent()
	{
		for(EVENTHANDLERS::iterator ptr = components.begin(); ptr != components.end(); ptr++)
		{
			ptr->second->setEventManager(&eventManager);
			ptr->second->registerEvents();
		}
	}

	void Core::registerMapLoader(MapLoaderPtr mapLoader)
	{
		SAFE_DELETE(this->mapLoader);
		this->mapLoader = mapLoader;
		components["02_mapLoader"] = mapLoader;
	}

	void Core::registerMapParser(MapParserPtr mapParser)
	{
		SAFE_DELETE(this->mapParser);
		this->mapParser = mapParser;
		mapParser->setCore(this);
		components["03_mapParser"] = mapParser;
	}
	void Core::registerCoreFlow(CoreFlowPtr coreFlow)
	{
		SAFE_DELETE(this->coreFlow);
		this->coreFlow = coreFlow;
		coreFlow->setCore(this);
		components["09_coreFlow"] = coreFlow;
	}
	void Core::registerItemFactory(ItemFactoryPtr itemFactory)
	{
		SAFE_DELETE(this->itemFactory);
		this->itemFactory = itemFactory;
		components["05_itemFactory"] = itemFactory;
	}
	void Core::registerMusicManager(MusicManagerPtr musicManager)
	{
		SAFE_DELETE(this->musicManager);
		this->musicManager = musicManager;
		components["06_musicManager"] = musicManager;
	}
	void Core::registerDisplay(DisplayPtr display)
	{
		SAFE_DELETE(this->display);
		this->display = display;
		components["07_display"] = display;
	}
	void Core::registerRenderSystem(RenderSystemPtr renderSystem)
	{
		SAFE_DELETE(this->renderSystem);
		this->renderSystem = renderSystem;
		components["08_renderSystem"] = renderSystem;
	}
	void Core::registerGameMode(GameModePtr gameMode)
	{
		SAFE_DELETE(this->gameMode);
		this->gameMode = gameMode;
		components["04_gameMode"] = gameMode;
	}
	void Core::registerInputManager(InputManagerPtr inputManager)
	{
		SAFE_DELETE(this->inputManager);
		this->inputManager = inputManager;
		components["10_inputManager"] = inputManager;
	}
	void Core::registerHookManager(HookManagerPtr hookManager)
	{
		SAFE_DELETE(this->hookManager);
		this->hookManager = hookManager;
		components["11_hookManagers"] = hookManager;
	}
	void Core::registerUIPainter(UIPainterPtr uiPainter)
	{
		SAFE_DELETE(this->uiPainter);
		this->uiPainter = uiPainter;
		components["12_uiPainter"] = uiPainter;
	}
	void Core::registerGameModule(GameModulePtr gameModule)
	{
		SAFE_DELETE(this->gameModule);
		this->gameModule = gameModule;
		components["01_gameModule"] = gameModule;
	}
	void Core::registerEffectSystem(EffectSystemPtr effectSystem)
	{
		SAFE_DELETE(this->effectSystem);
		this->effectSystem = effectSystem;
		components["13_effectSystem"] = effectSystem;
	}
	void Core::registerNetworkSystem(GNetworkSystemPtr networkSystem)
	{
		SAFE_DELETE(this->networkSystem);
		this->networkSystem = networkSystem;
		components["14_networkSystem"] = networkSystem;
	}
	void Core::registerEvaluateStrategy(EvaluateStrategyPtr evaluateStrategy)
	{
		SAFE_DELETE(this->evaluateStrategy);
		this->evaluateStrategy = evaluateStrategy;
		components["15_evaluateStrategy"] = evaluateStrategy;
	}

	/*
	 * State functions
	 */
    Core& Core::addState(CoreStatePtr state, const std::string& tag) 
	{
		state->setCore(this);
		state->setEventManager(&eventManager);

        mFSMManager.defState(state, tag);
		
        return *this;
    } 
    
    CoreStatePtr Core::getState(const std::string& tag) const 
	{
        CoreStatePtr state = static_cast<CoreStatePtr>(mFSMManager[tag]);
        return state;
    }

	void Core::setState(const std::string& tag) 
	{
        SoraFSMState* state = mFSMManager.getCurrentState();
        //if(state)
        //    SoraKeyPoll::DelInputListener(static_cast<SoraGameState*>(state));
         
        mFSMManager.switchToState(tag);
        
        state = mFSMManager.getCurrentState();
        //if(state)
        //    SoraKeyPoll::AddInputListener(static_cast<SoraGameState*>(state));
    }

	/*
	 * Event handlers functions
	 */
	void Core::prepare(const std::string &configFile)
	{
		configloader::loadWithJson(config,configFile);
		if(config.has("fps"))
			sora::SoraCore::Ptr->setFPS(config.getAsDouble("fps"));
		
		RENDER_SYSTEM_PTR->prepare(config);
	}

	void Core::init()
	{
		sora::SoraCore::Ptr->setFPS(60);

		LOGGER->msg("Initialize","Core");
	}

	void Core::gameReset()
	{
		LOGGER->msg("Game Set On","Core");
		
		//reset speed scale
		setSpeedScale();
		MUSIC_MANAGER_PTR->setSpeedScale();
	}

	void Core::gameStop()
	{
		LOGGER->msg("Game Stop","Core");
	}

	/*
	 * Game functions
	 */
	void Core::onStart()
	{
		// pre start
		gameMode->preStart();

		for(EVENTHANDLERS::iterator ptr = components.begin(); ptr != components.end(); ptr++)
			ptr->second->gameStart();

		// after start
		gameMode->afterStart();
	}
	void Core::onInitiate()
	{
		components["0_core"] = this;

		checkModules();
		registerEvent();
		
		for(EVENTHANDLERS::iterator ptr = components.begin(); ptr != components.end(); ptr++)
			ptr->second->init();

		//add Game Mode hook
		gameMode->addHookIn();
	}

	void Core::onDestroy()
	{
		//onLeave(); //first leave
		for(EVENTHANDLERS::reverse_iterator ptr = components.rbegin(); ptr != components.rend(); ptr++)
			ptr->second->destroy();
	}
	void Core::onEnter()
	{
		for(EVENTHANDLERS::iterator ptr = components.begin(); ptr != components.end(); ptr++)
			ptr->second->gameReset();
		setState(initStateTag);
	}
	void Core::onLeave()
	{
		for(EVENTHANDLERS::reverse_iterator ptr = components.rbegin(); ptr != components.rend(); ptr++)
			ptr->second->gameStop();
	}
	void Core::onRender()
	{
		//renderSystem->draw();
		SoraSprite *sprite = renderToCanvas(SoraCore::Instance()->getScreenWidth(),SoraCore::Instance()->getScreenHeight());

		//render to screen
		sora::SoraGameApp::BeginScene();

		if(sprite)
		{
			sprite->setPosition(0,0);
			sprite->render();
		}
		//renderSystem->render();

		mFSMManager.onRender();

		sora::SoraGameApp::EndScene();
	}
	//void Core::holdOnConnection(float dt)
	//{
	//	((RoomState*)getGameApp()->getState("room"))->holdOnConnection(dt); //add new
	//}
	void Core::onUpdate(float dt)
	{
		//hold on room connection
		//holdOnConnection(dt);

		if(getState()==RUN||getState()==RESULT)
			for(EVENTHANDLERS::iterator ptr = components.begin(); ptr != components.end(); ptr++)
				ptr->second->update(dt);
		else if(getState()==END)
		{
			onLeave();
			setState(OVER);
		}
		else
			RENDER_SYSTEM_PTR->update(dt);

		mFSMManager.onUpdate(dt);

		char buffer[256];
		sprintf(buffer,"FPS %0.2lf",sora::SoraCore::Ptr->getFPS());
		sora::SoraCore::Ptr->setWindowTitle(buffer);
	}
	void Core::onKeyPressed(SoraKeyEvent& event)
	{
		CoreStatePtr curState = dynamic_cast<CoreStatePtr>(mFSMManager.getCurrentState());
		if(curState)
			  curState->onKeyPressed(event);

		inputManager->onKeyPressed(event);
	}
	void Core::onKeyReleased(SoraKeyEvent& event)
	{
		CoreStatePtr curState = dynamic_cast<CoreStatePtr>(mFSMManager.getCurrentState());
		if(curState)
			curState->onKeyReleased(event);

		inputManager->onKeyReleased(event);
	}
	void Core::onMouseClicked(SoraMouseEvent& event)
	{
		event.x = event.x*RENDER_SYSTEM_PTR->getGameWidth()/RENDER_SYSTEM_PTR->getWindowWidth();
		event.y = event.y*RENDER_SYSTEM_PTR->getGameHeight()/RENDER_SYSTEM_PTR->getWindowHeight();
		UI_PAINTER_PTR->onMouseClicked(event);
	}
	void Core::onMouseReleased(SoraMouseEvent& event)
	{
		event.x = event.x*RENDER_SYSTEM_PTR->getGameWidth()/RENDER_SYSTEM_PTR->getWindowWidth();
		event.y = event.y*RENDER_SYSTEM_PTR->getGameHeight()/RENDER_SYSTEM_PTR->getWindowHeight();
		UI_PAINTER_PTR->onMouseReleased(event);
	}
	void Core::onMouseMoved(SoraMouseEvent& event)
	{
		event.x = event.x*RENDER_SYSTEM_PTR->getGameWidth()/RENDER_SYSTEM_PTR->getWindowWidth();
		event.y = event.y*RENDER_SYSTEM_PTR->getGameHeight()/RENDER_SYSTEM_PTR->getWindowHeight();
		UI_PAINTER_PTR->onMouseMoved(event);
	}
	void Core::restart()
	{
		onLeave();
		onEnter();
	}
	void Core::pause()
	{
		//musicManager->pause();
		display->pause();
		coreFlow->pause();
	}
	void Core::resume()
	{
		//musicManager->resume();
		display->resume();
		coreFlow->resume();
	}
	void Core::ready()
	{
		for(EVENTHANDLERS::iterator ptr = components.begin(); ptr != components.end(); ptr++)
			ptr->second->gameReady();
	}
	void Core::over()
	{
		setState(Core::END);
	}
	void Core::flowOver()
	{
		for(EVENTHANDLERS::iterator ptr = components.begin(); ptr != components.end(); ptr++)
			ptr->second->gameOver();
		UI_PAINTER_PTR->gameStop();
		DISPLAY_PTR->gameStop();
		MUSIC_MANAGER_PTR->gameStop();
		CORE_FLOW_PTR->gameStop();
		NETWORK_SYSTEM_PTR->gameStop();
	}
	void Core::render(sora::SoraSprite *sprite, const std::string &tag)
	{
		renderSystem->render(sprite,tag);
	}
	void Core::render(float x, float y, sora::SoraSprite *sprite, const std::string &tag)
	{
		renderSystem->render(x,y,sprite,tag);
	}
	sora::SoraSprite* Core::renderToCanvas(float width, float height, uint32 mask)
	{
		if(renderSystem==NULL)
			return NULL;
		return renderSystem->renderToCanvas(width,height,mask);
	}
	void Core::setSpeedScale(float scale){
		assert(musicManager!=NULL);
		mSpeedScale=scale;
		musicManager->setSpeedScale(scale);
		display->setSpeedScale(scale);
	}
	void Core::setSong(const std::wstring &path, const std::wstring &mapFileName) {mapLoader->setSong(path,mapFileName);}
	double Core::getRealTime() {return coreFlow->getRealTime();}
	double Core::getRunTime() {return coreFlow->getRunTime();}
	double Core::getRunPosition() {return coreFlow->getPosition();}
	int32 Core::getScore() {return gameMode->getScore();}
	int32 Core::getHP() {return gameMode->getHP();}
}