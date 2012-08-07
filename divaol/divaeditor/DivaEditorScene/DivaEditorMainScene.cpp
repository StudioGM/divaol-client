#include "divaeditor/DivaEditorScene/DivaEditorScene.h"
#include "divaeditor/DivaEditorCommon.h"
#include "Animation/SoraGUIAnimation.h"
#include "divacore/Component/DivaStandardCoreFlow.h"
#include "divaeditor/DivaEditorScene/DivaEditorTimelineWidget.h"
#include "divaeditor/DivaEditorScene/DivaEditorMusicProgressWidget.h"

namespace divaeditor
{
	using namespace gcn;

#define EditUtility divacore::StandardEditUtility::instance()


	gcn::Container* DivaEditorMainScene::initTimelineCategory()
	{
		//Init Timeline Category
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


		//Timeline Panel
		gcn::Container *timelinePanel = new gcn::Container();
		timelinePanel->setId("timelinePanel");
		timelinePanel->setSize(600,500);
		timelinePanel->setPosition(340,110);
		timelineCategory->add(timelinePanel);

#pragma region BPM operation

		gcn::WTextField *txtField_timeline_BPM = new gcn::WTextField();
		txtField_timeline_BPM->enableNumericMode(true);
		txtField_timeline_BPM->setId("txtField_timeline_BPM");
		txtField_timeline_BPM->setSize(100,20);
		txtField_timeline_BPM->setPosition(100,50);
		sora::SoraGUI::Instance()->registerGUIResponser(txtField_timeline_BPM, this, "txtField_timeline_BPM", sora::RESPONSEACTION);
		timelinePanel->add(txtField_timeline_BPM);

		gcn::Label *label_timeline_bpm = new gcn::Label("Now BPM:");
		label_timeline_bpm->adjustSize();
		label_timeline_bpm->setBackgroundColor(gcn::Color(0,0,0,0));
		label_timeline_bpm->setPosition(txtField_timeline_BPM->getX()-label_timeline_bpm->getWidth()-5,
			txtField_timeline_BPM->getY() + (txtField_timeline_BPM->getHeight()-label_timeline_bpm->getHeight())/2);
		label_timeline_bpm->setForegroundColor(gcn::Color(255,255,255,255));
		timelinePanel->add(label_timeline_bpm);

		gcn::Button *btn_TimeLine_changeBPM = new gcn::Button("Change");
		btn_TimeLine_changeBPM->setId("btn_TimeLine_changeBPM");
		btn_TimeLine_changeBPM->setSize(50,20);
		btn_TimeLine_changeBPM->setPosition(txtField_timeline_BPM->getX()+txtField_timeline_BPM->getWidth()+5,txtField_timeline_BPM->getY());
		btn_TimeLine_changeBPM->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_TimeLine_changeBPM, this, "btn_TimeLine_changeBPM", sora::RESPONSEACTION);
		timelinePanel->add(btn_TimeLine_changeBPM);

		gcn::Button *btn_TimeLine_insertBPM = new gcn::Button("Insert");
		btn_TimeLine_insertBPM->setId("btn_TimeLine_insertBPM");
		btn_TimeLine_insertBPM->setSize(50,20);
		btn_TimeLine_insertBPM->setPosition(btn_TimeLine_changeBPM->getX()+btn_TimeLine_changeBPM->getWidth()+5,txtField_timeline_BPM->getY());
		btn_TimeLine_insertBPM->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_TimeLine_insertBPM, this, "btn_TimeLine_insertBPM", sora::RESPONSEACTION);
		timelinePanel->add(btn_TimeLine_insertBPM);

		gcn::Button *btn_TimeLine_deleteBPM = new gcn::Button("Delete");
		btn_TimeLine_deleteBPM->setId("btn_TimeLine_deleteBPM");
		btn_TimeLine_deleteBPM->setSize(50,20);
		btn_TimeLine_deleteBPM->setPosition(btn_TimeLine_insertBPM->getX()+btn_TimeLine_insertBPM->getWidth()+5,txtField_timeline_BPM->getY());
		btn_TimeLine_deleteBPM->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_TimeLine_deleteBPM, this, "btn_TimeLine_deleteBPM", sora::RESPONSEACTION);
		timelinePanel->add(btn_TimeLine_deleteBPM);

#pragma endregion BPM operation
		
#pragma region Offset operation

		gcn::WTextField *txtField_timeline_offSet = new gcn::WTextField();
		txtField_timeline_offSet->enableNumericMode(true);
		txtField_timeline_offSet->setId("txtField_timeline_offSet");
		txtField_timeline_offSet->setSize(100,20);
		txtField_timeline_offSet->setPosition(100,80);
		sora::SoraGUI::Instance()->registerGUIResponser(txtField_timeline_offSet, this, "txtField_timeline_offSet", sora::RESPONSEACTION);
		timelinePanel->add(txtField_timeline_offSet);

		gcn::Label *label_timeline_offset = new gcn::Label("Grid Offset:");
		label_timeline_offset->adjustSize();
		label_timeline_offset->setBackgroundColor(gcn::Color(0,0,0,0));
		label_timeline_offset->setPosition(txtField_timeline_offSet->getX()-label_timeline_offset->getWidth()-5,
			txtField_timeline_offSet->getY() + (txtField_timeline_offSet->getHeight()-label_timeline_offset->getHeight())/2);
		label_timeline_offset->setForegroundColor(gcn::Color(255,255,255,255));
		timelinePanel->add(label_timeline_offset);

		gcn::Button *btn_TimeLine_changeOffset = new gcn::Button("Change");
		btn_TimeLine_changeOffset->setId("btn_TimeLine_changeOffset");
		btn_TimeLine_changeOffset->setSize(50,20);
		btn_TimeLine_changeOffset->setPosition(txtField_timeline_offSet->getX()+txtField_timeline_offSet->getWidth()+5,txtField_timeline_offSet->getY());
		btn_TimeLine_changeOffset->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_TimeLine_changeOffset, this, "btn_TimeLine_changeOffset", sora::RESPONSEACTION);
		timelinePanel->add(btn_TimeLine_changeOffset);


