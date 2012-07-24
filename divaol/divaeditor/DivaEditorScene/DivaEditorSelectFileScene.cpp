#include "divaeditor/DivaEditorScene/DivaEditorScene.h"
#include "Animation/SoraGUIAnimation.h"

namespace divaeditor
{
	using namespace gcn;

	//////////////////////////////////////////////////////////////////////////
	//DivaEditor Category Select Scene
	//Usage:
	//1.Create New Map
	//2.Open a map file
	//3.Open Timeline Edit Scene
	//4.Open Note Edit Scene
	//5.Open Resource Edit Scene
	//6.Release Map
	//////////////////////////////////////////////////////////////////////////
	DivaEditorCategorySelectScene::DivaEditorCategorySelectScene()
	{
		sceneIndex = Editor::State::CATEGORY;

		//Init Scene
		top = new Container();
		top->setBaseColor(gcn::Color(0,0,0,0));
		top->setSize(sora::SoraCore::Instance()->getScreenWidth(),sora::SoraCore::Instance()->getScreenHeight());

		//New Map File Button
		btn_newFile = new Button("New File");
		btn_newFile->setId("btn_CSScene_newFile");
		btn_newFile->setPosition(10,10);
		sora::SoraGUI::Instance()->registerGUIResponser(btn_newFile, this, "btn_CSScene_newFile", sora::RESPONSEACTION);
		top->add(btn_newFile);

		//Open Map File Button
		btn_openFile = new Button("Open File");
		btn_openFile->setId("btn_CSScene_openFile");
		btn_openFile->setPosition(100,10);
		sora::SoraGUI::Instance()->registerGUIResponser(btn_openFile, this, "btn_CSScene_openFile", sora::RESPONSEACTION);
		top->add(btn_openFile);

		btn_gotoTimeLine = new Button("1.Edit Timeline");
		btn_gotoTimeLine->setId("btn_CSSCene_gotoTimeLine");
		btn_gotoTimeLine->setSize(400,60);
		sora::SoraGUI::Instance()->registerGUIResponser(btn_gotoTimeLine, this, "btn_CSSCene_gotoTimeLine", sora::RESPONSEACTION);
		top->add(btn_gotoTimeLine);

		btn_gotoNote = new Button("2.Edit Note");
		btn_gotoNote->setId("btn_CSSCene_gotoNote");
		btn_gotoNote->setSize(400,60);
		sora::SoraGUI::Instance()->registerGUIResponser(btn_gotoNote, this, "btn_CSSCene_gotoNote", sora::RESPONSEACTION);
		top->add(btn_gotoNote);

		btn_gotoResource = new Button("3.Edit Resources");
		btn_gotoResource->setId("btn_CSSCene_gotoResource");
		btn_gotoResource->setSize(400,60);
		sora::SoraGUI::Instance()->registerGUIResponser(btn_gotoResource, this, "btn_CSSCene_gotoResource", sora::RESPONSEACTION);
		top->add(btn_gotoResource);

		btn_Release = new Button("4.Release Map");
		btn_Release->setId("btn_CSSCene_Release");
		btn_Release->setSize(400,60);
		sora::SoraGUI::Instance()->registerGUIResponser(btn_Release, this, "btn_CSSCene_Release", sora::RESPONSEACTION);
		top->add(btn_Release);

		black = new Container();
		black->setSize(1280,720);
		black->setPosition(0,0);
		black->setBaseColor(gcn::Color(0,0,0,255));
		top->add(black);
	}
	
	void DivaEditorCategorySelectScene::willAppear()
	{
		top->setEnabled(true);
		//Set up the scene
		
		black->setAlpha(255);
		black->addModifier(new GUIAnimation_Alpha(0,60,GUIAnimation_Float_Linear, NONE,NULL,Bind(this, &DivaEditorCategorySelectScene::didAppear)));
		black->setVisible(true);

		btn_gotoTimeLine->removeModifier();
		btn_gotoTimeLine->setPosition(340,150);
		btn_gotoTimeLine->setAlpha(0);
		btn_gotoTimeLine->addModifier(new GUIAnimation_Position(gcn::Point(440,150),90,GUIAnimation_Float_LinearLog));
		btn_gotoTimeLine->addModifier(new GUIAnimation_Alpha(255,60,GUIAnimation_Float_Linear));

		btn_gotoNote->removeModifier();
		btn_gotoNote->setPosition(540,250);
		btn_gotoNote->setAlpha(0);
		btn_gotoNote->addModifier(new GUIAnimation_Stun(30, NONE,
									new GUIAnimation_Position(gcn::Point(440,250),90,GUIAnimation_Float_LinearLog)));
		btn_gotoNote->addModifier(new GUIAnimation_Stun(30, NONE,
									new GUIAnimation_Alpha(255,60,GUIAnimation_Float_Linear)));

		btn_gotoResource->removeModifier();
		btn_gotoResource->setPosition(340,350);
		btn_gotoResource->setAlpha(0);
		btn_gotoResource->addModifier(new GUIAnimation_Stun(60, NONE,
										new GUIAnimation_Position(gcn::Point(440,350),90,GUIAnimation_Float_LinearLog, NONE)));
		btn_gotoResource->addModifier(new GUIAnimation_Stun(60, NONE,
										new GUIAnimation_Alpha(255,60,GUIAnimation_Float_Linear)));

		btn_Release->removeModifier();
		btn_Release->setPosition(540,450);
		btn_Release->setAlpha(0);
		btn_Release->addModifier(new GUIAnimation_Stun(90, NONE,
									new GUIAnimation_Position(gcn::Point(440,450),90,GUIAnimation_Float_LinearLog)));
		btn_Release->addModifier(new GUIAnimation_Stun(90, NONE,
									new GUIAnimation_Alpha(255,60,GUIAnimation_Float_Linear)));
		

		GCN_GLOBAL->setTop(top);
	}

	void DivaEditorCategorySelectScene::didAppear(gcn::Widget *widget)
	{
		black->setVisible(false);
		if(Appeared)
			Appeared(this);
	}

	void DivaEditorCategorySelectScene::willDisappear()
	{
		black->removeModifier();
		black->setVisible(true);
		black->addModifier(new GUIAnimation_Alpha(255,60,GUIAnimation_Float_Linear,NONE,NULL,Bind(this, &DivaEditorCategorySelectScene::didDisappear)));
	}

	void DivaEditorCategorySelectScene::didDisappear(gcn::Widget *widget)
	{
		if(Disappeared)
			Disappeared(this);
	}


	void DivaEditorCategorySelectScene::action() 
	{
		if(getID() == "btn_CSScene_newFile") 
		{
			
		}
		else if(getID() == "btn_CSScene_openFile")
		{
			
		}
		else if(getID() == "btn_CSSCene_gotoTimeLine")
		{
			EDITOR_PTR->changeScene(Editor::State::TIMELINE);
		}
		else if(getID() == "btn_CSSCene_gotoNote")
		{
			EDITOR_PTR->changeScene(Editor::State::CATEGORY);
		}
		else if(getID() == "btn_CSSCene_gotoResource")
		{

		}
		else if(getID() == "btn_CSSCene_Release")
		{

		}
	}

	
}
