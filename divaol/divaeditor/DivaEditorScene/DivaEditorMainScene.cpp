#include "divaeditor/DivaEditorScene/DivaEditorScene.h"
#include "Animation/SoraGUIAnimation.h"
#include "divacore/Component/DivaStandardCoreFlow.h"
#include "divaeditor/DivaEditorScene/DivaEditorTimelineWidget.h"
#include "divaeditor/DivaEditorScene/DivaEditorMusicProgressWidget.h"

namespace divaeditor
{
	using namespace gcn;

#define EditUtility divacore::StandardEditUtility::instance()

	DivaEditorMainScene::DivaEditorMainScene()
	{
		sceneIndex = Editor::State::MAIN;

		//Init Scene
		top = new Container();
		top->setBaseColor(gcn::Color(0,0,0,0));
		top->setSize(1920,1080);



		//Tool Bar
		container_ToolBar = new Container();
		container_ToolBar->setSize(500,40);
		container_ToolBar->setBaseColor(gcn::Color(34,137,227,255));

		//Three Category Buttons
		gcn::Button *btn_TimeLine = new gcn::Button("Timeline");
		btn_TimeLine->setId("btn_TimeLine");
		btn_TimeLine->setSize(120,30);
		btn_TimeLine->setPosition(5,5);
		btn_TimeLine->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_TimeLine, this, "btn_TimeLine", sora::RESPONSEACTION);
		container_ToolBar->add(btn_TimeLine);
		
