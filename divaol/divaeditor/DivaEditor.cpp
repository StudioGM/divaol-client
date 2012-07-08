#include "DivaEditor.h"
#include "divacore/Core/DivaCore.h"

namespace divaeditor
{
	using namespace divacore;


	//class Editor : public sora::SoraGameState, public sora::SoraEventHandler, public EventHandler
	//{
	//	friend class Core;
	//    /*
	//	 * Common Variables
	//	 */
	//	//total state
	//	int mState,mMode;
	//	//map info
	//	MapInfoPtr mapInfo;
	//	//the initialize state name
	//	std::string initStateTag;
	//	//event manager
	//	EventManager eventManager;
	//	//state manager
	//	SoraFSMManager mFSMManager;
	//	//global config
	//	Config config;

	//	CorePtr core;


	//private:
	//	Editor():core(NULL) {}
	//    /*
	//	 * event functions
	//	 */
	//	void init();
	//	void gameReset();
	//	void gameStop();
	//	bool checkModules();
	//	void registerEvent();
	//	void setState(int state) {mState=state;}
	//public:
	//    /*
	//	 * singleton
	//	 */
	//	enum State{PREPARE,MAPINFO,TIMELINE,NOTEPLACEMENT,RESOURCEPLACEMENT};

	//	typedef Editor* EditorPtr;
	//	static EditorPtr Ptr;
	//	static EditorPtr instance() {static Editor instance; return (Ptr=&instance);}

	//public:
	//	 /*
	//	  * GameState
	//	  */
	//	void onStart();
	//	void onInitiate();
	//	void onDestroy();
	//	void onEnter();
	//	void onLeave();
	//	void onRender();
	//	void onUpdate(float dt);
	//	void onKeyPressed(SoraKeyEvent& event);
	//	void onKeyReleased(SoraKeyEvent& event);
	//	void onMouseClicked(SoraMouseEvent& event);
	//	void onMouseReleased(SoraMouseEvent& event);
	//	void onMouseMoved(SoraMouseEvent& event);

	//	/*
	//	 * CoreState
	//	 */
 //       Core& addState(CoreStatePtr state, const std::string& tag);
 //       CoreStatePtr getState(const std::string& tag) const;
 //       void setState(const std::string& tag);

	//    /*
	//	 * Control funcions
	//	 */
	//	 void prepare(const std::string &configFile);
	//	 void restart();
	//	 void pause();
	//	 void resume();
	//	 void over();
	//	 void ready();
	//	 void flowOver();
	//	 void render(sora::SoraSprite *sprite, const std::string &tag = "");
	//	 void render(float x, float y, sora::SoraSprite *sprite, const std::string &tag = "");
	//	 sora::SoraSprite* renderToCanvas(float width = 0, float height = 0);

	//	/*
	//	 * set functions
	//	 */


	//	/*
	//	 * Get functions
	//	 */
	//	 int getState() {return mState;}
	//};

	EditorPtr Editor::Ptr = NULL;


	//Register Functions

	void Editor::registerDivaCore(CorePtr _core)
	{
		core = _core;
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
		core->onEnter();
	}

	void Editor::onInitiate()
	{
		
	}

	void Editor::onLeave()
	{
		core->onLeave();
	}

	void Editor::onRender()
	{
		core->onRender();
	}

	void Editor::onUpdate(float dt)
	{
		core->onUpdate(dt);
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



}