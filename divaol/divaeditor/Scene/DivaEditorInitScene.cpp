#include "divaeditor/Scene/DivaEditorScene.h"




namespace divaeditor
{

	DivaEditorInitScene::DivaEditorInitScene()
	{
		sceneIndex = Editor::State::PRELOAD;

		//Init Scene
		top = new Container();
		top->setBaseColor(gcn::Color(0,0,0,0));
		top->setSize(1920,1080);


		gcn::Button *btn_selectFile = new gcn::Button("Open File...");
		btn_selectFile->setId("btn_selectFile");
		btn_selectFile->setForegroundColor(gcn::Color(255,255,255,255));
		btn_selectFile->setSize(500,50);
		btn_selectFile->setPosition(390,300);
		sora::SoraGUI::Instance()->registerGUIResponser(btn_selectFile, this, "btn_selectFile", sora::RESPONSEACTION);
		top->add(btn_selectFile);


		gcn::WLabel *wlabel_loading = new gcn::WLabel(L"Please select a file.");
		wlabel_loading->setId("wlabel_loading");
		wlabel_loading->setBaseColor(gcn::Color(0,0,0,0));
		wlabel_loading->setForegroundColor(gcn::Color(255,255,255,255));
		wlabel_loading->setSize(400,200);
		wlabel_loading->setAlignment(gcn::Graphics::Alignment::CENTER);
		wlabel_loading->setPosition(440,btn_selectFile->getY()+btn_selectFile->getHeight());
		top->add(wlabel_loading);
	}

	void DivaEditorInitScene::onUpdate(float dt)
	{
		if(!EDITCONFIG->map_initialized)
		{
			gcn::Button *btn_selectFile = (gcn::Button*)top->findWidgetById("btn_selectFile");
			btn_selectFile->setVisible(true);
		}
	}

	void DivaEditorInitScene::action()
	{
		if(getID()=="btn_selectFile" && !EDITCONFIG->map_initialized)
		{
			gcn::Button *btn_selectFile = (gcn::Button*)top->findWidgetById("btn_selectFile");
			gcn::WLabel *wlabel_loading = (gcn::WLabel*)top->findWidgetById("wlabel_loading");

			btn_selectFile->setVisible(false);
			wlabel_loading->setCaption(L"loading...");

			//Should get back current working directory
			wchar_t cwd[_MAX_PATH];
			_wgetcwd(cwd,_MAX_PATH);

			wstring selectFile = sora::SoraCore::Instance()
				->fileOpenDialogW(L"Diva Online Map Project(*.divaolproject)\0*.divaolproject\0All Files(*.*)\0*.*\0");

			_wchdir(cwd);

			if(selectFile!=L"")
			{
				std::wstring initResult = EDITOR_PTR->mapData->InitFromFile(selectFile);

				if(initResult==L"OK")
				{
					CORE_PTR->onEnter();
					EDITOR_PTR->setSongInitialized();
					EDITCONFIG->map_initialized=true;
				}
				else
				{
					wlabel_loading->setCaption(initResult);
					wlabel_loading->adjustSize();
					wlabel_loading->setSize(wlabel_loading->getWidth(),200);
					wlabel_loading->setPosition((1280-wlabel_loading->getWidth())/2,wlabel_loading->getY());
				}
			}
			else
				wlabel_loading->setCaption(L"Please select a file.");
		}
	}


	void DivaEditorInitScene::willAppear()
	{
		top->setEnabled(true);
		GCN_GLOBAL->setTop(top);

		didAppear(top);
	}
	void DivaEditorInitScene::didAppear(gcn::Widget *widget)
	{
		if(Appeared)
			Appeared(this);
	}
	void DivaEditorInitScene::willDisappear()
	{
		didDisappear(NULL);
	}
	void DivaEditorInitScene::didDisappear(gcn::Widget *widget)
	{
		if(Disappeared)
			Disappeared(this);
	}
}

