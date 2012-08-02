#include "DivaEditor.h"
#include "divacore/Core/DivaCore.h"

#include "app/SoraGameApp.h"

#include "DivaEditorScene/DivaEditorScene.h"
#include "DivaEditorDisplay.h "
#include "divacore/state/DivaCoreLoader.h"
#include "divacore/Component/DivaStandardCoreFlow.h"
#include "divaeditor/DivaEditorMapData.h"

namespace divaeditor
{
	using namespace divacore;

	

	EditorPtr Editor::Ptr = NULL;


	//Register Functions

	void Editor::coreDidLoad(void* arg)
	{
		if(!mapData)
			mapData = new DivaEditorMapData();
		mapData->registerMapInfo(core->getMapInfo());
		StandardEditUtility::instance().init();
		core->setState("play");
		core->pause();
		changeScene(State::MAIN);
	}


	void Editor::registerDivaCore(CorePtr _core)
	{
		core = _core;
		Task task;
		task.setAsMemberFunc(&Editor::coreDidLoad, this);
		
		((divacore::CoreLoader*)((*core->getManager())["load"]))->registerReadyCallback(task);

/*
		Task endTask;
		endTask.setAsMemberFunc(&Editor::coreDidEnd, this);

		CORE_FLOW_PTR->registerEndCallback(endTask);
		*/

		core->registerDisplay(new DivaEditorDisplay());
	}



	

	//Editor public GameState Functions
	void Editor::onStart()
	{
		core->onStart();
	}

	void Editor::onDestroy()
	{
		core->onDestroy();
	}

	void Editor::onEnter()
	{
		core->setSong("song/Ç§±¾—@","Ç§±¾—@_Hard.divaol");

		SoraCore::Instance()->showMouse(true);
		GCN_GLOBAL->initGUIChan(L"arial.ttf",14);

		initScenes();
		core->onEnter();

		//gcn::Button* button = new gcn::Button("lalalala");
		//button->setPosition(sora::SoraCore::Instance()->getScreenWidth()/2, 100);
		//button->setSize(100, 100);

		//gcn::Window* wnd = new gcn::Window("hahahaa");
		//wnd->add(button);
		//wnd->setDimension(gcn::Rectangle(0, 0, 1000, 1000));

		//GCN_GLOBAL->addWidget(button,NULL);
		//GCN_GLOBAL->setGlobalForegroundColor(Color::White);
	}

	void Editor::onInitiate()
	{
		core->onInitiate();
	}

	void Editor::onLeave()
	{
		core->onLeave();
	}

	void Editor::onRender()
	{
		SoraSprite* coreDrawSprite = core->renderToCanvas(SoraCore::Instance()->getScreenWidth(),
			SoraCore::Instance()->getScreenHeight());

		sora::SoraGameApp::BeginScene();
		coreDrawSprite->setPosition(0,0);
		coreDrawSprite->render();

		if(mState==State::MAIN)
		{
			GCN_GLOBAL->gcnDraw();
		}

		sora::SoraGameApp::EndScene();
	}

	void Editor::onUpdate(float dt)
	{
		core->onUpdate(dt);
		if(mState==State::MAIN)
		{
			SoraCore::Instance()->showMouse(true);
		}
		
	}

	void Editor::onKeyPressed(SoraKeyEvent& event)
	{
		core->onKeyPressed(event);
	}

	void Editor::onKeyReleased(SoraKeyEvent& event)
	{
		core->onKeyReleased(event);
	}

	void Editor::onMouseClicked(SoraMouseEvent& event)
	{
		core->onMouseClicked(event);
	}

	void Editor::onMouseReleased(SoraMouseEvent& event)
	{
		core->onMouseReleased(event);
	}

	void Editor::onMouseMoved(SoraMouseEvent& event)
	{
		core->onMouseMoved(event);
	}

	void Editor::onMouseWheelUp(SoraMouseEvent& event)
	{

	}

	void Editor::onMouseWheelDown(SoraMouseEvent& event)
	{

	}


	void Editor::initScenes()
	{
		scenes[State::MAIN] = new DivaEditorMainScene();
	}


	void Editor::changeScene(State sceneState)
	{
		if(nowScene!=NULL && nowScene->sceneIndex!=sceneState)
		{
			nextScene = scenes[sceneState];
			nowScene->Disappeared = Bind(this, &Editor::onSceneDisappeared);
			nowScene->willDisappear();
		}
		else if(nowScene==NULL)
		{
			nextScene = scenes[sceneState];
			onSceneDisappeared(NULL);
		}
	}

	void Editor::onSceneDisappeared(DivaEditorScene* sender)
	{
		nowScene = nextScene;
		nowScene->Appeared = Bind(this, &Editor::onSceneAppeared);
		nowScene->willAppear();
		mState = nowScene->sceneIndex;
	}

	void Editor::onSceneAppeared(DivaEditorScene* sender)
	{

	}
}