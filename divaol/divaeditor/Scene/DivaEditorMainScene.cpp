#include "divaeditor/Scene/DivaEditorScene.h"
#include "divaeditor/Component/DivaEditorStandardOperation.h"
#include "divaeditor/DivaEditorCommon.h"

#include "Animation/SoraGUIAnimation.h"

#include "divacore/Component/DivaStandardCoreFlow.h"
#include "divacore/Mode/DivaEditMode.h"

#include "direct.h"

//#define EDITOR_DIVA
#define EDITOR_PICKMEUP

namespace divaeditor
{
	using namespace gcn;

#define EditUtility divacore::StandardEditUtility::instance()

#define MainSceneImageFile L"standard/pic/editor_001.png"

	gcn::Container* DivaEditorMainScene::initTimelineCategory()
	{
		//Init Timeline Category
		gcn::Container *timelineCategory = new gcn::Container();
		timelineCategory->setSize(1280,720);
		timelineCategory->setOpaque(true);
		timelineCategory->setBaseColor(gcn::Color(0,0,0,0));

		//Timeline Panel
		gcn::BoarderedContainer *timelinePanel = new gcn::BoarderedContainer();
		timelinePanel->setBoarderSize(3);
		timelinePanel->setBaseColor(gcn::Color(16,22,33,200));
		timelinePanel->setForegroundColor(gcn::Color(53,53,53,150));
		//timelinePanel->setOpaque(true);
		//timelinePanel->setAlpha(150);
		timelinePanel->setId("timelinePanel");
		timelinePanel->setSize(400,300);
		timelinePanel->setPosition(800,300);
		timelineCategory->add(timelinePanel);


		gcn::WCheckBox *wcheckbox_timeline_moveNote = new gcn::WCheckBox(LOCALIZATION->getLocalStr(L"MainScene_wcheckbox_timeline_moveNote"));
		wcheckbox_timeline_moveNote->setSelected(true);
		wcheckbox_timeline_moveNote->setId("wcheckbox_timeline_moveNote");
		wcheckbox_timeline_moveNote->adjustSize();
		wcheckbox_timeline_moveNote->setPosition(60,20);
		wcheckbox_timeline_moveNote->setBaseColor(gcn::Color(0,0,0,150));
		wcheckbox_timeline_moveNote->setForegroundColor(gcn::Color(255,255,255,255));
		wcheckbox_timeline_moveNote->setCheckForeGroundColor(gcn::Color(0,0,0,255));
		sora::SoraGUI::Instance()->registerGUIResponser(wcheckbox_timeline_moveNote, this, "wcheckbox_timeline_moveNote", sora::RESPONSEACTION);
		timelinePanel->add(wcheckbox_timeline_moveNote);


#pragma region BPM operation

		

		gcn::WTextField *txtField_timeline_BPM = new gcn::WTextField();
		txtField_timeline_BPM->enableNumericMode(true);
		txtField_timeline_BPM->setId("txtField_timeline_BPM");
		txtField_timeline_BPM->setSize(100,20);
		txtField_timeline_BPM->setPosition(150,50);
		sora::SoraGUI::Instance()->registerGUIResponser(txtField_timeline_BPM, this, "txtField_timeline_BPM", sora::RESPONSEACTION);
		timelinePanel->add(txtField_timeline_BPM);

		gcn::WLabel *label_timeline_bpm = new gcn::WLabel(LOCALIZATION->getLocalStr(L"MainScene_label_timeline_bpm"));
		label_timeline_bpm->adjustSize();
		label_timeline_bpm->setBackgroundColor(gcn::Color(0,0,0,0));
		label_timeline_bpm->setPosition(txtField_timeline_BPM->getX()-label_timeline_bpm->getWidth()-5,
			txtField_timeline_BPM->getY() + (txtField_timeline_BPM->getHeight()-label_timeline_bpm->getHeight())/2);
		label_timeline_bpm->setForegroundColor(gcn::Color(255,255,255,255));
		timelinePanel->add(label_timeline_bpm);

		gcn::WButton *btn_TimeLine_changeBPM = new gcn::WButton();
		btn_TimeLine_changeBPM->setFrameSize(0);
		btn_TimeLine_changeBPM->setNormalImage(MainSceneImageFile,gcn::Rectangle(316,363,23,23));
		btn_TimeLine_changeBPM->setDownImage(MainSceneImageFile,gcn::Rectangle(385,363,23,23));
		btn_TimeLine_changeBPM->setId("btn_TimeLine_changeBPM");
		btn_TimeLine_changeBPM->setSize(23,23);
		btn_TimeLine_changeBPM->setPosition(txtField_timeline_BPM->getX()+txtField_timeline_BPM->getWidth()+5,txtField_timeline_BPM->getY());
		btn_TimeLine_changeBPM->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_TimeLine_changeBPM, this, "btn_TimeLine_changeBPM", sora::RESPONSEACTION);
		timelinePanel->add(btn_TimeLine_changeBPM);

		gcn::WButton *btn_TimeLine_insertBPM = new gcn::WButton();
		btn_TimeLine_insertBPM->setFrameSize(0);
		btn_TimeLine_insertBPM->setNormalImage(MainSceneImageFile,gcn::Rectangle(339,363,23,23));
		btn_TimeLine_insertBPM->setDownImage(MainSceneImageFile,gcn::Rectangle(408,363,23,23));
		btn_TimeLine_insertBPM->setId("btn_TimeLine_insertBPM");
		btn_TimeLine_insertBPM->setSize(23,23);
		btn_TimeLine_insertBPM->setPosition(btn_TimeLine_changeBPM->getX()+btn_TimeLine_changeBPM->getWidth()+2,txtField_timeline_BPM->getY());
		btn_TimeLine_insertBPM->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_TimeLine_insertBPM, this, "btn_TimeLine_insertBPM", sora::RESPONSEACTION);
		timelinePanel->add(btn_TimeLine_insertBPM);

		gcn::WButton *btn_TimeLine_deleteBPM = new gcn::WButton();
		btn_TimeLine_deleteBPM->setFrameSize(0);
		btn_TimeLine_deleteBPM->setNormalImage(MainSceneImageFile,gcn::Rectangle(362,363,23,23));
		btn_TimeLine_deleteBPM->setDownImage(MainSceneImageFile,gcn::Rectangle(431,363,23,23));
		btn_TimeLine_deleteBPM->setId("btn_TimeLine_deleteBPM");
		btn_TimeLine_deleteBPM->setSize(23,23);
		btn_TimeLine_deleteBPM->setPosition(btn_TimeLine_insertBPM->getX()+btn_TimeLine_insertBPM->getWidth()+2,txtField_timeline_BPM->getY());
		btn_TimeLine_deleteBPM->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_TimeLine_deleteBPM, this, "btn_TimeLine_deleteBPM", sora::RESPONSEACTION);
		timelinePanel->add(btn_TimeLine_deleteBPM);

#pragma endregion BPM operation

#pragma region Stop Operation

		gcn::WTextField *txtField_timeline_Stop = new gcn::WTextField();
		txtField_timeline_Stop->enableNumericMode(true);
		txtField_timeline_Stop->setId("txtField_timeline_Stop");
		txtField_timeline_Stop->setSize(100,20);
		txtField_timeline_Stop->setPosition(txtField_timeline_BPM->getX(),txtField_timeline_BPM->getY()+txtField_timeline_BPM->getHeight()+10);
		sora::SoraGUI::Instance()->registerGUIResponser(txtField_timeline_Stop, this, "txtField_timeline_Stop", sora::RESPONSEACTION);
		timelinePanel->add(txtField_timeline_Stop);

		gcn::WLabel *label_timeline_Stop = new gcn::WLabel(LOCALIZATION->getLocalStr(L"MainScene_label_timeline_Stop"));
		label_timeline_Stop->adjustSize();
		label_timeline_Stop->setBackgroundColor(gcn::Color(0,0,0,0));
		label_timeline_Stop->setPosition(txtField_timeline_Stop->getX()-label_timeline_Stop->getWidth()-5,
			txtField_timeline_Stop->getY() + (txtField_timeline_Stop->getHeight()-label_timeline_Stop->getHeight())/2);
		label_timeline_Stop->setForegroundColor(gcn::Color(255,255,255,255));
		timelinePanel->add(label_timeline_Stop);

		gcn::WButton *btn_TimeLine_changeStop = new gcn::WButton();
		btn_TimeLine_changeStop->setFrameSize(0);
		btn_TimeLine_changeStop->setNormalImage(MainSceneImageFile,gcn::Rectangle(316,363,23,23));
		btn_TimeLine_changeStop->setDownImage(MainSceneImageFile,gcn::Rectangle(385,363,23,23));
		btn_TimeLine_changeStop->setId("btn_TimeLine_changeStop");
		btn_TimeLine_changeStop->setSize(23,23);
		btn_TimeLine_changeStop->setPosition(txtField_timeline_Stop->getX()+txtField_timeline_Stop->getWidth()+5,txtField_timeline_Stop->getY());
		btn_TimeLine_changeStop->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_TimeLine_changeStop, this, "btn_TimeLine_changeStop", sora::RESPONSEACTION);
		timelinePanel->add(btn_TimeLine_changeStop);

		gcn::WButton *btn_TimeLine_insertStop = new gcn::WButton();
		btn_TimeLine_insertStop->setFrameSize(0);
		btn_TimeLine_insertStop->setNormalImage(MainSceneImageFile,gcn::Rectangle(339,363,23,23));
		btn_TimeLine_insertStop->setDownImage(MainSceneImageFile,gcn::Rectangle(408,363,23,23));
		btn_TimeLine_insertStop->setId("btn_TimeLine_insertStop");
		btn_TimeLine_insertStop->setSize(23,23);
		btn_TimeLine_insertStop->setPosition(txtField_timeline_Stop->getX()+txtField_timeline_Stop->getWidth()+5,txtField_timeline_Stop->getY());
		btn_TimeLine_insertStop->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_TimeLine_insertStop, this, "btn_TimeLine_insertStop", sora::RESPONSEACTION);
		timelinePanel->add(btn_TimeLine_insertStop);

		gcn::WButton *btn_TimeLine_deleteStop = new gcn::WButton();
		btn_TimeLine_deleteStop->setFrameSize(0);
		btn_TimeLine_deleteStop->setNormalImage(MainSceneImageFile,gcn::Rectangle(362,363,23,23));
		btn_TimeLine_deleteStop->setDownImage(MainSceneImageFile,gcn::Rectangle(431,363,23,23));
		btn_TimeLine_deleteStop->setId("btn_TimeLine_deleteStop");
		btn_TimeLine_deleteStop->setSize(23,23);
		btn_TimeLine_deleteStop->setPosition(btn_TimeLine_changeStop->getX()+btn_TimeLine_changeStop->getWidth()+2,txtField_timeline_Stop->getY());
		btn_TimeLine_deleteStop->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_TimeLine_deleteStop, this, "btn_TimeLine_deleteStop", sora::RESPONSEACTION);
		timelinePanel->add(btn_TimeLine_deleteStop);

#pragma endregion Stop Operation

#pragma region BeatNum Operation

		gcn::WTextField *txtField_timeline_BeatNum = new gcn::WTextField();
		txtField_timeline_BeatNum->enableNumericMode(true);
		txtField_timeline_BeatNum->setId("txtField_timeline_BeatNum");
		txtField_timeline_BeatNum->setSize(100,20);
		txtField_timeline_BeatNum->setPosition(txtField_timeline_BPM->getX(),txtField_timeline_Stop->getY()+txtField_timeline_Stop->getHeight()+10);
		sora::SoraGUI::Instance()->registerGUIResponser(txtField_timeline_BeatNum, this, "txtField_timeline_BeatNum", sora::RESPONSEACTION);
		timelinePanel->add(txtField_timeline_BeatNum);

		gcn::WLabel *label_timeline_BeatNum = new gcn::WLabel(LOCALIZATION->getLocalStr(L"MainScene_label_timeline_BeatNum"));
		label_timeline_BeatNum->adjustSize();
		label_timeline_BeatNum->setBackgroundColor(gcn::Color(0,0,0,0));
		label_timeline_BeatNum->setPosition(txtField_timeline_BeatNum->getX()-label_timeline_BeatNum->getWidth()-5,
			txtField_timeline_BeatNum->getY() + (txtField_timeline_BeatNum->getHeight()-label_timeline_BeatNum->getHeight())/2);
		label_timeline_BeatNum->setForegroundColor(gcn::Color(255,255,255,255));
		timelinePanel->add(label_timeline_BeatNum);

		gcn::WButton *btn_TimeLine_changeBeatNum = new gcn::WButton();
		btn_TimeLine_changeBeatNum->setFrameSize(0);
		btn_TimeLine_changeBeatNum->setNormalImage(MainSceneImageFile,gcn::Rectangle(339,363,23,23));
		btn_TimeLine_changeBeatNum->setDownImage(MainSceneImageFile,gcn::Rectangle(408,363,23,23));
		btn_TimeLine_changeBeatNum->setId("btn_TimeLine_changeBeatNum");
		btn_TimeLine_changeBeatNum->setSize(23,23);
		btn_TimeLine_changeBeatNum->setPosition(txtField_timeline_BeatNum->getX()+txtField_timeline_BeatNum->getWidth()+5,txtField_timeline_BeatNum->getY());
		btn_TimeLine_changeBeatNum->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_TimeLine_changeBeatNum, this, "btn_TimeLine_changeBeatNum", sora::RESPONSEACTION);
		timelinePanel->add(btn_TimeLine_changeBeatNum);

		gcn::WButton *btn_TimeLine_deleteBeatNum = new gcn::WButton();
		btn_TimeLine_deleteBeatNum->setFrameSize(0);
		btn_TimeLine_deleteBeatNum->setNormalImage(MainSceneImageFile,gcn::Rectangle(362,363,23,23));
		btn_TimeLine_deleteBeatNum->setDownImage(MainSceneImageFile,gcn::Rectangle(431,363,23,23));
		btn_TimeLine_deleteBeatNum->setId("btn_TimeLine_deleteBeatNum");
		btn_TimeLine_deleteBeatNum->setSize(23,23);
		btn_TimeLine_deleteBeatNum->setPosition(btn_TimeLine_changeBeatNum->getX()+btn_TimeLine_changeBeatNum->getWidth()+2,txtField_timeline_BeatNum->getY());
		btn_TimeLine_deleteBeatNum->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_TimeLine_deleteBeatNum, this, "btn_TimeLine_deleteBeatNum", sora::RESPONSEACTION);
		timelinePanel->add(btn_TimeLine_deleteBeatNum);

#pragma endregion BeatNum Operation
		
#pragma region TailSpeed Operation


		gcn::WTextField *txtField_timeline_TailSpeed = new gcn::WTextField();
		txtField_timeline_TailSpeed->enableNumericMode(true);
		txtField_timeline_TailSpeed->setId("txtField_timeline_TailSpeed");
		txtField_timeline_TailSpeed->setSize(100,20);
		txtField_timeline_TailSpeed->setPosition(txtField_timeline_BPM->getX(),txtField_timeline_BeatNum->getY()+txtField_timeline_BeatNum->getHeight()+10);
		sora::SoraGUI::Instance()->registerGUIResponser(txtField_timeline_TailSpeed, this, "txtField_timeline_TailSpeed", sora::RESPONSEACTION);
		timelinePanel->add(txtField_timeline_TailSpeed);

		gcn::WLabel *label_timeline_TailSpeed = new gcn::WLabel(LOCALIZATION->getLocalStr(L"MainScene_txtField_timeline_TailSpeed"));
		label_timeline_TailSpeed->adjustSize();
		label_timeline_TailSpeed->setBackgroundColor(gcn::Color(0,0,0,0));
		label_timeline_TailSpeed->setPosition(txtField_timeline_TailSpeed->getX()-label_timeline_TailSpeed->getWidth()-5,
			txtField_timeline_TailSpeed->getY() + (txtField_timeline_TailSpeed->getHeight()-label_timeline_TailSpeed->getHeight())/2);
		label_timeline_TailSpeed->setForegroundColor(gcn::Color(255,255,255,255));
		timelinePanel->add(label_timeline_TailSpeed);

		gcn::WButton *btn_TimeLine_changeTailSpeed = new gcn::WButton();
		btn_TimeLine_changeTailSpeed->setFrameSize(0);
		btn_TimeLine_changeTailSpeed->setNormalImage(MainSceneImageFile,gcn::Rectangle(316,363,23,23));
		btn_TimeLine_changeTailSpeed->setDownImage(MainSceneImageFile,gcn::Rectangle(385,363,23,23));
		btn_TimeLine_changeTailSpeed->setId("btn_TimeLine_changeTailSpeed");
		btn_TimeLine_changeTailSpeed->setSize(23,23);
		btn_TimeLine_changeTailSpeed->setPosition(txtField_timeline_TailSpeed->getX()+txtField_timeline_TailSpeed->getWidth()+5,txtField_timeline_TailSpeed->getY());
		btn_TimeLine_changeTailSpeed->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_TimeLine_changeTailSpeed, this, "btn_TimeLine_changeTailSpeed", sora::RESPONSEACTION);
		timelinePanel->add(btn_TimeLine_changeTailSpeed);

#pragma endregion TailSpeed Operation