		gcn::Button *btn_Note = new gcn::Button("Note");
		btn_Note->setId("btn_Note");
		btn_Note->setSize(120,30);
		btn_Note->setPosition(130,5);
		btn_Note->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_Note, this, "btn_Note", sora::RESPONSEACTION);
		container_ToolBar->add(btn_Note);

		gcn::Button *btn_Show = new gcn::Button("Show");
		btn_Show->setId("btn_Show");
		btn_Show->setSize(120,30);
		btn_Show->setPosition(255,5);
		btn_Show->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_Show, this, "btn_Show", sora::RESPONSEACTION);
		container_ToolBar->add(btn_Show);


		//Play Control Buttons
		gcn::Button *btn_Play = new gcn::Button("Play");
		btn_Play->setId("btn_Play");
		btn_Play->setSize(30,30);
		btn_Play->setPosition(400,5);
		btn_Play->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_Play, this, "btn_Play", sora::RESPONSEACTION);
		container_ToolBar->add(btn_Play);

		gcn::Button *btn_Pause = new gcn::Button("Pause");
		btn_Pause->setId("btn_Pause");
		btn_Pause->setSize(30,30);
		btn_Pause->setPosition(432,5);
		btn_Pause->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_Pause, this, "btn_Pause", sora::RESPONSEACTION);
		container_ToolBar->add(btn_Pause);

		gcn::Button *btn_Stop = new gcn::Button("Stop");
		btn_Stop->setId("btn_Stop");
		btn_Stop->setSize(30,30);
		btn_Stop->setPosition(464,5);
		btn_Stop->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_Stop, this, "btn_Stop", sora::RESPONSEACTION);
		container_ToolBar->add(btn_Stop);


		top->add(container_ToolBar);



		//Init Timeline
		gcn::Container *timelineCategory = new gcn::Container();
		timelineCategory->setSize(1280,720);
		timelineCategory->setOpaque(true);
		timelineCategory->setBaseColor(gcn::Color(0,0,0,0));
		
		Timeline *timeline = new Timeline();
		timeline->setId("timeline_TimeLine");
		timeline->setSize(500,40);
		timeline->setPosition(700,0);
		timeline->setBackgroundColor(gcn::Color(0,0,0,0));
		timeline->setForegroundColor(gcn::Color(255,255,255,255));
		timelineCategory->add(timeline);
		
		gcn::Button *btn_TimeLine_wider = new gcn::Button("-");
		btn_TimeLine_wider->setId("btn_TimeLine_wider");
		btn_TimeLine_wider->setSize(20,20);
		btn_TimeLine_wider->setPosition(1205,0);
		btn_TimeLine_wider->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_TimeLine_wider, this, "btn_TimeLine_wider", sora::RESPONSEACTION);
		timelineCategory->add(btn_TimeLine_wider);

		gcn::Button *btn_TimeLine_tighter = new gcn::Button("+");
		btn_TimeLine_tighter->setId("btn_TimeLine_tighter");
		btn_TimeLine_tighter->setSize(20,20);
		btn_TimeLine_tighter->setPosition(1205,25);
		btn_TimeLine_tighter->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_TimeLine_tighter, this, "btn_TimeLine_tighter", sora::RESPONSEACTION);
		timelineCategory->add(btn_TimeLine_tighter);

		gcn::Button *btn_TimeLine_deeper = new gcn::Button("¡ý");
		btn_TimeLine_deeper->setId("btn_TimeLine_deeper");
		btn_TimeLine_deeper->setSize(20,20);
		btn_TimeLine_deeper->setPosition(1230,0);
		btn_TimeLine_deeper->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_TimeLine_deeper, this, "btn_TimeLine_deeper", sora::RESPONSEACTION);
		timelineCategory->add(btn_TimeLine_deeper);

		gcn::Button *btn_TimeLine_lighter = new gcn::Button("¡ü");
		btn_TimeLine_lighter->setId("btn_TimeLine_lighter");
		btn_TimeLine_lighter->setSize(20,20);
		btn_TimeLine_lighter->setPosition(1230,25);
		btn_TimeLine_lighter->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_TimeLine_lighter, this, "btn_TimeLine_lighter", sora::RESPONSEACTION);
		timelineCategory->add(btn_TimeLine_lighter);




		container_Categories[State::TIMELINE] = timelineCategory;


		top->add(timelineCategory);
		top->moveToBottom(timelineCategory);





		//Add Total Time Progress
		DivaEditorMusicProgressWidget *progressBar = new DivaEditorMusicProgressWidget();
		progressBar->setId("progressBar");
		progressBar->setSize(500,30);
		progressBar->setPosition(50,680);
		progressBar->setBackgroundColor(gcn::Color(0,0,0,0));
		progressBar->setForegroundColor(gcn::Color(255,255,255,255));
		top->add(progressBar);
	}

	void DivaEditorMainScene::willAppear()
	{
		top->setEnabled(true);
		GCN_GLOBAL->setTop(top);

		didAppear(top);
	}

	void DivaEditorMainScene::didAppear(gcn::Widget *widget)
	{
		if(Appeared)
			Appeared(this);
	}

	void DivaEditorMainScene::willDisappear()
	{
		
	}

	void DivaEditorMainScene::didDisappear(gcn::Widget *widget)
	{
		if(Disappeared)
			Disappeared(this);
	}


	void DivaEditorMainScene::action() 
	{
		if(getID()=="btn_Play")
		{
			if(CORE_FLOW_PTR->getState()==CoreFlow::PAUSE)
				CORE_PTR->resume();
			else if(CORE_FLOW_PTR->getState()==CoreFlow::RUN)
				EditUtility.setPosition(0);
			
		}
		else if(getID()=="btn_Pause")
		{
			if(CORE_FLOW_PTR->getState() == CoreFlow::RUN)
				CORE_PTR->pause();
			else if(CORE_FLOW_PTR->getState() == CoreFlow::PAUSE)
				CORE_PTR->resume();
		}
		else if(getID()=="btn_Stop")
		{
			CORE_PTR->pause();
			EditUtility.setPosition(0);
		}


		else if(getID() == "btn_TimeLine") 
		{
			EDITOR_PTR->mapData->setStop(CORE_PTR->getRunPosition(),72);
			
		}
		else if(getID() == "btn_Note")
		{
			
		}

		else if(getID() == "btn_TimeLine_wider")
		{
			((Timeline*)container_Categories[State::TIMELINE]->findWidgetById("timeline_TimeLine"))->increaseShowRangeScale();
		}
		else if(getID() == "btn_TimeLine_tighter")
		{
			((Timeline*)container_Categories[State::TIMELINE]->findWidgetById("timeline_TimeLine"))->decreaseShowRangeScale();
		}
		else if(getID() == "btn_TimeLine_deeper")
		{
			((Timeline*)container_Categories[State::TIMELINE]->findWidgetById("timeline_TimeLine"))->increaseGridToShowPerBeat();
		}
		else if(getID() == "btn_TimeLine_lighter")
		{
			((Timeline*)container_Categories[State::TIMELINE]->findWidgetById("timeline_TimeLine"))->decreaseGridToShowPerBeat();
		}
	}
}