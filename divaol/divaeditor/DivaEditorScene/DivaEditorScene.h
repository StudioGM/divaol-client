#ifndef DIVA_EDITOR_SCENE
#define DIVA_EDITOR_SCENE

#include "divaeditor/DivaEditor.h"
#include "divacore/core/DivaCore.h"

#include "guichansetup.h"
#include "gcnPanel.h"

#include "soraguichan/SoraGUI.h"
#include "Soraguichan/SoraGUIResponserMap.h"

namespace divaeditor
{
	using namespace divacore;
	using namespace sora;
	using namespace gcn;


#define sWidth SoraCore::Instance()->getScreenWidth()

	class DivaEditorScene
	{	
	protected:
		Container *top;
		Container *black;

	public:
		int sceneIndex;

		//EventHandlers
		SoraFunction<void(DivaEditorScene*)> Disappeared;
		SoraFunction<void(DivaEditorScene*)> Appeared;

		virtual void willAppear() = NULL;
		virtual void didAppear(gcn::Widget *widget) = NULL;
		virtual void willDisappear() = NULL;
		virtual void didDisappear(gcn::Widget *widget) = NULL;

		virtual void onRender() = NULL;
		virtual void onUpdate() = NULL;
		
		//Input events
		virtual void onKeyPressed(SoraKeyEvent& event) = NULL;
		virtual void onKeyReleased(SoraKeyEvent& event) = NULL;
		virtual void onMouseClicked(SoraMouseEvent& event) = NULL;
		virtual void onMouseReleased(SoraMouseEvent& event) = NULL;
		virtual void onMouseMoved(SoraMouseEvent& event) = NULL;
	};



	class DivaEditorCategorySelectScene : public DivaEditorScene, public sora::SoraGUIResponser
	{
	private:
		gcn::Button *btn_newFile, *btn_openFile;

		gcn::Button *btn_play, *btn_pause, *btn_stop;

		gcn::Button *btn_gotoTimeLine, *btn_gotoNote, *btn_gotoResource, *btn_Release;

	public:
		DivaEditorCategorySelectScene();

		virtual void willAppear();
		virtual void didAppear(gcn::Widget *widget);
		virtual void willDisappear();
		virtual void didDisappear(gcn::Widget *widget);

		virtual void action();

		virtual void onRender(){};
		virtual void onUpdate(){};

		//Input events
		virtual void onKeyPressed(SoraKeyEvent& event){};
		virtual void onKeyReleased(SoraKeyEvent& event){};
		virtual void onMouseClicked(SoraMouseEvent& event){};
		virtual void onMouseReleased(SoraMouseEvent& event){};
		virtual void onMouseMoved(SoraMouseEvent& event){};
	};


	class DivaEditorEditTimelineScene : public DivaEditorScene
	{

	};

	class DivaEditorEditNoteScene : public DivaEditorScene
	{

	};

	class DivaEditorEditResourceScene : public DivaEditorScene
	{

	};
}




#endif