#pragma region Offset operation

		gcn::WTextField *txtField_timeline_offSet = new gcn::WTextField();
		txtField_timeline_offSet->enableNumericMode(true);
		txtField_timeline_offSet->setId("txtField_timeline_offSet");
		txtField_timeline_offSet->setSize(100,20);
		txtField_timeline_offSet->setPosition(txtField_timeline_BPM->getX(),txtField_timeline_TailSpeed->getY()+txtField_timeline_TailSpeed->getHeight()+30);
		sora::SoraGUI::Instance()->registerGUIResponser(txtField_timeline_offSet, this, "txtField_timeline_offSet", sora::RESPONSEACTION);
		timelinePanel->add(txtField_timeline_offSet);

		gcn::WLabel *label_timeline_offset = new gcn::WLabel(LOCALIZATION->getLocalStr(L"MainScene_label_timeline_offset"));
		label_timeline_offset->adjustSize();
		label_timeline_offset->setBackgroundColor(gcn::Color(0,0,0,0));
		label_timeline_offset->setPosition(txtField_timeline_offSet->getX()-label_timeline_offset->getWidth()-5,
			txtField_timeline_offSet->getY() + (txtField_timeline_offSet->getHeight()-label_timeline_offset->getHeight())/2);
		label_timeline_offset->setForegroundColor(gcn::Color(255,255,255,255));
		timelinePanel->add(label_timeline_offset);

		gcn::WButton *btn_TimeLine_changeOffset = new gcn::WButton();
		btn_TimeLine_changeOffset->setFrameSize(0);
		btn_TimeLine_changeOffset->setNormalImage(MainSceneImageFile,gcn::Rectangle(316,363,23,23));
		btn_TimeLine_changeOffset->setDownImage(MainSceneImageFile,gcn::Rectangle(385,363,23,23));
		btn_TimeLine_changeOffset->setId("btn_TimeLine_changeOffset");
		btn_TimeLine_changeOffset->setSize(23,23);
		btn_TimeLine_changeOffset->setPosition(txtField_timeline_offSet->getX()+txtField_timeline_offSet->getWidth()+5,txtField_timeline_offSet->getY());
		btn_TimeLine_changeOffset->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_TimeLine_changeOffset, this, "btn_TimeLine_changeOffset", sora::RESPONSEACTION);
		timelinePanel->add(btn_TimeLine_changeOffset);

		gcn::DivaEditorSlider *slider_timeline_offset = new gcn::DivaEditorSlider(-192,192);
		slider_timeline_offset->setId("slider_timeline_offset");
		slider_timeline_offset->setSize(385,20);
		slider_timeline_offset->setPosition((timelinePanel->getWidth()-slider_timeline_offset->getWidth())/2,txtField_timeline_offSet->getY()+txtField_timeline_offSet->getHeight()+10);
		sora::SoraGUI::Instance()->registerGUIResponser(slider_timeline_offset, this, "slider_timeline_offset", sora::RESPONSEACTION);
		timelinePanel->add(slider_timeline_offset);


#pragma endregion Offset operation
		
		return timelineCategory;
	}
	gcn::Container* DivaEditorMainScene::initNoteCategory()
	{
		//Init Note Category
		gcn::Container *noteCategory = new gcn::Container();
		noteCategory->setSize(top->getWidth(),top->getHeight());
		noteCategory->setOpaque(true);
		noteCategory->setBaseColor(gcn::Color(0,0,0,0));

#ifdef EDITOR_DIVA
		NoteArea *noteArea = new NoteArea();
#else
		PickMeUpNoteArea *noteArea = new PickMeUpNoteArea();
#endif

		float factor = 720.0/1080.0;
		noteArea->setId("NoteArea");
		noteArea->setPosition(EDITCONFIG->NoteAreaX*factor, EDITCONFIG->NoteAreaY*factor);
		noteArea->adjustSize(factor);
		noteCategory->add(noteArea);


#pragma region tool box button set

		gcn::Container *toolBoxButtonSet = new gcn::Container();
		toolBoxButtonSet->setId("toolBoxButtonSet");
		toolBoxButtonSet->setBaseColor(gcn::Color(0,0,0,0));
		toolBoxButtonSet->setPosition(top->getWidth() - 55,noteArea->getY());
		noteCategory->add(toolBoxButtonSet);
		
		gcn::WButton *btn_note_flipHorizontal = new gcn::WButton();
		btn_note_flipHorizontal->setFrameSize(0);
		btn_note_flipHorizontal->setNormalImage(MainSceneImageFile,gcn::Rectangle(353,145,45,45));
		btn_note_flipHorizontal->setDownImage(MainSceneImageFile,gcn::Rectangle(90,318,45,45));
		btn_note_flipHorizontal->setForegroundColor(gcn::Color(255,255,255,255));
		btn_note_flipHorizontal->setId("btn_note_flipHorizontal");
		btn_note_flipHorizontal->setSize(45,45);
		//btn_note_flipHorizontal->setPosition(wlable_toolBoxTitle->getX()+5,wlable_toolBoxTitle->getY()+wlable_toolBoxTitle->getHeight()+5);
		btn_note_flipHorizontal->setPosition(0,0);
		sora::SoraGUI::Instance()->registerGUIResponser(btn_note_flipHorizontal, this, "btn_note_flipHorizontal", sora::RESPONSEACTION);
		toolBoxButtonSet->add(btn_note_flipHorizontal);

		gcn::WButton *btn_note_flipVertical = new gcn::WButton();
		btn_note_flipVertical->setFrameSize(0);
		btn_note_flipVertical->setNormalImage(MainSceneImageFile,gcn::Rectangle(443,145,45,45));
		btn_note_flipVertical->setDownImage(MainSceneImageFile,gcn::Rectangle(173,145,45,45));
		btn_note_flipVertical->setForegroundColor(gcn::Color(255,255,255,255));
		btn_note_flipVertical->setId("btn_note_flipVertical");
		btn_note_flipVertical->setSize(btn_note_flipHorizontal->getWidth(),btn_note_flipHorizontal->getHeight());
		btn_note_flipVertical->setPosition(btn_note_flipHorizontal->getX(),btn_note_flipHorizontal->getY()+btn_note_flipHorizontal->getHeight()+5);
		sora::SoraGUI::Instance()->registerGUIResponser(btn_note_flipVertical, this, "btn_note_flipVertical", sora::RESPONSEACTION);
		toolBoxButtonSet->add(btn_note_flipVertical);


		gcn::WButton *btn_note_allToLetter = new gcn::WButton();
		btn_note_allToLetter->setFrameSize(0);
		btn_note_allToLetter->setNormalImage(MainSceneImageFile,gcn::Rectangle(128,145,45,45));
		btn_note_allToLetter->setDownImage(MainSceneImageFile,gcn::Rectangle(90,273,45,45));
		btn_note_allToLetter->setForegroundColor(gcn::Color(255,255,255,255));
		btn_note_allToLetter->setId("btn_note_allToLetter");
		btn_note_allToLetter->setSize(btn_note_flipHorizontal->getWidth(),btn_note_flipHorizontal->getHeight());
		btn_note_allToLetter->setPosition(btn_note_flipHorizontal->getX(),btn_note_flipVertical->getY()+btn_note_flipVertical->getHeight()+5);
		sora::SoraGUI::Instance()->registerGUIResponser(btn_note_allToLetter, this, "btn_note_allToLetter", sora::RESPONSEACTION);
		toolBoxButtonSet->add(btn_note_allToLetter);

		gcn::WButton *btn_note_allToArrow = new gcn::WButton();
		btn_note_allToArrow->setFrameSize(0);
		btn_note_allToArrow->setNormalImage(MainSceneImageFile,gcn::Rectangle(263,145,45,45));
		btn_note_allToArrow->setDownImage(MainSceneImageFile,gcn::Rectangle(315,273,45,45));
		btn_note_allToArrow->setForegroundColor(gcn::Color(255,255,255,255));
		btn_note_allToArrow->setId("btn_note_allToArrow");
		btn_note_allToArrow->setSize(btn_note_flipHorizontal->getWidth(),btn_note_flipHorizontal->getHeight());
		btn_note_allToArrow->setPosition(btn_note_flipHorizontal->getX(),btn_note_allToLetter->getY()+btn_note_allToLetter->getHeight()+5);
		sora::SoraGUI::Instance()->registerGUIResponser(btn_note_allToArrow, this, "btn_note_allToArrow", sora::RESPONSEACTION);
		toolBoxButtonSet->add(btn_note_allToArrow);

		gcn::WButton *btn_note_noteTypeSymmetry = new gcn::WButton();
		btn_note_noteTypeSymmetry->setFrameSize(0);
		btn_note_noteTypeSymmetry->setNormalImage(MainSceneImageFile,gcn::Rectangle(308,145,45,45));
		btn_note_noteTypeSymmetry->setDownImage(MainSceneImageFile,gcn::Rectangle(218,145,45,45));
		btn_note_noteTypeSymmetry->setForegroundColor(gcn::Color(255,255,255,255));
		btn_note_noteTypeSymmetry->setId("btn_note_noteTypeSymmetry");
		btn_note_noteTypeSymmetry->setSize(btn_note_flipHorizontal->getWidth(),btn_note_flipHorizontal->getHeight());
		btn_note_noteTypeSymmetry->setPosition(btn_note_flipHorizontal->getX(),btn_note_allToArrow->getY()+btn_note_allToArrow->getHeight()+5);
		sora::SoraGUI::Instance()->registerGUIResponser(btn_note_noteTypeSymmetry, this, "btn_note_noteTypeSymmetry", sora::RESPONSEACTION);
		toolBoxButtonSet->add(btn_note_noteTypeSymmetry);
		

		gcn::WButton *btn_note_simplifyTo2Key = new gcn::WButton();
		btn_note_simplifyTo2Key->setFrameSize(0);
		btn_note_simplifyTo2Key->setNormalImage(MainSceneImageFile,gcn::Rectangle(45,273,45,45));
		btn_note_simplifyTo2Key->setDownImage(MainSceneImageFile,gcn::Rectangle(405,273,45,45));
		btn_note_simplifyTo2Key->setForegroundColor(gcn::Color(255,255,255,255));
		btn_note_simplifyTo2Key->setId("btn_note_simplifyTo2Key");
		btn_note_simplifyTo2Key->setSize(btn_note_flipHorizontal->getWidth(),btn_note_flipHorizontal->getHeight());
		btn_note_simplifyTo2Key->setPosition(btn_note_flipHorizontal->getX(),btn_note_noteTypeSymmetry->getY()+btn_note_noteTypeSymmetry->getHeight()+5);
		sora::SoraGUI::Instance()->registerGUIResponser(btn_note_simplifyTo2Key, this, "btn_note_simplifyTo2Key", sora::RESPONSEACTION);
		toolBoxButtonSet->add(btn_note_simplifyTo2Key);

		gcn::WButton *btn_note_simplifyTo1Key = new gcn::WButton();
		btn_note_simplifyTo1Key->setFrameSize(0);
		btn_note_simplifyTo1Key->setNormalImage(MainSceneImageFile,gcn::Rectangle(398,145,45,45));
		btn_note_simplifyTo1Key->setDownImage(MainSceneImageFile,gcn::Rectangle(270,273,45,45));
		btn_note_simplifyTo1Key->setForegroundColor(gcn::Color(255,255,255,255));
		btn_note_simplifyTo1Key->setId("btn_note_simplifyTo1Key");
		btn_note_simplifyTo1Key->setSize(btn_note_flipHorizontal->getWidth(),btn_note_flipHorizontal->getHeight());
		btn_note_simplifyTo1Key->setPosition(btn_note_flipHorizontal->getX(),btn_note_simplifyTo2Key->getY()+btn_note_simplifyTo2Key->getHeight()+5);
		sora::SoraGUI::Instance()->registerGUIResponser(btn_note_simplifyTo1Key, this, "btn_note_simplifyTo1Key", sora::RESPONSEACTION);
		toolBoxButtonSet->add(btn_note_simplifyTo1Key);

		gcn::WButton *btn_note_splitLongNoteToSingle = new gcn::WButton();
		btn_note_splitLongNoteToSingle->setFrameSize(0);
		btn_note_splitLongNoteToSingle->setNormalImage(MainSceneImageFile,gcn::Rectangle(0,273,45,45));
		btn_note_splitLongNoteToSingle->setDownImage(MainSceneImageFile,gcn::Rectangle(360,273,45,45));
		btn_note_splitLongNoteToSingle->setForegroundColor(gcn::Color(255,255,255,255));
		btn_note_splitLongNoteToSingle->setId("btn_note_splitLongNoteToSingle");
		btn_note_splitLongNoteToSingle->setSize(btn_note_flipHorizontal->getWidth(),btn_note_flipHorizontal->getHeight());
		btn_note_splitLongNoteToSingle->setPosition(btn_note_flipHorizontal->getX(),btn_note_simplifyTo1Key->getY()+btn_note_simplifyTo1Key->getHeight()+5);
		sora::SoraGUI::Instance()->registerGUIResponser(btn_note_splitLongNoteToSingle, this, "btn_note_splitLongNoteToSingle", sora::RESPONSEACTION);
		toolBoxButtonSet->add(btn_note_splitLongNoteToSingle);



		toolBoxButtonSet->setSize(200,btn_note_splitLongNoteToSingle->getY() + btn_note_splitLongNoteToSingle->getHeight() + 10);

#pragma endregion tool box button set
		

		gcn::BoarderedContainer *keySoundBox = new gcn::BoarderedContainer();
		keySoundBox->setBoarderSize(3);
		keySoundBox->setBaseColor(gcn::Color(16,22,33,200));
		keySoundBox->setForegroundColor(gcn::Color(53,53,53,150));
		keySoundBox->setId("keySoundBox");
		keySoundBox->setSize((noteCategory->getWidth()-noteArea->getWidth())/2-10,noteArea->getHeight());
		keySoundBox->setAlpha(150);
		keySoundBox->setPosition(noteCategory->getWidth()-5-keySoundBox->getWidth(),toolBoxButtonSet->getY()+toolBoxButtonSet->getHeight());
		noteCategory->add(keySoundBox);


		gcn::WButton *btn_note_expandKeySoundBox = new gcn::WButton(LOCALIZATION->getLocalStr(L"MainScene_btn_note_expandKeySoundBox"));
		btn_note_expandKeySoundBox->setId("btn_note_expandKeySoundBox");
		btn_note_expandKeySoundBox->setSize(keySoundBox->getWidth(), 40);
		btn_note_expandKeySoundBox->setPosition(0,0);
		btn_note_expandKeySoundBox->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_note_expandKeySoundBox, this, "btn_note_expandKeySoundBox", sora::RESPONSEACTION);
		
#pragma region key sound box set

		gcn::Container *keySoundControlSet = new gcn::Container();
		keySoundControlSet->setId("keySoundControlSet");
		keySoundControlSet->setSize(400,noteArea->getHeight());
		keySoundControlSet->setBaseColor(gcn::Color(0,0,0,0));
		keySoundControlSet->setPosition(0,0);
		keySoundBox->add(keySoundControlSet);

		gcn::WButton *btn_note_modifyHitSound = new gcn::WButton(LOCALIZATION->getLocalStr(L"MainScene_changeStr"));
		btn_note_modifyHitSound->setId("btn_note_modifyHitSound");
		btn_note_modifyHitSound->setSize(50,20);
		btn_note_modifyHitSound->setPosition(10,50);
		btn_note_modifyHitSound->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_note_modifyHitSound, this, "btn_note_modifyHitSound", sora::RESPONSEACTION);
		keySoundControlSet->add(btn_note_modifyHitSound);

		gcn::WButton *btn_note_deleteHitSound = new gcn::WButton(LOCALIZATION->getLocalStr(L"MainScene_deleteStr"));
		btn_note_deleteHitSound->setId("btn_note_deleteHitSound");
		btn_note_deleteHitSound->setSize(50,20);
		btn_note_deleteHitSound->setPosition(btn_note_modifyHitSound->getX()+btn_note_modifyHitSound->getWidth()+5,btn_note_modifyHitSound->getY());
		btn_note_deleteHitSound->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_note_deleteHitSound, this, "btn_note_deleteHitSound", sora::RESPONSEACTION);
		keySoundControlSet->add(btn_note_deleteHitSound);

		gcn::WLabel *wlabel_globalHitSound = new gcn::WLabel(LOCALIZATION->getLocalStr(L"MainScene_wlabel_globalHitSound"));
		wlabel_globalHitSound->setId("wlabel_globalHitSound");
		wlabel_globalHitSound->setForegroundColor(gcn::Color(255,255,255,255));
		wlabel_globalHitSound->setBackgroundColor(gcn::Color(0,0,0,0));
		wlabel_globalHitSound->setPosition(btn_note_deleteHitSound->getX()+btn_note_deleteHitSound->getWidth()+5,btn_note_deleteHitSound->getY());
		wlabel_globalHitSound->adjustSize();
		keySoundControlSet->add(wlabel_globalHitSound);

		gcn::WButton *btn_note_modifyMissSound = new gcn::WButton(LOCALIZATION->getLocalStr(L"MainScene_changeStr"));
		btn_note_modifyMissSound->setId("btn_note_modifyMissSound");
		btn_note_modifyMissSound->setSize(50,20);
		btn_note_modifyMissSound->setPosition(btn_note_modifyHitSound->getX(),btn_note_modifyHitSound->getY()+btn_note_modifyHitSound->getHeight()+5);
		btn_note_modifyMissSound->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_note_modifyMissSound, this, "btn_note_modifyMissSound", sora::RESPONSEACTION);
		keySoundControlSet->add(btn_note_modifyMissSound);

		gcn::WButton *btn_note_deleteMissSound = new gcn::WButton(LOCALIZATION->getLocalStr(L"MainScene_deleteStr"));
		btn_note_deleteMissSound->setId("btn_note_deleteMissSound");
		btn_note_deleteMissSound->setSize(50,20);
		btn_note_deleteMissSound->setPosition(btn_note_modifyMissSound->getX()+btn_note_modifyMissSound->getWidth()+5,btn_note_modifyMissSound->getY());
		btn_note_deleteMissSound->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_note_deleteMissSound, this, "btn_note_deleteMissSound", sora::RESPONSEACTION);
		keySoundControlSet->add(btn_note_deleteMissSound);

		gcn::WLabel *wlabel_globalMissSound = new gcn::WLabel(LOCALIZATION->getLocalStr(L"MainScene_wlabel_globalMissSound"));
		wlabel_globalMissSound->setId("wlabel_globalMissSound");
		wlabel_globalMissSound->setForegroundColor(gcn::Color(255,255,255,255));
		wlabel_globalMissSound->setBackgroundColor(gcn::Color(0,0,0,0));
		wlabel_globalMissSound->setPosition(btn_note_deleteMissSound->getX()+btn_note_deleteMissSound->getWidth()+5,btn_note_deleteMissSound->getY());
		wlabel_globalMissSound->adjustSize();
		keySoundControlSet->add(wlabel_globalMissSound);

		gcn::WLabel *wlabel_selectedNoteKey = new gcn::WLabel(LOCALIZATION->getLocalStr(L"MainScene_wlabel_selectedNoteKey"));
		wlabel_selectedNoteKey->setId("wlabel_selectedNoteKey");
		wlabel_selectedNoteKey->setForegroundColor(gcn::Color(255,255,255,255));
		wlabel_selectedNoteKey->setBackgroundColor(gcn::Color(0,0,0,0));
		wlabel_selectedNoteKey->setPosition(btn_note_modifyHitSound->getX(), wlabel_globalMissSound->getY()+50);
		wlabel_selectedNoteKey->adjustSize();
		keySoundControlSet->add(wlabel_selectedNoteKey);


		gcn::Container *listbox_container = new gcn::Container();
		listbox_container->setId("listbox_container");
		listbox_container->setSize(300, 240);
		listbox_container->setPosition(wlabel_selectedNoteKey->getX(),wlabel_selectedNoteKey->getY()+30);
		keySoundControlSet->add(listbox_container);

		gcn::WListBox *wlistbox_notekey = new gcn::WListBox(new WListModel());
		wlistbox_notekey->setId("wlistbox_notekey");
		wlistbox_notekey->setSize(300,500);
		wlistbox_notekey->setPosition(0,0);
		wlistbox_notekey->addSelectionListener(this);
		//wlistbox_notekey->mListModel->insertElement(0,L"None");
		listbox_container->add(wlistbox_notekey);

		gcn::DivaEditorSlider *slider_listbox_items = new gcn::DivaEditorSlider(0,1);
		slider_listbox_items->setOrientation(gcn::DivaEditorSlider::VERTICAL);
		slider_listbox_items->setId("slider_listbox_items");
		slider_listbox_items->setSize(20, 240);
		slider_listbox_items->setPosition(listbox_container->getX() + listbox_container->getWidth()+10, listbox_container->getY());
		slider_listbox_items->setValue(1);
		sora::SoraGUI::Instance()->registerGUIResponser(slider_listbox_items, this, "slider_listbox_items", sora::RESPONSEACTION);
		keySoundControlSet->add(slider_listbox_items);

