#include "divaeditor/Core/DivaEditor.h"
#include "divaeditor/Component/DivaEditorDisplay.h"
#include "divaeditor/Core/DivaEditorLocalization.h"

#include "divacore/state/DivaCoreLoader.h"
#include "divacore/Component/DivaStandardCoreFlow.h"

#include "app/SoraGameApp.h"


namespace divaeditor
{
	using namespace divacore;

#ifdef WIN32
	WNDPROC	g_lpLastEditorWndProc = 0;
#endif
	

	EditorPtr Editor::Ptr = NULL;


	//Register Functions

	void Editor::coreDidLoad(void* arg)
	{
		mapData->registerMapInfo(CORE_PTR->getMapInfo());
		StandardEditUtility::instance().init();
		CORE_PTR->setState("play");
		CORE_PTR->pause();
		changeScene(State::MAIN);
	}


	void Editor::initDivaCoreForEditor()
	{
		Task task;
		task.setAsMemberFunc(&Editor::coreDidLoad, this);
		((divacore::CoreLoader*)((*CORE_PTR->getManager())["load"]))->registerReadyCallback(task);

		CORE_PTR->registerDisplay(new DivaEditorDisplay());
	}

	void Editor::setSongInitialized()
	{
		songInitialized = true;
	}

	//Editor public GameState Functions
	void Editor::onStart()
	{
		
	}

	void Editor::onDestroy()
	{
		CORE_PTR->onDestroy();
	}

	void Editor::onEnter()
	{
		initEditorProc();

		SoraCore::ClearGlobalHotkeys();

		SoraCore::Instance()->showMouse(true);
		GCN_GLOBAL->initGUIChan(LOCALIZATION->getLocalFontPath().c_str(),14);

		initScenes();

		if(!mapData)
			mapData = new DivaEditorMapData();

		changeScene(State::PRELOAD);
	}

	void Editor::onInitiate()
	{
		CORE_PTR->onInitiate();
	}

	void Editor::onLeave()
	{
		CORE_PTR->onLeave();
	}

	void Editor::onRender()
	{
		
		
		SoraSprite* coreDrawSprite=NULL;
		if(mState==State::MAIN)
		{
			

			uint32 renderMask = 0;
			if(EDITCONFIG->display_background)
				renderMask = renderMask | divacore::RenderSystem::RS_RENDER_BACKGROUND;
			if(EDITCONFIG->display_note)
				renderMask = renderMask | divacore::RenderSystem::RS_RENDER_NOTE;
			renderMask = renderMask | divacore::RenderSystem::RS_RENDER_UI | divacore::RenderSystem::RS_RENDER_EFFECT;


			coreDrawSprite = CORE_PTR->renderToCanvas(SoraCore::Instance()->getScreenWidth(),
				SoraCore::Instance()->getScreenHeight(),renderMask);

			
			coreDrawSprite->setPosition(0,0);
			coreDrawSprite->render();
		}

		sora::SoraGameApp::BeginScene();
		if(coreDrawSprite)
			coreDrawSprite->render();
		GCN_GLOBAL->gcnDraw();
		sora::SoraGameApp::EndScene();
	}

	void Editor::onUpdate(float dt)
	{
		SoraCore::Instance()->showMouse(true);
		if(songInitialized)
			CORE_PTR->onUpdate(dt);
		
		if(scenes.find(mState)!=scenes.end())
			scenes[mState]->onUpdate(dt);
	}

	void Editor::onKeyPressed(SoraKeyEvent& event)
	{
		if(songInitialized)
			CORE_PTR->onKeyPressed(event);
		if(scenes.find(mState)!=scenes.end())
			scenes[mState]->onKeyPressed(event);
	}

	void Editor::onKeyReleased(SoraKeyEvent& event)
	{
		if(songInitialized)
			CORE_PTR->onKeyReleased(event);
		if(scenes.find(mState)!=scenes.end())
			scenes[mState]->onKeyReleased(event);
	}

	void Editor::onMouseClicked(SoraMouseEvent& event)
	{
		if(songInitialized)
			CORE_PTR->onMouseClicked(event);
		if(scenes.find(mState)!=scenes.end())
			scenes[mState]->onMouseClicked(event);
	}

	void Editor::onMouseReleased(SoraMouseEvent& event)
	{
		if(songInitialized)
			CORE_PTR->onMouseReleased(event);
		if(scenes.find(mState)!=scenes.end())
			scenes[mState]->onMouseReleased(event);
	}

	void Editor::onMouseMoved(SoraMouseEvent& event)
	{
		if(songInitialized)
			CORE_PTR->onMouseMoved(event);
		if(scenes.find(mState)!=scenes.end())
			scenes[mState]->onMouseMoved(event);
	}

	void Editor::onMouseWheelUp(SoraMouseEvent& event)
	{
		if(scenes.find(mState)!=scenes.end())
			scenes[mState]->onMouseWheelUp(event);
	}

	void Editor::onMouseWheelDown(SoraMouseEvent& event)
	{
		if(scenes.find(mState)!=scenes.end())
			scenes[mState]->onMouseWheelDown(event);
	}


	void Editor::initScenes()
	{
		scenes[State::MAIN] = new DivaEditorMainScene();
		scenes[State::PRELOAD] = new DivaEditorInitScene();
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
		mState = (State)nowScene->sceneIndex;
	}

	void Editor::onSceneAppeared(DivaEditorScene* sender)
	{

	}

#ifdef WIN32
	LRESULT CALLBACK EditorWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
	{

		if(nMsg==WM_CLOSE)
		{
			if(EDITCONFIG->isMapChanged)
			{
				int msgBoxResult = MessageBoxW(hWnd,LOCALIZATION->getLocalStr(L"CloseApp_MapModifiedNotification").c_str(),
												LOCALIZATION->getLocalStr(L"CloseApp_MapModified").c_str()
												,MB_YESNOCANCEL);
				if(msgBoxResult == IDYES)
					EDITOR_PTR->mapData->SaveFile();
				else if(msgBoxResult == IDCANCEL)
					return(0);
			}
		}

		return g_lpLastEditorWndProc ? g_lpLastEditorWndProc(hWnd, nMsg, wParam, lParam): TRUE;
	}
#endif

	void Editor::initEditorProc()
	{
#ifdef WIN32
		HWND hWnd = (HWND)sora::SoraCore::Instance()->getMainWindowHandle();
		if (!hWnd) throw NULL;

		if (g_lpLastEditorWndProc == NULL) {
			LONG prevWndProc = ::GetWindowLong(hWnd, GWL_WNDPROC);
			LONG wndProc = ::SetWindowLong(hWnd,GWL_WNDPROC,(LONG)EditorWndProc);

			g_lpLastEditorWndProc = (WNDPROC)(prevWndProc);
		}
#endif
	}
}