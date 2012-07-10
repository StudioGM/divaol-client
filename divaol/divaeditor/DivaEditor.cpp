#include "DivaEditor.h"
#include "divacore/Core/DivaCore.h"

namespace divaeditor
{
	using namespace divacore;

	

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
		mState = State::SELECTFILE;
		core->onInitiate();

	}

	void Editor::onLeave()
	{
		core->onLeave();
	}

	void Editor::onRender()
	{
		if(mState==State::SELECTFILE)
		{

		}
		else
		{
			core->onRender();
		}


		
	}

	void Editor::onUpdate(float dt)
	{
		if(mState==State::SELECTFILE)
		{

		}
		else
		{
			core->onUpdate(dt);
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



}