#pragma endregion key sound box set


		keySoundBox->add(btn_note_expandKeySoundBox);
		keySoundControlSet->setVisible(false);
		keySoundBox->setSize(btn_note_expandKeySoundBox->getWidth(),btn_note_expandKeySoundBox->getHeight());

		return noteCategory;
	}
	gcn::Container* DivaEditorMainScene::initShowCategory()
	{
		//Init Note Category
		gcn::Container *showCategory = new gcn::Container();
		showCategory->setSize(1280,720);
		showCategory->setOpaque(true);
		showCategory->setBaseColor(gcn::Color(0,0,0,0));

		ResourcePanel *resourcePanel = new ResourcePanel();
		resourcePanel->addSelectionListener(this);
		resourcePanel->setId("resourcePanel");
		resourcePanel->setBaseColor(gcn::Color(41,57,85,100));
		resourcePanel->setForegroundColor(gcn::Color(255,255,255,255));
		resourcePanel->setSize(500,500);
		resourcePanel->setElementGridSize(128);
		resourcePanel->adjustSize();
		showCategory->add(resourcePanel);


#pragma region resourcePanelControlContainer

		gcn::BoarderedContainer *resourcePanelControlContainer = new gcn::BoarderedContainer();
		resourcePanelControlContainer->setBoarderSize(3);
		resourcePanelControlContainer->setBaseColor(gcn::Color(16,22,33,200));
		resourcePanelControlContainer->setForegroundColor(gcn::Color(53,53,53,150));
		resourcePanelControlContainer->setSize(resourcePanel->getWidth(), 30);

		resourcePanel->setPosition(100,(showCategory->getHeight()-resourcePanel->getHeight()-resourcePanelControlContainer->getHeight())/2);
		resourcePanelControlContainer->setPosition(resourcePanel->getX(),resourcePanel->getY()+resourcePanel->getHeight());

		showCategory->add(resourcePanelControlContainer);

		int buttonSize = 23;

		gcn::WButton *btn_resourceLeftPage = new gcn::WButton(L"¡û");
		btn_resourceLeftPage->setId("btn_resourceLeftPage");
		btn_resourceLeftPage->setSize(buttonSize, buttonSize);
		btn_resourceLeftPage->setPosition(resourcePanelControlContainer->getWidth()/2-20-buttonSize,5);
		btn_resourceLeftPage->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_resourceLeftPage, this, "btn_resourceLeftPage", sora::RESPONSEACTION);
		resourcePanelControlContainer->add(btn_resourceLeftPage);

		gcn::WLabel *label_resourcePageNum = new gcn::WLabel();
		label_resourcePageNum->setId("label_resourcePageNum");
		label_resourcePageNum->setSize(20*2,buttonSize);
		label_resourcePageNum->setPosition(btn_resourceLeftPage->getX()+btn_resourceLeftPage->getWidth(),5);
		label_resourcePageNum->setAlignment(gcn::Graphics::Alignment::CENTER);
		label_resourcePageNum->setForegroundColor(gcn::Color(255,255,255,255));
		resourcePanelControlContainer->add(label_resourcePageNum);
			
		gcn::WButton *btn_resourceRightPage = new gcn::WButton(L"¡ú");
		btn_resourceRightPage->setId("btn_resourceRightPage");
		btn_resourceRightPage->setPosition(resourcePanelControlContainer->getWidth()/2+20,5);
		btn_resourceRightPage->setSize(buttonSize,buttonSize);
		btn_resourceRightPage->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_resourceRightPage, this, "btn_resourceRightPage", sora::RESPONSEACTION);
		resourcePanelControlContainer->add(btn_resourceRightPage);

		gcn::WButton *btn_resourceAdd = new gcn::WButton();
		btn_resourceAdd->setFrameSize(0);
		btn_resourceAdd->setNormalImage(MainSceneImageFile,gcn::Rectangle(339,363,23,23));
		btn_resourceAdd->setDownImage(MainSceneImageFile,gcn::Rectangle(408,363,23,23));
		btn_resourceAdd->setId("btn_resourceAdd");
		btn_resourceAdd->setPosition(resourcePanelControlContainer->getWidth()-buttonSize*2-10,5);
		btn_resourceAdd->setSize(buttonSize,buttonSize);
		btn_resourceAdd->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_resourceAdd, this, "btn_resourceAdd", sora::RESPONSEACTION);
		resourcePanelControlContainer->add(btn_resourceAdd);

		gcn::WButton *btn_resourceRemove = new gcn::WButton();
		btn_resourceRemove->setFrameSize(0);
		btn_resourceRemove->setNormalImage(MainSceneImageFile,gcn::Rectangle(362,363,23,23));
		btn_resourceRemove->setDownImage(MainSceneImageFile,gcn::Rectangle(431,363,23,23));
		btn_resourceRemove->setId("btn_resourceRemove");
		btn_resourceRemove->setPosition(btn_resourceAdd->getX()+btn_resourceAdd->getWidth()+5,5);
		btn_resourceRemove->setSize(buttonSize,buttonSize);
		btn_resourceRemove->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_resourceRemove, this, "btn_resourceRemove", sora::RESPONSEACTION);
		resourcePanelControlContainer->add(btn_resourceRemove);

#pragma endregion resourcePanelControlContainer

#pragma region resourceDetail

		gcn::BoarderedContainer *resourceDetailContainer = new gcn::BoarderedContainer();
		resourceDetailContainer->setBoarderSize(3);
		resourceDetailContainer->setBaseColor(gcn::Color(16,22,33,200));
		resourceDetailContainer->setForegroundColor(gcn::Color(53,53,53,150));
		resourceDetailContainer->setId("resourceDetailContainer");
		resourceDetailContainer->setSize(resourcePanel->getWidth()+210,resourcePanel->getHeight()/2);
		resourceDetailContainer->setPosition(resourcePanel->getX()+resourcePanel->getWidth()+20,resourcePanel->getY());
		resourceDetailContainer->setVisible(false);
		showCategory->add(resourceDetailContainer);

		//File Path
		gcn::WLabel *wlabel_resourceDetail_filePath = new gcn::WLabel(LOCALIZATION->getLocalStr(L"MainScene_wlabel_resourceDetail_filePath"));
		wlabel_resourceDetail_filePath->setId("wlabel_resourceDetail_filePath");
		wlabel_resourceDetail_filePath->setBaseColor(gcn::Color(0,0,0,0));
		wlabel_resourceDetail_filePath->setForegroundColor(gcn::Color(255,255,255,255));
		wlabel_resourceDetail_filePath->adjustSize();
		wlabel_resourceDetail_filePath->setPosition(100-wlabel_resourceDetail_filePath->getWidth(),20);
		resourceDetailContainer->add(wlabel_resourceDetail_filePath);

		gcn::WLabel *wlabel_resourceDetail_filePathValue = new gcn::WLabel();
		wlabel_resourceDetail_filePathValue->setId("wlabel_resourceDetail_filePathValue");
		wlabel_resourceDetail_filePathValue->setBaseColor(gcn::Color(0,0,0,0));
		wlabel_resourceDetail_filePathValue->setForegroundColor(gcn::Color(255,255,255,255));
		wlabel_resourceDetail_filePathValue->setPosition(100,wlabel_resourceDetail_filePath->getY());
		resourceDetailContainer->add(wlabel_resourceDetail_filePathValue);

		//File Type
		gcn::WLabel *wlabel_resourceDetail_fileType = new gcn::WLabel(LOCALIZATION->getLocalStr(L"MainScene_wlabel_resourceDetail_fileType"));
		wlabel_resourceDetail_fileType->setId("wlabel_resourceDetail_fileType");
		wlabel_resourceDetail_fileType->setBaseColor(gcn::Color(0,0,0,0));
		wlabel_resourceDetail_fileType->setForegroundColor(gcn::Color(255,255,255,255));
		wlabel_resourceDetail_fileType->adjustSize();
		wlabel_resourceDetail_fileType->setPosition(100-wlabel_resourceDetail_fileType->getWidth(),wlabel_resourceDetail_filePath->getY()+30);
		resourceDetailContainer->add(wlabel_resourceDetail_fileType);

		gcn::WLabel *wlabel_resourceDetail_fileTypeValue = new gcn::WLabel();
		wlabel_resourceDetail_fileTypeValue->setId("wlabel_resourceDetail_fileTypeValue");
		wlabel_resourceDetail_fileTypeValue->setBaseColor(gcn::Color(0,0,0,0));
		wlabel_resourceDetail_fileTypeValue->setForegroundColor(gcn::Color(255,255,255,255));
		wlabel_resourceDetail_fileTypeValue->setPosition(100,wlabel_resourceDetail_fileType->getY());
		resourceDetailContainer->add(wlabel_resourceDetail_fileTypeValue);

		//File Label
		gcn::WTextField *wtextField_resourceDetail_fileLabelValue = new gcn::WTextField();
		wtextField_resourceDetail_fileLabelValue->setId("wtextField_resourceDetail_fileLabelValue");
		wtextField_resourceDetail_fileLabelValue->setSize(450,20);
		wtextField_resourceDetail_fileLabelValue->setPosition(100,wlabel_resourceDetail_fileType->getY()+30);
		sora::SoraGUI::Instance()->registerGUIResponser(wtextField_resourceDetail_fileLabelValue, this, "wtextField_resourceDetail_fileLabelValue", sora::RESPONSEACTION);
		resourceDetailContainer->add(wtextField_resourceDetail_fileLabelValue);

		gcn::WLabel *wlabel_resourceDetail_fileLabel = new gcn::WLabel(LOCALIZATION->getLocalStr(L"MainScene_wlabel_resourceDetail_fileLabel"));
		wlabel_resourceDetail_fileLabel->setId("wlabel_resourceDetail_fileLabel");
		wlabel_resourceDetail_fileLabel->setBaseColor(gcn::Color(0,0,0,0));
		wlabel_resourceDetail_fileLabel->setForegroundColor(gcn::Color(255,255,255,255));
		wlabel_resourceDetail_fileLabel->adjustSize();
		wlabel_resourceDetail_fileLabel->setPosition(100-wlabel_resourceDetail_fileLabel->getWidth(),
													wtextField_resourceDetail_fileLabelValue->getY()+(wtextField_resourceDetail_fileLabelValue->getHeight()-wlabel_resourceDetail_fileLabel->getHeight())/2);
		resourceDetailContainer->add(wlabel_resourceDetail_fileLabel);


		//File events
		gcn::WLabel *wlabel_resourceDetail_fileEvents = new gcn::WLabel(LOCALIZATION->getLocalStr(L"MainScene_wlabel_resourceDetail_fileEvents"));
		wlabel_resourceDetail_fileEvents->setId("wlabel_resourceDetail_fileEvents");
		wlabel_resourceDetail_fileEvents->setBaseColor(gcn::Color(0,0,0,0));
		wlabel_resourceDetail_fileEvents->setForegroundColor(gcn::Color(255,255,255,255));
		wlabel_resourceDetail_fileEvents->adjustSize();
		wlabel_resourceDetail_fileEvents->setPosition(100-wlabel_resourceDetail_fileEvents->getWidth(),wlabel_resourceDetail_fileLabel->getY()+30);
		resourceDetailContainer->add(wlabel_resourceDetail_fileEvents);

		gcn::WListBox *wlistbox_resourceDetail_fileEventsValue = new gcn::WListBox(new WListModel());
		wlistbox_resourceDetail_fileEventsValue->setId("wlistbox_resourceDetail_fileEventsValue");
		wlistbox_resourceDetail_fileEventsValue->setSize(300,500);
		wlistbox_resourceDetail_fileEventsValue->setPosition(100,wlabel_resourceDetail_fileEvents->getY());
		wlistbox_resourceDetail_fileEventsValue->addSelectionListener(this);
		resourceDetailContainer->add(wlistbox_resourceDetail_fileEventsValue);

		gcn::WTextField *wtextField_resourceDetail_fileEventsPosition = new gcn::WTextField();
		wtextField_resourceDetail_fileEventsPosition->enableNumericMode(true);
		wtextField_resourceDetail_fileEventsPosition->setId("wtextField_resourceDetail_fileEventsPosition");
		wtextField_resourceDetail_fileEventsPosition->setSize(100,20);
		sora::SoraGUI::Instance()->registerGUIResponser(wtextField_resourceDetail_fileEventsPosition, this, "wtextField_resourceDetail_fileEventsPosition", sora::RESPONSEACTION);
		resourceDetailContainer->add(wtextField_resourceDetail_fileEventsPosition);

		gcn::WButton *btn_resourceDetail_addFileEvent = new gcn::WButton();
		btn_resourceDetail_addFileEvent->setFrameSize(0);
		btn_resourceDetail_addFileEvent->setNormalImage(MainSceneImageFile,gcn::Rectangle(339,363,23,23));
		btn_resourceDetail_addFileEvent->setDownImage(MainSceneImageFile,gcn::Rectangle(408,363,23,23));
		btn_resourceDetail_addFileEvent->setId("btn_resourceDetail_addFileEvent");
		btn_resourceDetail_addFileEvent->setSize(buttonSize,buttonSize);
		btn_resourceDetail_addFileEvent->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_resourceDetail_addFileEvent, this, "btn_resourceDetail_addFileEvent", sora::RESPONSEACTION);
		resourceDetailContainer->add(btn_resourceDetail_addFileEvent);

		gcn::WButton *btn_resourceDetail_removeFileEvent = new gcn::WButton();
		btn_resourceDetail_removeFileEvent->setFrameSize(0);
		btn_resourceDetail_removeFileEvent->setNormalImage(MainSceneImageFile,gcn::Rectangle(362,363,23,23));
		btn_resourceDetail_removeFileEvent->setDownImage(MainSceneImageFile,gcn::Rectangle(431,363,23,23));
		btn_resourceDetail_removeFileEvent->setId("btn_resourceDetail_removeFileEvent");
		btn_resourceDetail_removeFileEvent->setSize(buttonSize,buttonSize);
		btn_resourceDetail_removeFileEvent->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_resourceDetail_removeFileEvent, this, "btn_resourceDetail_removeFileEvent", sora::RESPONSEACTION);
		resourceDetailContainer->add(btn_resourceDetail_removeFileEvent);


