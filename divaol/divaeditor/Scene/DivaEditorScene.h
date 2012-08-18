#ifndef DIVA_EDITOR_SCENE
#define DIVA_EDITOR_SCENE

#include<map>

#include "divaeditor/DivaEditor.h"
#include "divacore/core/DivaCore.h"

#include "guichansetup.h"

#include "soraguichan/SoraGUI.h"
#include "Soraguichan/SoraGUIResponserMap.h"

#include "divaeditor/Widgets/DivaEditorTimelineWidget.h"
#include "divaeditor/Widgets/DivaEditorMusicProgressWidget.h"
#include "divaeditor/Widgets/DivaEditorNoteArea.h"
#include "divaeditor/Widgets/DivaEditorResourcePanel.h"

#include "divaeditor/Widgets/BoarderedContainer.h"
#include "divaeditor/Widgets/WListBox.h"
#include "divaeditor/Widgets/WTextfield.h"
#include "divaeditor/Widgets/WLabel.h"
#include "divaeditor/Widgets/WCheckbox.h"

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
		virtual void onUpdate(float dt) = NULL;
		
		//Input events
		virtual void onKeyPressed(SoraKeyEvent& event) = NULL;
		virtual void onKeyReleased(SoraKeyEvent& event) = NULL;
		virtual void onMouseClicked(SoraMouseEvent& event) = NULL;
		virtual void onMouseReleased(SoraMouseEvent& event) = NULL;
		virtual void onMouseMoved(SoraMouseEvent& event) = NULL;
		virtual void onMouseWheelUp(SoraMouseEvent& event) = NULL;
		virtual void onMouseWheelDown(SoraMouseEvent& event) = NULL;
	};

	class DivaEditorInitScene :
		public DivaEditorScene,
		public sora::SoraGUIResponser
	{

	public:
		DivaEditorInitScene();

		virtual void willAppear();
		virtual void didAppear(gcn::Widget *widget);
		virtual void willDisappear();
		virtual void didDisappear(gcn::Widget *widget);

		virtual void action();

		virtual void onRender(){};
		virtual void onUpdate(float dt);

		//Input events
		virtual void onKeyPressed(SoraKeyEvent& event){};
		virtual void onKeyReleased(SoraKeyEvent& event){};
		virtual void onMouseClicked(SoraMouseEvent& event){};
		virtual void onMouseReleased(SoraMouseEvent& event){};
		virtual void onMouseMoved(SoraMouseEvent& event){};
		virtual void onMouseWheelUp(SoraMouseEvent& event){};
		virtual void onMouseWheelDown(SoraMouseEvent& event){};

	};

	class DivaEditorMainScene : 
		public DivaEditorScene, 
		public sora::SoraGUIResponser, 
		public gcn::SelectionListener
	{
	public:
		enum State{TIMELINE,NOTE,SHOW,PREVIEW};

	private:
		State nowState;

		gcn::Container *container_ToolBar;


		std::map<State,gcn::Container*> container_Categories;

		gcn::Container* initTimelineCategory();
		gcn::Container* initNoteCategory();
		gcn::Container* initShowCategory();
		gcn::Container* initPlayCategory();

		void refreshResourcePanelDetail();
		void refreshEventDetail();

		void flowDidEnd(void* arg);

	public:

		DivaEditorMainScene();

		void ChangeState(State state);

		virtual void willAppear();
		virtual void didAppear(gcn::Widget *widget);
		virtual void willDisappear();
		virtual void didDisappear(gcn::Widget *widget);

		virtual void action();

		virtual void onRender(){};
		virtual void onUpdate(float dt);

		//Input events
		virtual void onKeyPressed(SoraKeyEvent& event);
		virtual void onKeyReleased(SoraKeyEvent& event);
		virtual void onMouseClicked(SoraMouseEvent& event){};
		virtual void onMouseReleased(SoraMouseEvent& event){};
		virtual void onMouseMoved(SoraMouseEvent& event){};
		virtual void onMouseWheelUp(SoraMouseEvent& event);
		virtual void onMouseWheelDown(SoraMouseEvent& event);

		//Selection events
		virtual void valueChanged(const gcn::SelectionEvent &event);
	};

}




#endif