#pragma endregion Offset operation

		
		return timelineCategory;
	}

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



		container_Categories[State::TIMELINE] = initTimelineCategory();


		top->add(container_Categories[State::TIMELINE]);
		top->moveToBottom(container_Categories[State::TIMELINE]);





		//Add Total Time Progress
		DivaEditorMusicProgressWidget *progressBar = new DivaEditorMusicProgressWidget();
		progressBar->setId("progressBar");
		progressBar->setSize(500,30);
		progressBar->setPosition(50,680);
		progressBar->setBackgroundColor(gcn::Color(0,0,0,0));
		progressBar->setForegroundColor(gcn::Color(255,255,255,255));
		top->add(progressBar);


		nowState = State::TIMELINE;
	}

	void DivaEditorMainScene::onUpdate(float dt)
	{
		if(nowState==TIMELINE)
		{
			gcn::WTextField* txtField_timeline_BPM = (gcn::WTextField*)container_Categories[nowState]->findWidgetById("txtField_timeline_BPM");
			gcn::WTextField* txtField_timeline_offSet = (gcn::WTextField*)container_Categories[nowState]->findWidgetById("txtField_timeline_offSet");
			gcn::Button* btn_TimeLine_changeOffset = (gcn::Button*)container_Categories[nowState]->findWidgetById("btn_TimeLine_changeOffset");

			gcn::Button* btn_TimeLine_changeBPM = (gcn::Button*)container_Categories[nowState]->findWidgetById("btn_TimeLine_changeBPM");
			gcn::Button* btn_TimeLine_insertBPM = (gcn::Button*)container_Categories[nowState]->findWidgetById("btn_TimeLine_insertBPM");
			gcn::Button* btn_TimeLine_deleteBPM = (gcn::Button*)container_Categories[nowState]->findWidgetById("btn_TimeLine_deleteBPM");

			if(!(txtField_timeline_BPM->isFocused()||btn_TimeLine_changeBPM->isFocused()||btn_TimeLine_insertBPM->isFocused()||btn_TimeLine_deleteBPM->isFocused()))
			{
				txtField_timeline_BPM->setText(fTows(EDITOR_PTR->mapData->getBPM(CORE_PTR->getRunPosition()),2));
			}
			if(!(txtField_timeline_offSet->isFocused()||btn_TimeLine_changeOffset->isFocused()))
			{
				txtField_timeline_offSet->setText(iToWS(EDITOR_PTR->mapData->getOffset()));
			}
		}
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
		gcn::Widget *actionWidget = top->findWidgetById(getID());

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

		//Category
		else if(getID() == "btn_TimeLine") 
		{
			EDITOR_PTR->mapData->stop_insert(CORE_PTR->getRunPosition(),72);
			
		}
		else if(getID() == "btn_Note")
		{
			EDITOR_PTR->mapData->offset_set(EDITOR_PTR->mapData->getOffset()+4,true);
		}

		//Timeline
		else if(getID() == "btn_TimeLine_wider")
		{
			EDITCONFIG->increaseShowRangeScale();
		}
		else if(getID() == "btn_TimeLine_tighter")
		{
			EDITCONFIG->decreaseShowRangeScale();
		}
		else if(getID() == "btn_TimeLine_deeper")
		{
			EDITCONFIG->increaseGridToShowPerBeat();
		}
		else if(getID() == "btn_TimeLine_lighter")
		{
			EDITCONFIG->decreaseGridToShowPerBeat();
		}


		//Timeline Category
		else if(getID() == "txtField_timeline_BPM" || getID() == "btn_TimeLine_changeBPM")
		{			
			EDITOR_PTR->mapData->bpm_change(CORE_PTR->getRunPosition(), ((WTextField*)container_Categories[State::TIMELINE]->findWidgetById("txtField_timeline_BPM"))->getFloat());
			actionWidget->setFocusable(false);
			actionWidget->setFocusable(true);
		}
		else if(getID() == "txtField_timeline_offSet" || getID() == "btn_TimeLine_changeOffset")
		{
			int thisOffset = ((WTextField*)container_Categories[State::TIMELINE]->findWidgetById("txtField_timeline_offSet"))->getFloat();
			EDITOR_PTR->mapData->offset_set(thisOffset, true);
			actionWidget->setFocusable(false);
			actionWidget->setFocusable(true);
		}

	}
}