#pragma endregion resourceDetail

		return showCategory;
	}
	gcn::Container* DivaEditorMainScene::initPlayCategory()
	{
		//Init Note Category
		gcn::Container *playCategory = new gcn::Container();
		playCategory->setSize(1280,720);
		playCategory->setOpaque(true);
		playCategory->setBaseColor(gcn::Color(0,0,0,0));

		return playCategory;
	}

	void DivaEditorMainScene::flowDidEnd(void* arg)
	{
		EDITOR_PTR->mapData->PauseAndResume();
		EDITOR_PTR->mapData->setPos(CORE_FLOW_PTR->getTotalPosition());

		
	}

	DivaEditorMainScene::DivaEditorMainScene()
	{
		GCN_GLOBAL->addIgnoreKey(gcn::Key::SPACE);
		GCN_GLOBAL->addIgnoreKey(gcn::Key::TAB);

		sceneIndex = Editor::State::MAIN;

		//Init Scene
		top = new Container();
		top->setBaseColor(gcn::Color(0,0,0,0));
		top->setOpaque(true);
		top->setSize(1280,720);

		//Tool Bar
		container_ToolBar = new Container();
		container_ToolBar->setSize(top->getWidth(),56);
		container_ToolBar->setBaseColor(gcn::Color(0,0,0,120));

#pragma region Category Buttons
		//Three Category Buttons
		gcn::WButton *btn_TimeLine = new gcn::WButton(LOCALIZATION->getLocalStr(L"MainScene_btn_TimeLine"));
		btn_TimeLine->setFrameSize(0);
		btn_TimeLine->setNormalImage(MainSceneImageFile,gcn::Rectangle(315,318,125,40));
		btn_TimeLine->setDownImage(MainSceneImageFile,gcn::Rectangle(0,363,125,40));
		btn_TimeLine->setId("btn_TimeLine");
		btn_TimeLine->setSize(125,40);
		btn_TimeLine->setPosition(8,7);
		btn_TimeLine->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_TimeLine, this, "btn_TimeLine", sora::RESPONSEACTION);
		container_ToolBar->add(btn_TimeLine);

		gcn::WButton *btn_Note = new gcn::WButton(LOCALIZATION->getLocalStr(L"MainScene_btn_Note"));
		btn_Note->setFrameSize(0);
		btn_Note->setNormalImage(MainSceneImageFile,gcn::Rectangle(315,318,125,40));
		btn_Note->setDownImage(MainSceneImageFile,gcn::Rectangle(0,363,125,40));
		btn_Note->setId("btn_Note");
		btn_Note->setSize(125,40);
		btn_Note->setPosition(btn_TimeLine->getX()+btn_TimeLine->getWidth()+3,7);
		btn_Note->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_Note, this, "btn_Note", sora::RESPONSEACTION);
		container_ToolBar->add(btn_Note);


		gcn::WButton *btn_Show = new gcn::WButton(LOCALIZATION->getLocalStr(L"MainScene_btn_Show"));
		btn_Show->setFrameSize(0);
		btn_Show->setNormalImage(MainSceneImageFile,gcn::Rectangle(315,318,125,40));
		btn_Show->setDownImage(MainSceneImageFile,gcn::Rectangle(0,363,125,40));
		btn_Show->setId("btn_Show");
		btn_Show->setSize(125,40);
		btn_Show->setPosition(btn_Note->getX()+btn_Note->getWidth()+3,7);
		btn_Show->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_Show, this, "btn_Show", sora::RESPONSEACTION);
		container_ToolBar->add(btn_Show);

		gcn::WButton *btn_Category_Play = new gcn::WButton(LOCALIZATION->getLocalStr(L"MainScene_btn_Category_Play"));
		btn_Category_Play->setFrameSize(0);
		btn_Category_Play->setNormalImage(MainSceneImageFile,gcn::Rectangle(315,318,125,40));
		btn_Category_Play->setDownImage(MainSceneImageFile,gcn::Rectangle(0,363,125,40));
		btn_Category_Play->setId("btn_Category_Play");
		btn_Category_Play->setSize(125,40);
		btn_Category_Play->setPosition(btn_Show->getX()+btn_Show->getWidth()+3,7);
		btn_Category_Play->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_Category_Play, this, "btn_Category_Play", sora::RESPONSEACTION);
		container_ToolBar->add(btn_Category_Play);

		top->add(container_ToolBar);

#pragma endregion Category Buttons
		
#pragma region Play Control And Timeline Widget

		//Add Total Time Progress
		DivaEditorMusicProgressWidget *progressBar = new DivaEditorMusicProgressWidget();
		progressBar->setId("progressBar");
		progressBar->setSize(277,35);
		progressBar->setPosition(677,10);
		progressBar->setBackgroundColor(gcn::Color(0,0,0,150));
		progressBar->setForegroundColor(gcn::Color(255,255,255,255));
		container_ToolBar->add(progressBar);


		gcn::WButton *btn_Pause = new gcn::WButton();
		btn_Pause->setFrameSize(0);
		btn_Pause->setNormalImage(MainSceneImageFile,gcn::Rectangle(225,273,45,45));
		btn_Pause->setDownImage(MainSceneImageFile,gcn::Rectangle(135,318,45,45));
		btn_Pause->setId("btn_Pause");
		btn_Pause->setSize(45,45);
		btn_Pause->setPosition(progressBar->getX()+progressBar->getWidth()+5,progressBar->getY()-5);
		btn_Pause->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_Pause, this, "btn_Pause", sora::RESPONSEACTION);
		top->add(btn_Pause);

		gcn::WButton *btn_Stop = new gcn::WButton();
		btn_Stop->setFrameSize(0);
		btn_Stop->setNormalImage(MainSceneImageFile,gcn::Rectangle(180,318,45,45));
		btn_Stop->setDownImage(MainSceneImageFile,gcn::Rectangle(45,318,45,45));
		btn_Stop->setId("btn_Stop");
		btn_Stop->setSize(btn_Pause->getWidth(),btn_Pause->getHeight());
		btn_Stop->setPosition(btn_Pause->getX()+btn_Pause->getWidth()+3,btn_Pause->getY());
		btn_Stop->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_Stop, this, "btn_Stop", sora::RESPONSEACTION);
		top->add(btn_Stop);


		gcn::WButton *btn_SpeedDown = new gcn::WButton();
		btn_SpeedDown->setFrameSize(0);
		btn_SpeedDown->setNormalImage(MainSceneImageFile,gcn::Rectangle(450,273,45,45));
		btn_SpeedDown->setDownImage(MainSceneImageFile,gcn::Rectangle(0,318,45,45));
		btn_SpeedDown->setId("btn_SpeedDown");
		btn_SpeedDown->setSize(btn_Pause->getWidth(),btn_Pause->getHeight());
		btn_SpeedDown->setPosition(btn_Stop->getX()+btn_Stop->getWidth()+3,btn_Stop->getY());
		btn_SpeedDown->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_SpeedDown, this, "btn_SpeedDown", sora::RESPONSEACTION);
		top->add(btn_SpeedDown);

		gcn::WButton *btn_SpeedUp = new gcn::WButton();
		btn_SpeedUp->setFrameSize(0);
		btn_SpeedUp->setNormalImage(MainSceneImageFile,gcn::Rectangle(135,273,45,45));
		btn_SpeedUp->setDownImage(MainSceneImageFile,gcn::Rectangle(225,318,45,45));
		btn_SpeedUp->setId("btn_SpeedUp");
		btn_SpeedUp->setSize(btn_Pause->getWidth(),btn_Pause->getHeight());
		btn_SpeedUp->setPosition(btn_SpeedDown->getX()+btn_SpeedDown->getWidth()+3,btn_SpeedDown->getY());
		btn_SpeedUp->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_SpeedUp, this, "btn_SpeedUp", sora::RESPONSEACTION);
		top->add(btn_SpeedUp);

		gcn::WLabel *wlabel_playTime = new gcn::WLabel();
		wlabel_playTime->setId("wlabel_playTime");
		wlabel_playTime->setBaseColor(gcn::Color(0,0,0,150));
		wlabel_playTime->setForegroundColor(gcn::Color(255,255,255,255));
		wlabel_playTime->setPosition(btn_SpeedUp->getX() + btn_SpeedUp->getWidth()+5,progressBar->getY());
		top->add(wlabel_playTime);

		gcn::WLabel *wlabel_playPos = new gcn::WLabel();
		wlabel_playPos->setId("wlabel_playPos");
		wlabel_playPos->setBaseColor(gcn::Color(0,0,0,150));
		wlabel_playPos->setForegroundColor(gcn::Color(255,255,255,255));
		wlabel_playPos->setPosition(wlabel_playTime->getX(),wlabel_playTime->getY()+15);
		top->add(wlabel_playPos);

		gcn::WButton *btn_SaveTo = new gcn::WButton(LOCALIZATION->getLocalStr(L"MainScene_btn_SaveTo"));
		btn_SaveTo->setFrameSize(0);
		btn_SaveTo->setNormalImage(MainSceneImageFile,gcn::Rectangle(97,403,97,22));
		btn_SaveTo->setDownImage(MainSceneImageFile,gcn::Rectangle(0,403,97,22));
		btn_SaveTo->setId("btn_SaveTo");
		btn_SaveTo->setSize(97,22);
		btn_SaveTo->setPosition(top->getWidth() - btn_SaveTo->getWidth()-5, top->getHeight()-btn_SaveTo->getHeight()-5);
		btn_SaveTo->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_SaveTo, this, "btn_SaveTo", sora::RESPONSEACTION);
		top->add(btn_SaveTo);

		gcn::WButton *btn_Save = new gcn::WButton(LOCALIZATION->getLocalStr(L"MainScene_btn_Save"));
		btn_Save->setFrameSize(0);
		btn_Save->setNormalImage(MainSceneImageFile,gcn::Rectangle(97,403,97,22));
		btn_Save->setDownImage(MainSceneImageFile,gcn::Rectangle(0,403,97,22));
		btn_Save->setId("btn_Save");
		btn_Save->setSize(97,22);
		btn_Save->setPosition(top->getWidth() - btn_Save->getWidth()-5, btn_SaveTo->getY()-btn_Save->getHeight()-5);
		btn_Save->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_Save, this, "btn_Save", sora::RESPONSEACTION);
		top->add(btn_Save);

		gcn::WCheckBox *wcheckbox_autosave = new gcn::WCheckBox(LOCALIZATION->getLocalStr(L"MainScene_wcheckbox_autosave"));
		wcheckbox_autosave->setSelected(true);
		wcheckbox_autosave->setId("wcheckbox_autosave");
		wcheckbox_autosave->adjustSize();
		wcheckbox_autosave->setForegroundColor(gcn::Color(255,255,255,255));
		wcheckbox_autosave->setPosition(btn_Save->getX(),btn_Save->getY() - wcheckbox_autosave->getHeight()-2);
		sora::SoraGUI::Instance()->registerGUIResponser(wcheckbox_autosave, this, "wcheckbox_autosave", sora::RESPONSEACTION);
		top->add(wcheckbox_autosave);


		Timeline *timeline = new Timeline();
		timeline->setId("timeline_TimeLine");
		timeline->setSize(1100,80);
		timeline->setPosition(20,635);
		timeline->setBackgroundColor(gcn::Color(0,0,0,120));
		timeline->setForegroundColor(gcn::Color(255,255,255,255));
		timeline->setMaxGridHeightFactor(0.2);
		top->add(timeline);

		gcn::WButton *btn_TimeLine_wider = new gcn::WButton();
		btn_TimeLine_wider->setFrameSize(0);
		btn_TimeLine_wider->setNormalImage(MainSceneImageFile,gcn::Rectangle(293,363,23,23));
		btn_TimeLine_wider->setDownImage(MainSceneImageFile,gcn::Rectangle(224,363,23,23));
		btn_TimeLine_wider->setId("btn_TimeLine_wider");
		btn_TimeLine_wider->setSize(23,23);
		btn_TimeLine_wider->setPosition(timeline->getX()+timeline->getWidth()+3,top->getHeight()-5-btn_TimeLine_wider->getHeight()*2-1);
		//btn_TimeLine_wider->setPosition(1120+3,top->getHeight()-5-btn_TimeLine_wider->getHeight()*2-1);
		btn_TimeLine_wider->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_TimeLine_wider, this, "btn_TimeLine_wider", sora::RESPONSEACTION);
		top->add(btn_TimeLine_wider);

		gcn::WButton *btn_TimeLine_tighter = new gcn::WButton();
		btn_TimeLine_tighter->setFrameSize(0);
		btn_TimeLine_tighter->setNormalImage(MainSceneImageFile,gcn::Rectangle(247,363,23,23));
		btn_TimeLine_tighter->setDownImage(MainSceneImageFile,gcn::Rectangle(270,363,23,23));
		btn_TimeLine_tighter->setId("btn_TimeLine_tighter");
		btn_TimeLine_tighter->setSize(23,23);
		btn_TimeLine_tighter->setPosition(btn_TimeLine_wider->getX(),btn_TimeLine_wider->getY()+btn_TimeLine_wider->getHeight()+1);
		btn_TimeLine_tighter->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_TimeLine_tighter, this, "btn_TimeLine_tighter", sora::RESPONSEACTION);
		top->add(btn_TimeLine_tighter);

		gcn::WButton *btn_TimeLine_deeper = new gcn::WButton();
		btn_TimeLine_deeper->setFrameSize(0);
		btn_TimeLine_deeper->setNormalImage(MainSceneImageFile,gcn::Rectangle(201,363,23,23));
		btn_TimeLine_deeper->setDownImage(MainSceneImageFile,gcn::Rectangle(155,363,23,23));
		btn_TimeLine_deeper->setId("btn_TimeLine_deeper");
		btn_TimeLine_deeper->setSize(23,23);
		btn_TimeLine_deeper->setPosition(btn_TimeLine_wider->getX()+btn_TimeLine_wider->getWidth()+2,btn_TimeLine_wider->getY());
		btn_TimeLine_deeper->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_TimeLine_deeper, this, "btn_TimeLine_deeper", sora::RESPONSEACTION);
		top->add(btn_TimeLine_deeper);

		gcn::WButton *btn_TimeLine_lighter = new gcn::WButton();
		btn_TimeLine_lighter->setFrameSize(0);
		btn_TimeLine_lighter->setNormalImage(MainSceneImageFile,gcn::Rectangle(178,363,23,23));
		btn_TimeLine_lighter->setDownImage(MainSceneImageFile,gcn::Rectangle(132,363,23,23));
		btn_TimeLine_lighter->setId("btn_TimeLine_lighter");
		btn_TimeLine_lighter->setSize(23,23);
		btn_TimeLine_lighter->setPosition(btn_TimeLine_deeper->getX(),btn_TimeLine_deeper->getY()+btn_TimeLine_deeper->getHeight()+1);
		btn_TimeLine_lighter->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_TimeLine_lighter, this, "btn_TimeLine_lighter", sora::RESPONSEACTION);
		top->add(btn_TimeLine_lighter);

				
#ifdef EDITOR_PICKMEUP
		gcn::WButton *btn_SortNote = new gcn::WButton();
		btn_SortNote->setId("btn_SortNote");
		btn_SortNote->setCaption(L"Sort");
		btn_SortNote->setSize(48,23);
		btn_SortNote->setPosition(btn_TimeLine_wider->getX(),btn_TimeLine_wider->getY() - btn_SortNote->getHeight() - 2);
		btn_SortNote->setForegroundColor(gcn::Color(255,255,255,255));
		sora::SoraGUI::Instance()->registerGUIResponser(btn_SortNote, this, "btn_SortNote", sora::RESPONSEACTION);
		top->add(btn_SortNote);
#endif

#pragma endregion Play Control And Timeline Widget

#pragma region Layer Toggle

		gcn::WCheckBox *wcheckbox_showNote = new gcn::WCheckBox(LOCALIZATION->getLocalStr(L"MainScene_wcheckbox_showNote"));
		wcheckbox_showNote->setId("wcheckbox_showNote");
		wcheckbox_showNote->adjustSize();
		wcheckbox_showNote->setPosition(btn_Category_Play->getX()+btn_Category_Play->getWidth()+5,2);
		wcheckbox_showNote->setBaseColor(gcn::Color(0,0,0,150));
		wcheckbox_showNote->setForegroundColor(gcn::Color(255,255,255,255));
		wcheckbox_showNote->setCheckForeGroundColor(gcn::Color(0,0,0,255));
		sora::SoraGUI::Instance()->registerGUIResponser(wcheckbox_showNote, this, "wcheckbox_showNote", sora::RESPONSEACTION);
		top->add(wcheckbox_showNote);

		gcn::WCheckBox *wcheckbox_showGrid = new gcn::WCheckBox(LOCALIZATION->getLocalStr(L"MainScene_wcheckbox_showGrid"));
		wcheckbox_showGrid->setId("wcheckbox_showGrid");
		wcheckbox_showGrid->adjustSize();
		wcheckbox_showGrid->setPosition(wcheckbox_showNote->getX(),wcheckbox_showNote->getY()+wcheckbox_showNote->getHeight()+1);
		wcheckbox_showGrid->setBaseColor(gcn::Color(0,0,0,150));
		wcheckbox_showGrid->setForegroundColor(gcn::Color(255,255,255,255));
		wcheckbox_showGrid->setCheckForeGroundColor(gcn::Color(0,0,0,255));
		sora::SoraGUI::Instance()->registerGUIResponser(wcheckbox_showGrid, this, "wcheckbox_showGrid", sora::RESPONSEACTION);
		top->add(wcheckbox_showGrid);

		gcn::WCheckBox *wcheckbox_showBackground = new gcn::WCheckBox(LOCALIZATION->getLocalStr(L"MainScene_wcheckbox_showBackground"));
		wcheckbox_showBackground->setId("wcheckbox_showBackground");
		wcheckbox_showBackground->adjustSize();
		wcheckbox_showBackground->setPosition(wcheckbox_showGrid->getX(),wcheckbox_showGrid->getY()+wcheckbox_showGrid->getHeight()+1);
		wcheckbox_showBackground->setBaseColor(gcn::Color(0,0,0,150));
		wcheckbox_showBackground->setForegroundColor(gcn::Color(255,255,255,255));
		wcheckbox_showBackground->setCheckForeGroundColor(gcn::Color(0,0,0,255));
		sora::SoraGUI::Instance()->registerGUIResponser(wcheckbox_showBackground, this, "wcheckbox_showBackground", sora::RESPONSEACTION);
		top->add(wcheckbox_showBackground);

#pragma endregion Layer Toggle

#pragma region Note Placement State

		gcn::WLabel *wlabel_nowPlaceNoteCategory = new gcn::WLabel();
		wlabel_nowPlaceNoteCategory->setId("wlabel_nowPlaceNoteCategory");
		wlabel_nowPlaceNoteCategory->setPosition(wcheckbox_showBackground->getX(),wcheckbox_showBackground->getY()+wcheckbox_showBackground->getHeight()+10);
		wlabel_nowPlaceNoteCategory->setBaseColor(gcn::Color(0,0,0,150));
		wlabel_nowPlaceNoteCategory->setForegroundColor(gcn::Color(255,255,255,255));
		top->add(wlabel_nowPlaceNoteCategory);

