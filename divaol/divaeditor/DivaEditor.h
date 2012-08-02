/*
 *  DivaCore.h
 *
 *  Created by CK on 7/6/12.
 *  Copyright 2012 CK. All rights reserved.
 *
 */

#ifndef DIVA_EDITOR
#define DIVA_EDITOR

#include<algorithm>

#include "divacore/Core/DivaCommon.h"

#include "divacore/Core/DivaCore.h"
#include "divacore/Core/DivaMapInfo.h"

#include "SoraFSM.h"
#include "SoraInputListeners.h"
#include "SoraCanvas.h"
#include "app/SoraGameState.h"

#include "guichansetup.h"

#include "divaeditor/DivaEditorMapData.h"
#include "divaeditor/DivaEditorScene/DivaEditorScene.h"


namespace divaeditor
{
	using namespace sora;
	using namespace divacore;
	using namespace std;


	class Editor;
	class DivaEditorScene;
	class DivaEditorMapData;

	typedef Core* CorePtr;
	typedef Editor* EditorPtr;


	/*
	Editor
	manage all the editor functions
	*/
	class Editor : public sora::SoraGameState, public sora::SoraEventHandler, public EventHandler
	{
	public: 
		enum State{PRELOAD,MAIN};

	private:
		friend class Core;
	    /*
		 * Common Variables
		 */
		//total state
		int mState,mMode;
		//the initialize state name
		std::string initStateTag;
		//event manager
		EventManager eventManager;
		//state manager
		SoraFSMManager mFSMManager;

		


	private:
		Editor():core(NULL),nowScene(NULL),mState(PRELOAD) {};

		void coreDidLoad(void* arg);

		void registerEvent(){};
		void setState(int state) {mState=state;}

		/*
		 * Scene Manage
		 */
		DivaEditorScene *nowScene;
		DivaEditorScene *nextScene;
		map<State,DivaEditorScene*> scenes;

		void initScenes();
		void onSceneDisappeared(DivaEditorScene* sender);
		void onSceneAppeared(DivaEditorScene* sender);

	public:
		
		//map info
		DivaEditorMapData *mapData;

		/*
		 * singleton
		 */
		static EditorPtr Ptr;
		static EditorPtr instance() {static Editor instance; return (Ptr=&instance);}
		CorePtr core;

		/*
		register and get
		*/

		void registerDivaCore(CorePtr _core);

	public:
		// Scene
		void changeScene(State sceneState);

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
		 sora::SoraSprite* renderToCanvas(float width = 0, float height = 0);

		/*
		 * set functions
		 */


		/*
		 * Get functions
		 */
		 int getState() {return mState;}
	};


	#define INITIALIZE_EDITOR divaeditor::Editor::instance()
	#define EDITOR_PTR (divaeditor::Editor::Ptr)
}

#endif