#pragma endregion Note Placement State

		container_Categories[State::TIMELINE] = initTimelineCategory();
		container_Categories[State::NOTE] = initNoteCategory();
		container_Categories[State::SHOW] = initShowCategory();
		container_Categories[State::PREVIEW] = initPlayCategory();


		for (std::map<State,gcn::Container*>::iterator i=container_Categories.begin();i!=container_Categories.end();i++)
		{
			top->add(i->second);
			top->moveToBottom(i->second);
		}


		Task task;
		task.setAsMemberFunc(&DivaEditorMainScene::flowDidEnd, this);
		CORE_FLOW_PTR->registerEndCallback(task);

		ChangeState(State::TIMELINE);

		rhythm_LastPos=0;
	}

	void DivaEditorMainScene::ChangeState(State state)
	{
		if(container_Categories.find(state)==container_Categories.end())
			return;
		for (std::map<State,gcn::Container*>::iterator i=container_Categories.begin();i!=container_Categories.end();i++)
			i->second->setVisible(false);
		container_Categories[state]->setVisible(true);
		nowState=state;

#ifdef EDITOR_DIVA
		EDITCONFIG->display_grid=true;
#else
		EDITCONFIG->display_grid=false;
#endif
		EDITCONFIG->display_note=true;

		EDITCONFIG->display_background=true;

		gcn::WLabel *wlabel_nowPlaceNoteCategory = (gcn::WLabel*)top->findWidgetById("wlabel_nowPlaceNoteCategory");
		wlabel_nowPlaceNoteCategory->setVisible(false);

		Timeline* timeline = (Timeline*)top->findWidgetById("timeline_TimeLine");
		timeline->setEnabled(false);

		((divacore::EditMode*)CORE_PTR->getGameMode())->setSound(true,true);


		gcn::WButton *btn_TimeLine = (gcn::WButton*)top->findWidgetById("btn_TimeLine");
		gcn::WButton *btn_Note = (gcn::WButton*)top->findWidgetById("btn_Note");
		gcn::WButton *btn_Show = (gcn::WButton*)top->findWidgetById("btn_Show");
		gcn::WButton *btn_Category_Play = (gcn::WButton*)top->findWidgetById("btn_Category_Play");
		btn_TimeLine->setChecked(false);
		btn_Note->setChecked(false);
		btn_Show->setChecked(false);
		btn_Category_Play->setChecked(false);

		if(nowState==State::TIMELINE)
		{
			btn_TimeLine->setChecked(true);
			EDITCONFIG->display_grid=false;
			((divacore::EditMode*)CORE_PTR->getGameMode())->setSound(false,false);
		}
		else if(nowState==State::NOTE)
		{
			btn_Note->setChecked(true);
			timeline->setEnabled(true);
			wlabel_nowPlaceNoteCategory->setVisible(true);
		}
		else if(nowState==State::SHOW)
		{
			btn_Show->setChecked(true);
			EDITCONFIG->display_grid=false;
			EDITCONFIG->display_note=false;
			refreshResourcePanelDetail();
			refreshEventDetail();
		}
		else if(nowState==State::PREVIEW)
		{
			btn_Category_Play->setChecked(true);
			EDITCONFIG->display_grid=false;
		}
		
		((divacore::EditMode*)CORE_PTR->getGameMode())->setPlayble(nowState==State::PREVIEW);

	}

	void DivaEditorMainScene::onUpdate(float dt)
	{
		//Play rhythm sound
		double thisPosition = CORE_PTR->getRunPosition();
		
		if(nowState == TIMELINE && CORE_FLOW_PTR->getState() == CoreFlow::RUN)
		{
			gcn::DivaEditorSlider* slider_timeline_offset = (gcn::DivaEditorSlider*)container_Categories[nowState]->findWidgetById("slider_timeline_offset");

			int beatCrossNum = EDITOR_PTR->mapData->getCrossAStandardBeatPos(rhythm_LastPos,thisPosition,slider_timeline_offset->getValue());
			if(beatCrossNum)
				CORE_PTR->getMusicManager()->playDirectWithFile("Data/tick_" + iToS(beatCrossNum) + ".wav",true);
		}
		rhythm_LastPos = thisPosition;


		gcn::WButton *btn_Pause = (gcn::WButton*)top->findWidgetById("btn_Pause");
		if(CORE_FLOW_PTR->getState() == CoreFlow::PAUSE)
		{
			btn_Pause->setNormalImage(L"",gcn::Rectangle(225,273,45,45));
			btn_Pause->setDownImage(L"",gcn::Rectangle(135,318,45,45));
		}
		else
		{
			btn_Pause->setNormalImage(L"",gcn::Rectangle(180,273,45,45));
			btn_Pause->setDownImage(L"",gcn::Rectangle(270,318,45,45));
		}


		gcn::WCheckBox *wcheckbox_showNote = (gcn::WCheckBox*)top->findWidgetById("wcheckbox_showNote");
		gcn::WCheckBox *wcheckbox_showGrid = (gcn::WCheckBox*)top->findWidgetById("wcheckbox_showGrid");
		gcn::WCheckBox *wcheckbox_showBackground = (gcn::WCheckBox*)top->findWidgetById("wcheckbox_showBackground");
		wcheckbox_showNote->setSelected(EDITCONFIG->display_note);
		wcheckbox_showGrid->setSelected(EDITCONFIG->display_grid);
		wcheckbox_showBackground->setSelected(EDITCONFIG->display_background);

		gcn::WLabel *wlabel_playTime = (gcn::WLabel*)top->findWidgetById("wlabel_playTime");
		wlabel_playTime->setCaption( secondToTimeWstr(CORE_PTR->getRunTime()) + L'/' + secondToTimeWstr(CORE_FLOW_PTR->getTotalTime()) + L" " +  fTows(CORE_PTR->getSpeedScale(),2) + L"x");
		wlabel_playTime->adjustSize();

		gcn::WLabel *wlabel_playPos = (gcn::WLabel*)top->findWidgetById("wlabel_playPos");
		wlabel_playPos->setCaption(iToWS(CORE_PTR->getRunPosition()) + L'/' + iToWS(CORE_FLOW_PTR->getTotalPosition())+LOCALIZATION->getLocalStr(L"Note_PosUnit"));
		wlabel_playPos->adjustSize();

		if(nowState==TIMELINE)
		{
			gcn::WTextField* txtField_timeline_BPM = (gcn::WTextField*)container_Categories[nowState]->findWidgetById("txtField_timeline_BPM");
			gcn::WTextField* txtField_timeline_offSet = (gcn::WTextField*)container_Categories[nowState]->findWidgetById("txtField_timeline_offSet");
			gcn::WTextField* txtField_timeline_Stop = (gcn::WTextField*)container_Categories[nowState]->findWidgetById("txtField_timeline_Stop");
			gcn::WTextField* txtField_timeline_BeatNum = (gcn::WTextField*)container_Categories[nowState]->findWidgetById("txtField_timeline_BeatNum");
			gcn::WTextField* txtField_timeline_TailSpeed = (gcn::WTextField*)container_Categories[nowState]->findWidgetById("txtField_timeline_TailSpeed");

			gcn::WButton* btn_TimeLine_changeOffset = (gcn::WButton*)container_Categories[nowState]->findWidgetById("btn_TimeLine_changeOffset");

			gcn::WButton* btn_TimeLine_changeBPM = (gcn::WButton*)container_Categories[nowState]->findWidgetById("btn_TimeLine_changeBPM");
			gcn::WButton* btn_TimeLine_insertBPM = (gcn::WButton*)container_Categories[nowState]->findWidgetById("btn_TimeLine_insertBPM");
			gcn::WButton* btn_TimeLine_deleteBPM = (gcn::WButton*)container_Categories[nowState]->findWidgetById("btn_TimeLine_deleteBPM");

			gcn::WButton* btn_TimeLine_changeStop = (gcn::WButton*)container_Categories[nowState]->findWidgetById("btn_TimeLine_changeStop");
			gcn::WButton* btn_TimeLine_insertStop = (gcn::WButton*)container_Categories[nowState]->findWidgetById("btn_TimeLine_insertStop");
			gcn::WButton* btn_TimeLine_deleteStop = (gcn::WButton*)container_Categories[nowState]->findWidgetById("btn_TimeLine_deleteStop");

			gcn::WButton* btn_TimeLine_changeBeatNum = (gcn::WButton*)container_Categories[nowState]->findWidgetById("btn_TimeLine_changeBeatNum");
			gcn::WButton* btn_TimeLine_deleteBeatNum = (gcn::WButton*)container_Categories[nowState]->findWidgetById("btn_TimeLine_deleteBeatNum");

			gcn::WButton* btn_TimeLine_changeTailSpeed = (gcn::WButton*)container_Categories[nowState]->findWidgetById("btn_TimeLine_changeTailSpeed");

			gcn::DivaEditorSlider* slider_timeline_offset = (gcn::DivaEditorSlider*)container_Categories[nowState]->findWidgetById("slider_timeline_offset");

			if(!(txtField_timeline_BPM->isFocused()||btn_TimeLine_changeBPM->isFocused()||btn_TimeLine_insertBPM->isFocused()||btn_TimeLine_deleteBPM->isFocused()))
			{
				txtField_timeline_BPM->setText(dTows(EDITOR_PTR->mapData->getBPM(CORE_PTR->getRunPosition()),3));
			}
			if(!(txtField_timeline_offSet->isFocused()||btn_TimeLine_changeOffset->isFocused()))
			{
				txtField_timeline_offSet->setText(iToWS(EDITOR_PTR->mapData->getOffset()));
			}
			if(!(slider_timeline_offset->isFocused()))
			{
				slider_timeline_offset->setValue(EDITOR_PTR->mapData->getOffset());
			}
			else if(slider_timeline_offset->isFocused())
			{
				txtField_timeline_offSet->setText(iToWS(slider_timeline_offset->getValue()));
			}
			if(!(txtField_timeline_Stop->isFocused()||btn_TimeLine_changeStop->isFocused()||btn_TimeLine_insertStop->isFocused()||btn_TimeLine_deleteStop->isFocused()))
			{
				txtField_timeline_Stop->setText(iToWS(EDITOR_PTR->mapData->getStop(CORE_PTR->getRunPosition())));
			}
			if(EDITOR_PTR->mapData->getStop(CORE_PTR->getRunPosition())!=0)
			{
				btn_TimeLine_insertStop->setVisible(false);
				btn_TimeLine_changeStop->setVisible(true);
				btn_TimeLine_deleteStop->setVisible(true);
			}
			else
			{
				btn_TimeLine_insertStop->setVisible(true);
				btn_TimeLine_changeStop->setVisible(false);
				btn_TimeLine_deleteStop->setVisible(false);
			}
			if(!(txtField_timeline_BeatNum->isFocused()||btn_TimeLine_changeBeatNum->isFocused()||btn_TimeLine_deleteBeatNum->isFocused()))
			{
				txtField_timeline_BeatNum->setText(iToWS(EDITOR_PTR->mapData->getBeatNum(CORE_PTR->getRunPosition())));
			}
			if(!(txtField_timeline_TailSpeed->isFocused()||btn_TimeLine_changeTailSpeed->isFocused()))
			{
				txtField_timeline_TailSpeed->setText(fTows(EDITOR_PTR->mapData->coreInfoPtr->header.speedScale,1));
			}
		}
		else if(nowState==NOTE)
		{
			gcn::WLabel *wlabel_nowPlaceNoteCategory = (gcn::WLabel*)top->findWidgetById("wlabel_nowPlaceNoteCategory");

			if(EDITCONFIG->EDITSTATE_NOTESTATE == EditorConfig::NORMAL)
				wlabel_nowPlaceNoteCategory->setCaption(LOCALIZATION->getLocalStr(L"Note_NormalNote"));
			else if(EDITCONFIG->EDITSTATE_NOTESTATE == EditorConfig::LONG)
				wlabel_nowPlaceNoteCategory->setCaption(LOCALIZATION->getLocalStr(L"Note_LongNote"));
			else if(EDITCONFIG->EDITSTATE_NOTESTATE == EditorConfig::COMBO)
				wlabel_nowPlaceNoteCategory->setCaption(LOCALIZATION->getLocalStr(L"Note_ComboNote"));
			
			wlabel_nowPlaceNoteCategory->adjustSize();



			gcn::DivaEditorSlider* slider_listbox_items = (gcn::DivaEditorSlider*)container_Categories[State::NOTE]->findWidgetById("slider_listbox_items");
			gcn::WListBox *wlistbox_notekey = (gcn::WListBox*)container_Categories[State::NOTE]->findWidgetById("wlistbox_notekey");
			gcn::Container *listbox_container = (gcn::Container*)container_Categories[State::NOTE]->findWidgetById("listbox_container");
			int heightDelta = wlistbox_notekey->getHeight() - listbox_container->getHeight();
			if(heightDelta<0) heightDelta = 0;
			int nowDelta = -(int)((float)heightDelta * (1-slider_listbox_items->getValue()));
			if(nowDelta==0)
				wlistbox_notekey->setPosition(wlistbox_notekey->getX(), nowDelta);
			else
				wlistbox_notekey->setPosition(wlistbox_notekey->getX(), nowDelta);
		}
		else if(nowState==SHOW)
		{
			ResourcePanel *resourcePanel = (ResourcePanel*)container_Categories[nowState]->findWidgetById("resourcePanel");
			

			gcn::WLabel *label_resourcePageNum = (gcn::WLabel*)container_Categories[nowState]->findWidgetById("label_resourcePageNum");
			label_resourcePageNum->setCaption(iToWS(resourcePanel->_nowPage+1)+L"/"+iToWS((EDITOR_PTR->mapData->coreInfoPtr->resources.size()-1)/resourcePanel->_gridPerPage+1));
			
			

			if(resourcePanel->getSelectedIndex()!=-1)
			{
				gcn::WTextField *wtextField_resourceDetail_fileLabelValue = (gcn::WTextField*)container_Categories[nowState]->findWidgetById("wtextField_resourceDetail_fileLabelValue");
				if(!wtextField_resourceDetail_fileLabelValue->isFocused())
					wtextField_resourceDetail_fileLabelValue->setText(EDITOR_PTR->mapData->getResourceDescriptionByIndex(resourcePanel->getSelectedIndex()));

				std::string selectedResourceID = EDITOR_PTR->mapData->findResourceIDByIndex(resourcePanel->getSelectedIndex());

				gcn::WListBox *wlistbox_resourceDetail_fileEventsValue = (WListBox*)container_Categories[State::SHOW]->findWidgetById("wlistbox_resourceDetail_fileEventsValue");
				gcn::WTextField *wtextField_resourceDetail_fileEventsPosition = (gcn::WTextField*)container_Categories[nowState]->findWidgetById("wtextField_resourceDetail_fileEventsPosition");

				if(wlistbox_resourceDetail_fileEventsValue->getSelected()!=-1 && !wtextField_resourceDetail_fileEventsPosition->isFocused())
				{
					wtextField_resourceDetail_fileEventsPosition->setText( iToWS(EDITOR_PTR->mapData->coreInfoPtr->events[
						EDITOR_PTR->mapData->findResourceEventIndexByIndexInResource(wlistbox_resourceDetail_fileEventsValue->getSelected(),
							selectedResourceID)].position) );
				}
			}
		}

		//check auto update
		if(EDITCONFIG->isAutoSave)
		{
			EDITCONFIG->lastSavedTimeDelta += dt;
			if(EDITCONFIG->lastSavedTimeDelta > EDITCONFIG->AutoSaveTime)
			{
				EDITCONFIG->lastSavedTimeDelta = 0;
				EDITOR_PTR->mapData->SaveFile();
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

		//After map initialized here
		//Check if the map has key and miss resource
		//If not, add default key sound

		EDITCONFIG->mainScene = this;

		if(EDITOR_PTR->mapData->coreInfoPtr->resources.find("hit")==EDITOR_PTR->mapData->coreInfoPtr->resources.end())
		{
			EDITOR_PTR->mapData->modifyGlobalHitMissSound(L"Data/hit.wav","hit");
		}
		gcn::WLabel *wlabel_globalHitSound = (gcn::WLabel*)container_Categories[State::NOTE]->findWidgetById("wlabel_globalHitSound");
		std::wstring hitPath = EDITOR_PTR->mapData->coreInfoPtr->resources["hit"].filePath;
		wlabel_globalHitSound->setCaption(LOCALIZATION->getLocalStr(L"MainScene_wlabel_globalHitSound") + L": " + (hitPath==L""?LOCALIZATION->getLocalStr(L"GlobalSoundDisabled"):hitPath));
		wlabel_globalHitSound->adjustSize();

		if(EDITOR_PTR->mapData->coreInfoPtr->resources.find("miss")==EDITOR_PTR->mapData->coreInfoPtr->resources.end())
		{
			EDITOR_PTR->mapData->modifyGlobalHitMissSound(L"Data/miss.wav","miss");
		}
		gcn::WLabel *wlabel_globalMissSound = (gcn::WLabel*)container_Categories[State::NOTE]->findWidgetById("wlabel_globalMissSound");
		std::wstring missPath = EDITOR_PTR->mapData->coreInfoPtr->resources["miss"].filePath;
		wlabel_globalMissSound->setCaption(LOCALIZATION->getLocalStr(L"MainScene_wlabel_globalMissSound") + L": " + (missPath==L""?LOCALIZATION->getLocalStr(L"GlobalSoundDisabled"):missPath));
		wlabel_globalMissSound->adjustSize();

		refreshKeySoundList();

		EditUtility.refreshAll();
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

		float nowTimePos = CORE_PTR->getRunPosition();

#pragma region Playback Control


		if(getID()=="btn_Pause")
		{
			EDITOR_PTR->mapData->PauseAndResume();
		}
		else if(getID()=="btn_Stop")
		{
			EDITOR_PTR->mapData->stop();
		}
		else if(getID()=="btn_SpeedUp")
		{
			float nowSpeed = CORE_PTR->getSpeedScale();
			if(nowSpeed<2)
			{
				nowSpeed += 0.25;
				CORE_PTR->setSpeedScale(nowSpeed);
				EditUtility.refreshAll();
			}
		}
		else if(getID()=="btn_SpeedDown")
		{
			float nowSpeed = CORE_PTR->getSpeedScale();
			if(nowSpeed>0.3)
			{
				nowSpeed -= 0.25;
				CORE_PTR->setSpeedScale(nowSpeed);
				EditUtility.refreshAll();
			}
		}

#pragma endregion Playback Control

#pragma region Category Change

		else if(getID() == "btn_TimeLine") 
		{
			ChangeState(State::TIMELINE);
		}
		else if(getID() == "btn_Note")
		{
			ChangeState(State::NOTE);
		}
		else if(getID() == "btn_Show")
		{
			ChangeState(State::SHOW);
		}
		else if(getID()=="btn_Category_Play")
		{
			ChangeState(State::PREVIEW);
		}
#pragma endregion Category Change

#pragma region TimeLine Widget Control

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
		else if(getID() == "btn_SortNote")
		{
			DivaEditorOperationSet *thisModifySet = new DivaEditorOperationSet();
			for(int i=0;i<EDITOR_PTR->mapData->coreInfoPtr->notes.size();i++)
				thisModifySet->addOperation(new DivaEditorOperation_ModifyNote(i,DivaEditorOperation_ModifyNote::SORTFORPICKMEUP));
			EDITCONFIG->addAndDoOperation(thisModifySet);
		}

#pragma endregion TimeLine Widget Control

#pragma region Timeline Category Actions

		else if(getID() == "wcheckbox_timeline_moveNote")
		{
			gcn::WCheckBox *wcheckbox_timeline_moveNote = (gcn::WCheckBox*)top->findWidgetById("wcheckbox_timeline_moveNote");
			EDITCONFIG->config_moveNoteAsWell = wcheckbox_timeline_moveNote->isSelected();
		}
		//BPM
		else if(getID() == "txtField_timeline_BPM" || getID() == "btn_TimeLine_changeBPM")
		{
			DivaEditorOperationSet *thisBPMOperationSet = new DivaEditorOperationSet();
			thisBPMOperationSet->needToRecalcTime=true;
			thisBPMOperationSet->needToPause=true;

			int thisBPMPos = EDITOR_PTR->mapData->getBPMPos(nowTimePos),
				nextBPMPos = EDITOR_PTR->mapData->getNextBPMPos(nowTimePos);
			double oldBPM = EDITOR_PTR->mapData->getBPM(nowTimePos),
				newBPM = ((WTextField*)container_Categories[State::TIMELINE]->findWidgetById("txtField_timeline_BPM"))->getDouble();

			if(newBPM<10) newBPM=10;
			else if(newBPM>999) newBPM=999;

			if(abs(newBPM-oldBPM)<1e-6) return;

			bool needReverse = newBPM>oldBPM;

			if(EDITCONFIG->config_moveNoteAsWell)
			{
				for (int i=needReverse?(EDITOR_PTR->mapData->coreInfoPtr->notes.size()-1):0;needReverse?(i>=0):(i<EDITOR_PTR->mapData->coreInfoPtr->notes.size());needReverse?(i--):(i++))
				{
					divacore::MapNote &thisNote = EDITOR_PTR->mapData->coreInfoPtr->notes[i];
					if(thisNote.notePoint[0].position>thisBPMPos ||
						(thisNote.notePoint.size()>1 && thisNote.notePoint[1].position>thisBPMPos))
						thisBPMOperationSet->addOperation(new DivaEditorOperation_ModifyNote(i,thisBPMPos,nextBPMPos,oldBPM,newBPM));
				}

				for(int i=needReverse?(EDITOR_PTR->mapData->coreInfoPtr->events.size()-1):0;needReverse?(i>=0):(i<EDITOR_PTR->mapData->coreInfoPtr->events.size());needReverse?(i--):(i++))
				{
					divacore::MapEvent &thisEvent = EDITOR_PTR->mapData->coreInfoPtr->events[i];
					if(thisEvent.position > thisBPMPos)
						thisBPMOperationSet->addOperation(new DivaEditorOperation_ModifyEvent(i,thisBPMPos,nextBPMPos,oldBPM,newBPM));
				}
			}

			thisBPMOperationSet->addOperation(new DivaEditorOperation_BPM(oldBPM,
						newBPM,
						thisBPMPos,
						DivaEditorOperation_BPM::CHANGEBPM));

			EDITCONFIG->addAndDoOperation(thisBPMOperationSet);
			
			actionWidget->setFocusable(false);
			actionWidget->setFocusable(true);
		}
		else if(getID() == "btn_TimeLine_insertBPM")
		{
			DivaEditorOperationSet *thisBPMOperationSet = new DivaEditorOperationSet();
			thisBPMOperationSet->needToRecalcTime=true;
			thisBPMOperationSet->needToPause=true;

			int thisBPMPos = EDITOR_PTR->mapData->getNearestStandardGrid(nowTimePos, EDITCONFIG->getGridToShowPerBeat()),
				nextBPMPos = EDITOR_PTR->mapData->getNextBPMPos(nowTimePos);
			double oldBPM = EDITOR_PTR->mapData->getBPM(nowTimePos),
				newBPM = ((WTextField*)container_Categories[State::TIMELINE]->findWidgetById("txtField_timeline_BPM"))->getDouble();

			if(newBPM<10) newBPM=10;
			else if(newBPM>999) newBPM=999;

			if(abs(newBPM-oldBPM)<1e-6) return;


			bool needReverse = newBPM>oldBPM;

			if(EDITCONFIG->config_moveNoteAsWell)
			{
				for (int i=needReverse?(EDITOR_PTR->mapData->coreInfoPtr->notes.size()-1):0;needReverse?(i>=0):(i<EDITOR_PTR->mapData->coreInfoPtr->notes.size());needReverse?(i--):(i++))
				{
					divacore::MapNote &thisNote = EDITOR_PTR->mapData->coreInfoPtr->notes[i];
					if(thisNote.notePoint[0].position>thisBPMPos ||
						(thisNote.notePoint.size()>1 && thisNote.notePoint[1].position>thisBPMPos))
						thisBPMOperationSet->addOperation(new DivaEditorOperation_ModifyNote(i,thisBPMPos,nextBPMPos,oldBPM,newBPM));
				}

				for(int i=needReverse?(EDITOR_PTR->mapData->coreInfoPtr->events.size()-1):0;needReverse?(i>=0):(i<EDITOR_PTR->mapData->coreInfoPtr->events.size());needReverse?(i--):(i++))
				{
					divacore::MapEvent &thisEvent = EDITOR_PTR->mapData->coreInfoPtr->events[i];
					if(thisEvent.position > thisBPMPos)
						thisBPMOperationSet->addOperation(new DivaEditorOperation_ModifyEvent(i,thisBPMPos,nextBPMPos,oldBPM,newBPM));
				}
			}

			thisBPMOperationSet->addOperation(new DivaEditorOperation_BPM(0,
				newBPM,
				thisBPMPos,
				DivaEditorOperation_BPM::INSERTBPM));

			EDITCONFIG->addAndDoOperation(thisBPMOperationSet);
			
			actionWidget->setFocusable(false);
			actionWidget->setFocusable(true);
		}
		else if(getID() == "btn_TimeLine_deleteBPM")
		{
			DivaEditorOperationSet *thisBPMOperationSet = new DivaEditorOperationSet();
			thisBPMOperationSet->needToRecalcTime=true;
			thisBPMOperationSet->needToPause=true;

			int prevBPMIndex = EDITOR_PTR->mapData->getPrevBPMIndex(nowTimePos);
			if(prevBPMIndex==-1) return;


			int thisBPMPos = EDITOR_PTR->mapData->getBPMPos(nowTimePos),
				nextBPMPos = EDITOR_PTR->mapData->getNextBPMPos(nowTimePos);
			double oldBPM = EDITOR_PTR->mapData->getBPM(nowTimePos),
				newBPM = divacore::Argument::asDouble("value", EDITOR_PTR->mapData->coreInfoPtr->events[prevBPMIndex].arg);

			if(newBPM<10) newBPM=10;
			else if(newBPM>999) newBPM=999;

			bool needReverse = newBPM>oldBPM;

			if(EDITCONFIG->config_moveNoteAsWell)
			{
				for (int i=needReverse?(EDITOR_PTR->mapData->coreInfoPtr->notes.size()-1):0;needReverse?(i>=0):(i<EDITOR_PTR->mapData->coreInfoPtr->notes.size());needReverse?(i--):(i++))
				{
					divacore::MapNote &thisNote = EDITOR_PTR->mapData->coreInfoPtr->notes[i];
					if(thisNote.notePoint[0].position>thisBPMPos ||
						(thisNote.notePoint.size()>1 && thisNote.notePoint[1].position>thisBPMPos))
						thisBPMOperationSet->addOperation(new DivaEditorOperation_ModifyNote(i,thisBPMPos,nextBPMPos,oldBPM,newBPM));
				}

				for(int i=needReverse?(EDITOR_PTR->mapData->coreInfoPtr->events.size()-1):0;needReverse?(i>=0):(i<EDITOR_PTR->mapData->coreInfoPtr->events.size());needReverse?(i--):(i++))
				{
					divacore::MapEvent &thisEvent = EDITOR_PTR->mapData->coreInfoPtr->events[i];
					if(thisEvent.position > thisBPMPos)
						thisBPMOperationSet->addOperation(new DivaEditorOperation_ModifyEvent(i,thisBPMPos,nextBPMPos,oldBPM,newBPM));
				}
			}

			thisBPMOperationSet->addOperation(new DivaEditorOperation_BPM(EDITOR_PTR->mapData->getBPM(nowTimePos),
				0,
				EDITOR_PTR->mapData->getBPMPos(nowTimePos),
				DivaEditorOperation_BPM::DELETEBPM));

			EDITCONFIG->addAndDoOperation(thisBPMOperationSet);


			actionWidget->setFocusable(false);
			actionWidget->setFocusable(true);
		}

		//Stop
		else if(getID() == "btn_TimeLine_changeStop" || (getID() == "txtField_timeline_Stop" && container_Categories[State::TIMELINE]->findWidgetById("btn_TimeLine_changeStop")->isVisible()))
		{
			int nowStopIndex = EDITOR_PTR->mapData->getStopPos(nowTimePos);
			if(nowStopIndex!=-1)
			{
				int length = ((WTextField*)container_Categories[State::TIMELINE]->findWidgetById("txtField_timeline_Stop"))->getFloat();

				EDITCONFIG->addAndDoOperation(new DivaEditorOperation_STOP(EDITOR_PTR->mapData->getStop(nowTimePos),
																		length,
																		nowStopIndex,
																		length==0?DivaEditorOperation_STOP::DELETESTOP:DivaEditorOperation_STOP::CHANGESTOP));
			}
			actionWidget->setFocusable(false);
			actionWidget->setFocusable(true);
		}
		else if(getID() == "btn_TimeLine_insertStop" || (getID() == "txtField_timeline_Stop" && container_Categories[State::TIMELINE]->findWidgetById("btn_TimeLine_insertStop")->isVisible()))
		{
			int nowStopIndex = EDITOR_PTR->mapData->getStopPos(nowTimePos);
			if(nowStopIndex==-1)
			{
				int length = ((WTextField*)container_Categories[State::TIMELINE]->findWidgetById("txtField_timeline_Stop"))->getFloat();
				if(length>0)
				{
					EDITCONFIG->addAndDoOperation(new DivaEditorOperation_STOP(0,
																		length,
																		nowTimePos,
																		DivaEditorOperation_STOP::INSERTSTOP));
				}
			}

			actionWidget->setFocusable(false);
			actionWidget->setFocusable(true);
		}
		else if(getID() == "btn_TimeLine_deleteStop")
		{
			int nowStopIndex = EDITOR_PTR->mapData->getStopPos(nowTimePos);
			if(nowStopIndex!=-1)
			{
				EDITCONFIG->addAndDoOperation(new DivaEditorOperation_STOP(EDITOR_PTR->mapData->getStop(nowTimePos),
																		0,
																		nowStopIndex,
																		DivaEditorOperation_STOP::DELETESTOP));
			}
			actionWidget->setFocusable(false);
			actionWidget->setFocusable(true);
		}

		//Offset
		else if(getID() == "txtField_timeline_offSet" || getID() == "btn_TimeLine_changeOffset")
		{
			EDITCONFIG->addAndDoOperation(new DivaEditorOperation_GridOffset(EDITOR_PTR->mapData->getOffset(),
																		((WTextField*)container_Categories[State::TIMELINE]->findWidgetById("txtField_timeline_offSet"))->getFloat(),
																		EDITCONFIG->config_moveNoteAsWell
																		));

			actionWidget->setFocusable(false);
			actionWidget->setFocusable(true);
		}

		else if(getID() == "slider_timeline_offset")
		{
			gcn::DivaEditorSlider* slider_timeline_offset = (gcn::DivaEditorSlider*)actionWidget;
			EDITCONFIG->addAndDoOperation(new DivaEditorOperation_GridOffset(EDITOR_PTR->mapData->getOffset(),
				slider_timeline_offset->getValue(),
				EDITCONFIG->config_moveNoteAsWell
				));

			actionWidget->setFocusable(false);
			actionWidget->setFocusable(true);
		}

		//BeatNum
		else if(getID() == "txtField_timeline_BeatNum" || getID() == "btn_TimeLine_changeBeatNum")
		{
			int thisPeriod = EDITOR_PTR->mapData->getPeriodfromGrid(nowTimePos);
			bool beatExist = EDITOR_PTR->mapData->beatNumExist(thisPeriod);

			int beatNum = ((WTextField*)container_Categories[State::TIMELINE]->findWidgetById("txtField_timeline_BeatNum"))->getFloat();

			if(beatNum>0 && (!beatExist || (beatExist && beatNum != EDITOR_PTR->mapData->getBeatNum(nowTimePos))))
			{
				EDITCONFIG->addAndDoOperation(new DivaEditorOperation_BeatNum(EDITOR_PTR->mapData->getBeatNum(nowTimePos),
																		beatNum,
																		thisPeriod,
																		beatExist?DivaEditorOperation_BeatNum::CHANGEBEATNUM:DivaEditorOperation_BeatNum::INSERTBEATNUM));
			}

			actionWidget->setFocusable(false);
			actionWidget->setFocusable(true);
		}
		else if(getID() == "btn_TimeLine_deleteBeatNum")
		{
			int thisPeriod = EDITOR_PTR->mapData->getPeriodfromGrid(nowTimePos);
			if(EDITOR_PTR->mapData->getBeatNumPeriod(thisPeriod)!=0)
			{
				EDITCONFIG->addAndDoOperation(new DivaEditorOperation_BeatNum(EDITOR_PTR->mapData->getBeatNum(nowTimePos),
																		0,
																		thisPeriod,
																		DivaEditorOperation_BeatNum::DELETEBEATNUM));
			}

			actionWidget->setFocusable(false);
			actionWidget->setFocusable(true);
		}

		//Tail Speed
		else if(getID() == "txtField_timeline_TailSpeed" || getID() == "btn_TimeLine_changeTailSpeed")
		{
			EDITCONFIG->addAndDoOperation(new DivaEditorOperation_TailSpeed(EDITOR_PTR->mapData->getTailSpeed(),
																		((WTextField*)container_Categories[State::TIMELINE]->findWidgetById("txtField_timeline_TailSpeed"))->getFloat()));


			actionWidget->setFocusable(false);
			actionWidget->setFocusable(true);
		}

#pragma endregion Timeline Category Actions

#pragma region Note Category Actions

#pragma region toolbox

		else if(getID()=="btn_note_flipHorizontal")
		{
			if(EDITCONFIG->noteSelected.size()>0)
			{
				DivaEditorOperationSet *thisModifySet = new DivaEditorOperationSet();
				for(int i=0;i<EDITCONFIG->noteSelected.size();i++)
					thisModifySet->addOperation(new DivaEditorOperation_ModifyNote(EDITCONFIG->noteSelected[i],DivaEditorOperation_ModifyNote::FLIPHORIZONTAL));
				EDITCONFIG->addAndDoOperation(thisModifySet);
			}
		}
		else if(getID()=="btn_note_flipVertical")
		{
			if(EDITCONFIG->noteSelected.size()>0)
			{
				DivaEditorOperationSet *thisModifySet = new DivaEditorOperationSet();
				for(int i=0;i<EDITCONFIG->noteSelected.size();i++)
					thisModifySet->addOperation(new DivaEditorOperation_ModifyNote(EDITCONFIG->noteSelected[i],DivaEditorOperation_ModifyNote::FLIPVERTICAL));
				EDITCONFIG->addAndDoOperation(thisModifySet);
			}
		}
		else if(getID()=="btn_note_allToLetter")
		{
			if(EDITCONFIG->noteSelected.size()>0)
			{
				DivaEditorOperationSet *thisModifySet = new DivaEditorOperationSet();
				for(int i=0;i<EDITCONFIG->noteSelected.size();i++)
					thisModifySet->addOperation(new DivaEditorOperation_ModifyNote(EDITCONFIG->noteSelected[i],DivaEditorOperation_ModifyNote::TOLETTER));
				EDITCONFIG->addAndDoOperation(thisModifySet);
			}
		}
		else if(getID()=="btn_note_allToArrow")
		{
			if(EDITCONFIG->noteSelected.size()>0)
			{
				DivaEditorOperationSet *thisModifySet = new DivaEditorOperationSet();
				for(int i=0;i<EDITCONFIG->noteSelected.size();i++)
					thisModifySet->addOperation(new DivaEditorOperation_ModifyNote(EDITCONFIG->noteSelected[i],DivaEditorOperation_ModifyNote::TOARROW));
				EDITCONFIG->addAndDoOperation(thisModifySet);
			}
		}
		else if(getID()=="btn_note_noteTypeSymmetry")
		{
			if(EDITCONFIG->noteSelected.size()>0)
			{
				DivaEditorOperationSet *thisModifySet = new DivaEditorOperationSet();
				for(int i=0;i<EDITCONFIG->noteSelected.size();i++)
					thisModifySet->addOperation(new DivaEditorOperation_ModifyNote(EDITCONFIG->noteSelected[i],DivaEditorOperation_ModifyNote::TYPESYMMETRY));
				EDITCONFIG->addAndDoOperation(thisModifySet);
			}
		}
		else if(getID()=="btn_note_simplifyTo2Key")
		{
			if(EDITCONFIG->noteSelected.size()>0)
			{
				DivaEditorOperationSet *thisModifySet = new DivaEditorOperationSet();
				for(int i=0;i<EDITCONFIG->noteSelected.size();i++)
					thisModifySet->addOperation(new DivaEditorOperation_ModifyNote(EDITCONFIG->noteSelected[i],DivaEditorOperation_ModifyNote::SIMPLE2KEY));
				EDITCONFIG->addAndDoOperation(thisModifySet);
			}
		}
		else if(getID()=="btn_note_simplifyTo1Key")
		{
			if(EDITCONFIG->noteSelected.size()>0)
			{
				DivaEditorOperationSet *thisModifySet = new DivaEditorOperationSet();
				for(int i=0;i<EDITCONFIG->noteSelected.size();i++)
					thisModifySet->addOperation(new DivaEditorOperation_ModifyNote(EDITCONFIG->noteSelected[i],DivaEditorOperation_ModifyNote::SIMPLE1KEY));
				EDITCONFIG->addAndDoOperation(thisModifySet);
			}
		}
		else if(getID()=="btn_note_splitLongNoteToSingle")
		{
			DivaEditorOperationSet *thisSplitSet = new DivaEditorOperationSet();
			for(int i=0;i<EDITCONFIG->noteSelected.size();i++)
			{
				if(EDITOR_PTR->mapData->coreInfoPtr->notes[EDITCONFIG->noteSelected[i]].noteType == "long")
					thisSplitSet->addOperation(new DivaEditorOperation_SplitLongNote(EDITCONFIG->noteSelected[i]));
			}
			if(thisSplitSet->operations.size()>0)
				EDITCONFIG->addAndDoOperation(thisSplitSet);
			else
				delete thisSplitSet;
		}
#pragma endregion toolbox

#pragma region key sound

		else if(getID()=="btn_note_expandKeySoundBox")
		{
			NoteArea *noteArea = (NoteArea*)container_Categories[State::NOTE]->findWidgetById("NoteArea");
			gcn::BoarderedContainer *keySoundBox = (gcn::BoarderedContainer*)container_Categories[State::NOTE]->findWidgetById("keySoundBox");
			gcn::Container *keySoundControlSet = (gcn::Container*)container_Categories[State::NOTE]->findWidgetById("keySoundControlSet");
			gcn::WButton *btn_note_expandKeySoundBox = (gcn::WButton*)container_Categories[State::NOTE]->findWidgetById("btn_note_expandKeySoundBox");
			gcn::Container *toolBoxButtonSet = (gcn::Container*)container_Categories[State::NOTE]->findWidgetById("toolBoxButtonSet");

			if(btn_note_expandKeySoundBox->getCaption() == LOCALIZATION->getLocalStr(L"MainScene_btn_note_expandKeySoundBox"))
			{
				//Expand
				int yDelta = keySoundBox->getY() - noteArea->getY();

				keySoundBox->setSize(keySoundControlSet->getWidth() + keySoundControlSet->getX()+5,keySoundControlSet->getHeight());
				keySoundBox->setPosition(container_Categories[State::NOTE]->getWidth() - keySoundBox->getWidth() - 5, noteArea->getY());

				keySoundControlSet->setVisible(true);

				btn_note_expandKeySoundBox->setPosition(keySoundBox->getWidth()-btn_note_expandKeySoundBox->getWidth(),yDelta);
				btn_note_expandKeySoundBox->setCaption(LOCALIZATION->getLocalStr(L"MainScene_btn_expandToolBoxHide"));

				container_Categories[State::NOTE]->moveToTop(keySoundBox);
			}
			else if(btn_note_expandKeySoundBox->getCaption() == LOCALIZATION->getLocalStr(L"MainScene_btn_expandToolBoxHide"))
			{
				keySoundBox->setSize(btn_note_expandKeySoundBox->getWidth(),btn_note_expandKeySoundBox->getHeight());
				keySoundBox->setPosition(container_Categories[State::NOTE]->getWidth() - keySoundBox->getWidth() - 5, toolBoxButtonSet->getY()+toolBoxButtonSet->getHeight());

				keySoundControlSet->setVisible(false);

				btn_note_expandKeySoundBox->setPosition(0,0);
				btn_note_expandKeySoundBox->setCaption(LOCALIZATION->getLocalStr(L"MainScene_btn_note_expandKeySoundBox"));
			}
		}
		else if(getID()=="btn_note_modifyHitSound"||getID()=="btn_note_modifyMissSound")
		{
			//Should get back current working directory
			wchar_t cwd[_MAX_PATH];
			_wgetcwd(cwd,_MAX_PATH);

			wstring selectFile = sora::SoraCore::Instance()->fileOpenDialogW((audioDescription + L'(' + audioExtensions+L')' + L'\0' + audioExtensions + L'\0').c_str());

			_wchdir(cwd);

			if(selectFile!=L"")
			{
				if(getID()=="btn_note_modifyHitSound")
				{
					EDITOR_PTR->mapData->modifyGlobalHitMissSound(selectFile,"hit");
					gcn::WLabel *wlabel_globalHitSound = (gcn::WLabel*)container_Categories[State::NOTE]->findWidgetById("wlabel_globalHitSound");
					wlabel_globalHitSound->setCaption(LOCALIZATION->getLocalStr(L"MainScene_wlabel_globalHitSound") + L": " + EDITOR_PTR->mapData->coreInfoPtr->resources["hit"].filePath);
					wlabel_globalHitSound->adjustSize();
				}
				else
				{
					EDITOR_PTR->mapData->modifyGlobalHitMissSound(selectFile,"miss");
					gcn::WLabel *wlabel_globalMissSound = (gcn::WLabel*)container_Categories[State::NOTE]->findWidgetById("wlabel_globalMissSound");
					wlabel_globalMissSound->setCaption(LOCALIZATION->getLocalStr(L"MainScene_wlabel_globalMissSound") + L": " + EDITOR_PTR->mapData->coreInfoPtr->resources["miss"].filePath);
					wlabel_globalMissSound->adjustSize();
				}
			}
		}
		else if(getID()=="btn_note_deleteHitSound")
		{
			EDITOR_PTR->mapData->modifyGlobalHitMissSound(L"","hit");
			gcn::WLabel *wlabel_globalHitSound = (gcn::WLabel*)container_Categories[State::NOTE]->findWidgetById("wlabel_globalHitSound");
			wlabel_globalHitSound->setCaption(LOCALIZATION->getLocalStr(L"MainScene_wlabel_globalHitSound") + L": " + LOCALIZATION->getLocalStr(L"GlobalSoundDisabled"));
			wlabel_globalHitSound->adjustSize();
		}
		else if(getID()=="btn_note_deleteMissSound")
		{
			EDITOR_PTR->mapData->modifyGlobalHitMissSound(L"","miss");
			gcn::WLabel *wlabel_globalMissSound = (gcn::WLabel*)container_Categories[State::NOTE]->findWidgetById("wlabel_globalMissSound");
			wlabel_globalMissSound->setCaption(LOCALIZATION->getLocalStr(L"MainScene_wlabel_globalMissSound") + L": " + LOCALIZATION->getLocalStr(L"GlobalSoundDisabled"));
			wlabel_globalMissSound->adjustSize();
		}

#pragma endregion key sound

#pragma endregion Note Category Actions

#pragma region Show Category Actions

		else if(getID() == "btn_resourceLeftPage")
		{
			((ResourcePanel*)container_Categories[State::SHOW]->findWidgetById("resourcePanel"))->prevPage();
		}
		else if(getID() == "btn_resourceRightPage")
		{
			((ResourcePanel*)container_Categories[State::SHOW]->findWidgetById("resourcePanel"))->nextPage();
		}
		else if(getID() == "btn_resourceAdd")
		{
			//Should get back current working directory
			wchar_t cwd[_MAX_PATH];
			_wgetcwd(cwd,_MAX_PATH);

			wstring selectFile = sora::SoraCore::Instance()->fileOpenDialogW(L"All Files(*.*)\0*.*\0", NULL, true);
			_wchdir(cwd);

			if(selectFile!=L"")
			{
				std::vector<std::wstring> files;
				int off = 0, lastoff = 0;
				while((off = selectFile.find(L'\t', off))!=std::wstring::npos)
				{
					std::wstring thisFile = selectFile.substr(lastoff, off-lastoff);
					std::string addedResource = EDITOR_PTR->mapData->resource_add(thisFile);
					if(addedResource!="ERROR")
						((ResourcePanel*)container_Categories[State::SHOW]->findWidgetById("resourcePanel"))->setSelectedIndex(EDITOR_PTR->mapData->findResourceIndexByID(addedResource));
					lastoff = ++off;
				}
			}

			refreshKeySoundList();
		}
		else if(getID() == "btn_resourceRemove")
		{
			ResourcePanel *resourcePanel = (ResourcePanel*)container_Categories[State::SHOW]->findWidgetById("resourcePanel");
			if(resourcePanel->getSelectedIndex()!=-1)
			{
				EDITOR_PTR->mapData->resource_delete( EDITOR_PTR->mapData->findResourceIDByIndex(resourcePanel->getSelectedIndex()));
				((ResourcePanel*)container_Categories[State::SHOW]->findWidgetById("resourcePanel"))->setSelectedIndex(-1);
			}

			refreshKeySoundList();
		}
		else if(getID() == "wtextField_resourceDetail_fileLabelValue")
		{
			ResourcePanel *resourcePanel = (ResourcePanel*)container_Categories[State::SHOW]->findWidgetById("resourcePanel");
			gcn::WTextField *wtextFiled_resourceDetail_fileLabelValue = (gcn::WTextField*)container_Categories[State::SHOW]->findWidgetById("wtextField_resourceDetail_fileLabelValue");
			EDITOR_PTR->mapData->resourceDescription_modify(EDITOR_PTR->mapData->findResourceIDByIndex(resourcePanel->getSelectedIndex()),wtextFiled_resourceDetail_fileLabelValue->getText());
			actionWidget->setFocusable(false);
			actionWidget->setFocusable(true);

			refreshKeySoundList();
		}
		else if(getID() == "btn_resourceDetail_addFileEvent")
		{
			ResourcePanel *resourcePanel = (ResourcePanel*)container_Categories[State::SHOW]->findWidgetById("resourcePanel");
			if(resourcePanel->getSelectedIndex()!=-1)
			{
				EDITOR_PTR->mapData->resourceEvent_add(CORE_PTR->getRunPosition(),EDITOR_PTR->mapData->findResourceIDByIndex(resourcePanel->getSelectedIndex()));
				refreshResourcePanelDetail();
				refreshEventDetail();
				EditUtility.refreshAll();
			}
		}
		else if(getID() == "btn_resourceDetail_removeFileEvent")
		{
			ResourcePanel *resourcePanel = (ResourcePanel*)container_Categories[State::SHOW]->findWidgetById("resourcePanel");
			gcn::WListBox *wlistbox_resourceDetail_fileEventsValue = (WListBox*)container_Categories[State::SHOW]->findWidgetById("wlistbox_resourceDetail_fileEventsValue");

			if(wlistbox_resourceDetail_fileEventsValue->getSelected()>=0)
			{
				EDITOR_PTR->mapData->resourceEvent_delete( EDITOR_PTR->mapData->findResourceEventIndexByIndexInResource(
																					wlistbox_resourceDetail_fileEventsValue->getSelected(),
																					EDITOR_PTR->mapData->findResourceIDByIndex(resourcePanel->getSelectedIndex())) );
				refreshResourcePanelDetail();
				refreshEventDetail();
				EditUtility.refreshAll();
			}
		}
		else if(getID() == "wtextField_resourceDetail_fileEventsPosition")
		{
			gcn::WTextField *wtextField_resourceDetail_fileEventsPosition = (gcn::WTextField*)container_Categories[State::SHOW]->findWidgetById("wtextField_resourceDetail_fileEventsPosition");
			ResourcePanel *resourcePanel = (ResourcePanel*)container_Categories[State::SHOW]->findWidgetById("resourcePanel");
			gcn::WListBox *wlistbox_resourceDetail_fileEventsValue = (gcn::WListBox*)container_Categories[State::SHOW]->findWidgetById("wlistbox_resourceDetail_fileEventsValue");

			if(resourcePanel->getSelectedIndex()!=-1&&wlistbox_resourceDetail_fileEventsValue->getSelected()!=-1)
			{
				std::string selectedResourceID = EDITOR_PTR->mapData->findResourceIDByIndex(resourcePanel->getSelectedIndex());
				EDITOR_PTR->mapData->resourceEvent_modifyPos(EDITOR_PTR->mapData->findResourceEventIndexByIndexInResource(wlistbox_resourceDetail_fileEventsValue->getSelected(),selectedResourceID),
																wtextField_resourceDetail_fileEventsPosition->getFloat());
				refreshResourcePanelDetail();
				EditUtility.reCaltTime();
				EditUtility.refreshAll();
			}

			actionWidget->setFocusable(false);
			actionWidget->setFocusable(true);
		}
#pragma endregion Show Category Actions

#pragma region Layer Display Toggle
		else if(getID()=="wcheckbox_showNote")
		{
			gcn::WCheckBox *wcheckbox_showNote = (gcn::WCheckBox*)top->findWidgetById("wcheckbox_showNote");
			EDITCONFIG->display_note = wcheckbox_showNote->isSelected();
		}
		else if(getID()=="wcheckbox_showGrid")
		{
			gcn::WCheckBox *wcheckbox_showGrid = (gcn::WCheckBox*)top->findWidgetById("wcheckbox_showGrid");
			EDITCONFIG->display_grid = wcheckbox_showGrid->isSelected();
			if(EDITCONFIG->display_grid)
				EDITCONFIG->display_background=true;
		}
		else if(getID()=="wcheckbox_showBackground")
		{
			gcn::WCheckBox *wcheckbox_showBackground = (gcn::WCheckBox*)top->findWidgetById("wcheckbox_showBackground");
			EDITCONFIG->display_background = wcheckbox_showBackground->isSelected();
			if(!EDITCONFIG->display_background)
				EDITCONFIG->display_grid=false;
		}

#pragma endregion Layer Display Toggle

#pragma region Editor Controls
		
		else if(getID() == "btn_Save")
		{
			EDITOR_PTR->mapData->SaveFile();
		}
		else if(getID() == "btn_SaveTo")
		{
			if(EDITOR_PTR->mapData->ChooseWorkingFile(false)==L"OK")
				EDITOR_PTR->mapData->SaveFile();
		}
		else if(getID() == "wcheckbox_autosave")
		{
			gcn::WCheckBox *wcheckbox_autoSave = (gcn::WCheckBox*)top->findWidgetById("wcheckbox_autosave");
			if(!EDITCONFIG->isAutoSave)
				EDITCONFIG->lastSavedTimeDelta = 0;
			EDITCONFIG->isAutoSave = wcheckbox_autoSave->isSelected();
		}
		

#pragma endregion Editor Controls
	}
	


	void DivaEditorMainScene::refreshKeySoundList(bool onlySelection)
	{
		//EDITOR_PTR->mapData->selected
		gcn::WListBox *wlistbox_notekey = (gcn::WListBox*)container_Categories[State::NOTE]->findWidgetById("wlistbox_notekey");
		gcn::WListModel *keyListModel = wlistbox_notekey->getListModel();

		gcn::WLabel *wlabel_selectedNoteKey = (gcn::WLabel*)container_Categories[State::NOTE]->findWidgetById("wlabel_selectedNoteKey");

		if(!onlySelection)
		{
			keyListModel->clearElements();
			keyListModel->pushElement(L"None");
			std::vector<std::wstring> keySounds;
			for(divacore::MapInfo::RESOURCES::iterator i=EDITOR_PTR->mapData->coreInfoPtr->resources.begin();i!=EDITOR_PTR->mapData->coreInfoPtr->resources.end();i++)
			{
				divacore::MapResourceInfo &resourceInfo = i->second;
				if(resourceInfo.type == divacore::MapResourceInfo::AUDIO && resourceInfo.ID!="hit" && resourceInfo.ID!="miss" && resourceInfo.ID!=EDITOR_PTR->mapData->coreInfoPtr->header.mainSound)
				{
					keySounds.push_back(EDITOR_PTR->mapData->getResourceDescription(resourceInfo.ID));
					//keyListModel->pushElement( EDITOR_PTR->mapData->getResourceDescription(resourceInfo.ID));
				}
			}
			std::sort(keySounds.begin(),keySounds.end());
			for(int i=0;i<keySounds.size();i++)
				keyListModel->pushElement(keySounds[i]);

		}

		if(EDITCONFIG->noteSelected.size() == 0)
		{
			wlabel_selectedNoteKey->setCaption(LOCALIZATION->getLocalStr(L"MainScene_wlabel_selectedNoteKey") + L" --");
			wlistbox_notekey->setSelected(0, false);
		}
		else
		{
			std::wstring selectedNotesKey = EDITOR_PTR->mapData->getResourceDescription( EDITOR_PTR->mapData->coreInfoPtr->notes[EDITCONFIG->noteSelected[0]].notePoint[0].key );
			bool allsame = true;
			for(int i=1;i<EDITCONFIG->noteSelected.size();i++)
			{
				std::wstring thisKey = EDITOR_PTR->mapData->getResourceDescription( EDITOR_PTR->mapData->coreInfoPtr->notes[EDITCONFIG->noteSelected[i]].notePoint[0].key );
				if(thisKey!=selectedNotesKey)
				{
					allsame = false;
					break;
				}
			}

			if(!allsame)
			{
				wlabel_selectedNoteKey->setCaption(LOCALIZATION->getLocalStr(L"MainScene_wlabel_selectedNoteKey") + LOCALIZATION->getLocalStr(L"DifferentValue"));
				wlistbox_notekey->setSelected(-1, false);
			}
			else
			{
				if(selectedNotesKey == L"")
					selectedNotesKey = L"None";
				wlabel_selectedNoteKey->setCaption(LOCALIZATION->getLocalStr(L"MainScene_wlabel_selectedNoteKey") + selectedNotesKey);
				wlistbox_notekey->setSelected( keyListModel->getElementIndex(selectedNotesKey), false);
			}
		}
		wlabel_selectedNoteKey->adjustSize();
	}

	void DivaEditorMainScene::refreshResourcePanelDetail()
	{
		ResourcePanel *resourcePanel = (ResourcePanel*)container_Categories[State::SHOW]->findWidgetById("resourcePanel");
		gcn::BoarderedContainer *resourceDetailContainer = (gcn::BoarderedContainer*)container_Categories[State::SHOW]->findWidgetById("resourceDetailContainer");

		gcn::WListBox *wlistbox_resourceDetail_fileEventsValue = (gcn::WListBox*)container_Categories[State::SHOW]->findWidgetById("wlistbox_resourceDetail_fileEventsValue");
		wlistbox_resourceDetail_fileEventsValue->getListModel()->clearElements();

		if(resourcePanel->getSelectedIndex()!=-1)
		{
			resourceDetailContainer->setVisible(true);
			std::string selectedResourceID = EDITOR_PTR->mapData->findResourceIDByIndex(resourcePanel->getSelectedIndex());

			gcn::WLabel *wlabel_resourceDetail_filePath = (gcn::WLabel*)container_Categories[State::SHOW]->findWidgetById("wlabel_resourceDetail_filePath");
			gcn::WLabel *wlabel_resourceDetail_filePathValue = (gcn::WLabel*)container_Categories[State::SHOW]->findWidgetById("wlabel_resourceDetail_filePathValue");
			wlabel_resourceDetail_filePathValue->setCaption(EDITOR_PTR->mapData->coreInfoPtr->resources[selectedResourceID].filePath);
			wlabel_resourceDetail_filePathValue->adjustSize();
			wlabel_resourceDetail_filePathValue->setPosition(wlabel_resourceDetail_filePath->getX()+wlabel_resourceDetail_filePath->getWidth(),wlabel_resourceDetail_filePath->getY());

			gcn::WLabel *wlabel_resourceDetail_fileType = (gcn::WLabel*)container_Categories[State::SHOW]->findWidgetById("wlabel_resourceDetail_fileType");
			gcn::WLabel *wlabel_resourceDetail_fileTypeValue = (gcn::WLabel*)container_Categories[State::SHOW]->findWidgetById("wlabel_resourceDetail_fileTypeValue");
			wlabel_resourceDetail_fileTypeValue->setCaption(LOCALIZATION->getLocalStr(sora::s2ws(EDITOR_PTR->mapData->findResourceTypeStrByID(selectedResourceID))));
			wlabel_resourceDetail_fileTypeValue->adjustSize();
			wlabel_resourceDetail_fileTypeValue->setPosition(wlabel_resourceDetail_fileType->getX()+wlabel_resourceDetail_fileType->getWidth(),wlabel_resourceDetail_fileType->getY());

			gcn::WTextField *wtextField_resourceDetail_fileLabelValue = (gcn::WTextField*)container_Categories[State::SHOW]->findWidgetById("wtextField_resourceDetail_fileLabelValue");
			wtextField_resourceDetail_fileLabelValue->setText(EDITOR_PTR->mapData->getResourceDescriptionByIndex(resourcePanel->getSelectedIndex()));


			for (int i=0;i<EDITOR_PTR->mapData->coreInfoPtr->events.size();i++)
			{
				divacore::MapEvent &thisEvent = EDITOR_PTR->mapData->coreInfoPtr->events[i];
				if(thisEvent.eventType == "playVideo" || thisEvent.eventType == "displayImage" || thisEvent.eventType == "playMusic")
					if(Argument::asString("id",thisEvent.arg)==selectedResourceID)
						wlistbox_resourceDetail_fileEventsValue->getListModel()->pushElement(
								LOCALIZATION->getLocalStr(L"Event_detail",LOCALIZATION->getLocalStr(sora::s2ws(thisEvent.eventType)).c_str(),(int)thisEvent.position));
			}
			wlistbox_resourceDetail_fileEventsValue->adjustSize();

			gcn::WButton *btn_resourceDetail_addFileEvent = (gcn::WButton*)container_Categories[State::SHOW]->findWidgetById("btn_resourceDetail_addFileEvent");
			gcn::WButton *btn_resourceDetail_removeFileEvent = (gcn::WButton*)container_Categories[State::SHOW]->findWidgetById("btn_resourceDetail_removeFileEvent");

			btn_resourceDetail_addFileEvent->setPosition(wlistbox_resourceDetail_fileEventsValue->getX()+5, 
				wlistbox_resourceDetail_fileEventsValue->getY() + wlistbox_resourceDetail_fileEventsValue->getHeight()+5);
			btn_resourceDetail_removeFileEvent->setPosition(btn_resourceDetail_addFileEvent->getX()+btn_resourceDetail_addFileEvent->getWidth()+5, btn_resourceDetail_addFileEvent->getY());

			resourceDetailContainer->setSize(resourceDetailContainer->getWidth(), btn_resourceDetail_addFileEvent->getY()+btn_resourceDetail_addFileEvent->getHeight()+10);
		}
		else
			resourceDetailContainer->setVisible(false);
	}
	void DivaEditorMainScene::refreshEventDetail()
	{
		ResourcePanel *resourcePanel = (ResourcePanel*)container_Categories[State::SHOW]->findWidgetById("resourcePanel");
		gcn::WListBox *wlistbox_resourceDetail_fileEventsValue = (gcn::WListBox*)container_Categories[State::SHOW]->findWidgetById("wlistbox_resourceDetail_fileEventsValue");
		gcn::WTextField *wtextField_resourceDetail_fileEventsPosition = (gcn::WTextField*)container_Categories[State::SHOW]->findWidgetById("wtextField_resourceDetail_fileEventsPosition");
		std::string selectedResourceID = EDITOR_PTR->mapData->findResourceIDByIndex(resourcePanel->getSelectedIndex());

		if(wlistbox_resourceDetail_fileEventsValue->getSelected()>=wlistbox_resourceDetail_fileEventsValue->getListModel()->getNumberOfElements())
		{
			wlistbox_resourceDetail_fileEventsValue->setSelected(-1);
			return;
		}

		if(wlistbox_resourceDetail_fileEventsValue->getSelected()!=-1)
		{
			wtextField_resourceDetail_fileEventsPosition->setVisible(true);

			wtextField_resourceDetail_fileEventsPosition->setText( iToWS(EDITOR_PTR->mapData->coreInfoPtr->events[
				EDITOR_PTR->mapData->findResourceEventIndexByIndexInResource(wlistbox_resourceDetail_fileEventsValue->getSelected(),
					selectedResourceID)].position) );
				wtextField_resourceDetail_fileEventsPosition->setPosition(wlistbox_resourceDetail_fileEventsValue->getX()+wlistbox_resourceDetail_fileEventsValue->getWidth() + 5,
					wlistbox_resourceDetail_fileEventsValue->getY());
		}
		else
			wtextField_resourceDetail_fileEventsPosition->setVisible(false);
	}

	void DivaEditorMainScene::valueChanged(const gcn::SelectionEvent &event)
	{
		if(event.getSource()->getId()=="resourcePanel")
		{
			refreshResourcePanelDetail();
			refreshEventDetail();
		}
		else if(event.getSource()->getId()=="wlistbox_resourceDetail_fileEventsValue")
		{
			refreshEventDetail();
		}
		else if(event.getSource()->getId()=="wlistbox_notekey")
		{
			gcn::WListBox *wlistbox_notekey = (gcn::WListBox*)container_Categories[State::NOTE]->findWidgetById("wlistbox_notekey");
			if(EDITCONFIG->noteSelected.size()>0 && wlistbox_notekey->getSelected()>=0)
			{
				DivaEditorOperationSet *thisModifySet = new DivaEditorOperationSet();
				for(int i=0;i<EDITCONFIG->noteSelected.size();i++)
					thisModifySet->addOperation(new DivaEditorOperation_ModifyNote(EDITCONFIG->noteSelected[i], EDITOR_PTR->mapData->findResourceIDByDescription(wlistbox_notekey->getListModel()->getElementAt(wlistbox_notekey->getSelected()))));
				EDITCONFIG->addAndDoOperation(thisModifySet);
				refreshKeySoundList(true);
			}
		}
		
	}

	void DivaEditorMainScene::onMouseWheelUp(sora::SoraMouseEvent& event)
	{
		int setTo = CORE_PTR->getRunPosition();
		if(EDITCONFIG->isctrl)
		{
			//Find last position which contains a note
			for (int i=EDITOR_PTR->mapData->coreInfoPtr->notes.size()-1;i>=0;i--)
				if(EDITOR_PTR->mapData->coreInfoPtr->notes[i].notePoint[0].position<setTo)
				{
					setTo = EDITOR_PTR->mapData->coreInfoPtr->notes[i].notePoint[0].position;
					break;
				}
		}
		else
		{
			if(EDITCONFIG->isshift)
			{
				int nowPeriod = EDITOR_PTR->mapData->getPeriodfromGrid(setTo)-1;
				setTo = EDITOR_PTR->mapData->getGridFromPeriod(nowPeriod);
			}
			else
				for(int i=event.wheel;i!=0;i--)
					setTo = EDITOR_PTR->mapData->getPrevStandardGrid(setTo,EDITCONFIG->getGridToShowPerBeat());

			if(setTo<0) setTo=0;
		}
		
		EDITOR_PTR->mapData->setPos(setTo);
	}
	void DivaEditorMainScene::onMouseWheelDown(sora::SoraMouseEvent& event)
	{
		int setTo = CORE_PTR->getRunPosition();

		if(EDITCONFIG->isctrl)
		{
			//Find next position which contains a note
			for (int i=0;i<EDITOR_PTR->mapData->coreInfoPtr->notes.size();i++)
				if(EDITOR_PTR->mapData->coreInfoPtr->notes[i].notePoint[0].position>setTo)
				{
					setTo = EDITOR_PTR->mapData->coreInfoPtr->notes[i].notePoint[0].position;
					break;
				}
		}
		else
		{
			if(EDITCONFIG->isshift)
			{
				int nowPeriod = EDITOR_PTR->mapData->getPeriodfromGrid(setTo) + 1;
				setTo = EDITOR_PTR->mapData->getGridFromPeriod(nowPeriod);
			}
			else
				for(int i=abs(event.wheel);i!=0;i--)
					setTo = EDITOR_PTR->mapData->getNextStandardGrid(setTo,EDITCONFIG->getGridToShowPerBeat());

			if(setTo>CORE_FLOW_PTR->getTotalPosition()) setTo = CORE_FLOW_PTR->getTotalPosition();
		}
		
		EDITOR_PTR->mapData->setPos(setTo);
	}

	void DivaEditorMainScene::onKeyPressed(SoraKeyEvent& event)
	{
		if(event.key == sora::key::Ctrl)
			EDITCONFIG->isctrl=true;
		if(event.key == sora::key::Shift)
			EDITCONFIG->isshift=true;

		if(event.key == sora::key::Z && event.isCtrlFlag())
		{
			if(!event.isShiftFlag())
				EDITCONFIG->undoTo();
			else
				EDITCONFIG->redoTo();
		}
		else if(event.key == sora::key::C && event.isCtrlFlag())
			EDITOR_PTR->mapData->copy(false);
		else if(event.key == sora::key::X && event.isCtrlFlag())
			EDITOR_PTR->mapData->copy(true);
		else if(event.key == sora::key::V && event.isCtrlFlag())
			EDITOR_PTR->mapData->paste(CORE_PTR->getRunPosition());
		
		if(nowState==State::NOTE && event.key == sora::key::Left && !event.isShiftFlag() && !event.isAltFlag())
		{
			int setTo = EDITOR_PTR->mapData->getPrevStandardGrid(CORE_PTR->getRunPosition(),EDITCONFIG->getGridToShowPerBeat());
			if(setTo<0) setTo=0;
			EDITOR_PTR->mapData->setPos(setTo);
		}
		else if(nowState==State::NOTE && (event.key == sora::key::Left || event.key == sora::key::Right || event.key == sora::key::Up || event.key == sora::key::Down) && event.isShiftFlag()
					&&EDITCONFIG->noteSelected.size()>0)
		{
			//Move in note area
			gcn::Rectangle dragAreaRect = EDITOR_PTR->mapData->findSelectedAreaRectange();

			int deltaDragX = (event.key == sora::key::Left)?-1:((event.key == sora::key::Right)?1:0),
				deltaDragY = (event.key == sora::key::Up)?-1:((event.key == sora::key::Down)?1:0);

			//Check boarder
			if(dragAreaRect.x+deltaDragX>=0 && dragAreaRect.x+deltaDragX<=EDITCONFIG->NoteAreaWidth &&
				dragAreaRect.width+deltaDragX>=0 && dragAreaRect.width+deltaDragX<=EDITCONFIG->NoteAreaWidth &&
				dragAreaRect.y+deltaDragY>=0 && dragAreaRect.y+deltaDragY<=EDITCONFIG->NoteAreaHeight &&
				dragAreaRect.height+deltaDragY>=0 && dragAreaRect.height+deltaDragY<=EDITCONFIG->NoteAreaHeight)
			{
				//Operation move set of notes pos
				DivaEditorOperationSet *thisModifySet = new DivaEditorOperationSet();

				for(int i=0;i<EDITCONFIG->noteSelected.size();i++)
					thisModifySet->addOperation(new DivaEditorOperation_ModifyNote(EDITCONFIG->noteSelected[i],deltaDragX,deltaDragY,true));
				EDITCONFIG->addAndDoOperation(thisModifySet);

				dragAreaRect.x += deltaDragX;
				dragAreaRect.width += deltaDragX;
				dragAreaRect.y += deltaDragY;
				dragAreaRect.height += deltaDragY;
			}
		}
		else if(nowState==State::NOTE && (event.key == sora::key::Left || event.key == sora::key::Right || event.key == sora::key::Up || event.key == sora::key::Down) && event.isAltFlag()
					&&EDITCONFIG->noteSelected.size()>0)
		{
			//Move in time line
			int posDelta = (event.key == sora::key::Left)?-1:((event.key == sora::key::Right)?1:0),
				typeDelta = (event.key == sora::key::Up)?-1:((event.key == sora::key::Down)?1:0);

			if(posDelta!=0)
			{
				int thisPos = EDITOR_PTR->mapData->coreInfoPtr->notes[EDITCONFIG->noteSelected[0]].notePoint[0].position;
				if(posDelta<0)
					posDelta = EDITOR_PTR->mapData->getPrevStandardGrid(thisPos ,EDITCONFIG->getGridToShowPerBeat())-thisPos;
				else if(posDelta>0)
					posDelta = EDITOR_PTR->mapData->getNextStandardGrid(thisPos ,EDITCONFIG->getGridToShowPerBeat())-thisPos;

				EDITOR_PTR->mapData->modifySelectedNotesPos(posDelta);
			}
			else if(typeDelta!=0)
			{
				EDITOR_PTR->mapData->modifySelectedNotesType(typeDelta);
			}

		}
		else if(nowState==State::NOTE && event.key == sora::key::Right && !event.isShiftFlag() && !event.isAltFlag())
		{
			int setTo = EDITOR_PTR->mapData->getNextStandardGrid(CORE_PTR->getRunPosition(),EDITCONFIG->getGridToShowPerBeat());
			if(setTo>CORE_FLOW_PTR->getTotalPosition()) setTo = CORE_FLOW_PTR->getTotalPosition();
			EDITOR_PTR->mapData->setPos(setTo);
		}
		else if(nowState==State::NOTE && event.key == sora::key::Up && !event.isShiftFlag() && !event.isAltFlag())
		{
			EDITCONFIG->decreaseGridToShowPerBeat();
		}
		else if(nowState==State::NOTE && event.key == sora::key::Down && !event.isShiftFlag() && !event.isAltFlag())
		{
			EDITCONFIG->increaseGridToShowPerBeat();
		}
		

		if(nowState!=State::PREVIEW && event.key == sora::key::Space)
		{
			if(CORE_FLOW_PTR->getState() == CoreFlow::RUN)
				EDITOR_PTR->mapData->PauseAndResume();
			else
			{
				EDITUTILITY.reCaltTime();
				EDITOR_PTR->mapData->PauseAndResume();
			}
			
		}

		if(nowState==State::NOTE)
		{
			NoteArea* noteArea = (NoteArea*)container_Categories[nowState]->findWidgetById("NoteArea");
			noteArea->onKeyPressed(event);

			Timeline* timeline = (Timeline*)top->findWidgetById("timeline_TimeLine");
			timeline->onKeyPressed(event);
		}
	}
	void DivaEditorMainScene::onKeyReleased(SoraKeyEvent& event)
	{
		if(event.key == sora::key::Ctrl)
			EDITCONFIG->isctrl=false;
		else if(event.key == sora::key::Shift)
			EDITCONFIG->isshift=false;
		else if(event.key == sora::key::Tab && !event.isAltFlag())
			EDITCONFIG->ChangeEditState();
		if(event.key == sora::key::A && event.isCtrlFlag())
		{
			for (int i=0;i<EDITOR_PTR->mapData->coreInfoPtr->notes.size();i++)
			{
				if(!EDITCONFIG->isNoteSelected(i))
					EDITCONFIG->addSelectedNote(i);
			}
		}

		if(nowState==State::NOTE)
		{
			NoteArea* noteArea = (NoteArea*)container_Categories[nowState]->findWidgetById("NoteArea");
			noteArea->onKeyReleased(event);

			Timeline* timeline = (Timeline*)top->findWidgetById("timeline_TimeLine");
			timeline->onKeyReleased(event);
		}
	}


}