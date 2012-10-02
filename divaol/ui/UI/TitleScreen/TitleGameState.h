#pragma once

#include "SoraCanvas.h"
#include "guichan.hpp"
#include "guichansetup.h"

#include "SoraCore.h"
#include "TitleScreenUI.h"

#include "SoraCore.h"

#include "SoraWindowInfo.h"
#include "SoraFont.h"

#include "SoraDirectoryIterator.h"
#include "SoraFileUtility.h"
#include "SoraFont.h"
#include "SoraText.h"

#include "app/SoraGameApp.h"
#include "app/SoraGameState.h"

#include "SoraResourceFile.h"

#include "SoraFMODSoundSystem.h"
#include "ui/Config/DivaUIConfig.h"
#include "ui/UI/TitleScreen/TitleGameState.h"

namespace diva
{

	class GameInitState: public sora::SoraGameState, public sora::SoraEventHandler {
	public:
		GameInitState() {
			//this->app = app;
			isInitialized = false;
			state = 0;
			//nextState = "";
		}

		~GameInitState()
		{
			if (uiCanvas)
				delete uiCanvas;
		}

		void onRender() {


			uiCanvas->beginRender(0xFFFFFFFF);

			//room->Render();
			titleScreen->Render();
			sora::GCN_GLOBAL->gcnDraw();

			uiCanvas->finishRender();



			sora::SoraGameApp::BeginScene();

			sora::SoraSprite* sprite = uiCanvas->getCanvasSprite();
			sprite->setScale(double(config[L"windowWidth"].asInt()) / config[L"gameWidth"].asInt(), double(config[L"windowHeight"].asInt()) / config[L"gameHeight"].asInt());
			sprite->render(0, 0);

			sora::SoraGameApp::EndScene();

		}

		void beginLeave(const std::string& nextState)
		{
			if (nextState == "init")
				return;
			this->nextState = nextState;
			uiCanvas->getCanvasSprite()->addEffect(sora::CreateEffectFade(1.0, 0.0, 0.5));
			state = 1;
		}

		void onLeave()
		{
			titleScreen->Leave();
		}

		void onUpdate(float dt) {   
			//DivaCEGUI::Instance()->Update(dt);
			//room->Update(dt);
			if (state == 1)
			{
				if (!uiCanvas->getCanvasSprite()->hasEffect())
					NextState = nextState, state = 0;
			}
			if (NextState!="")
			{
				getGameApp()->setState(NextState);
				NextState = "";
				return;
			}
			titleScreen->Update(dt);
			uiCanvas->update(dt);
		}


		void onKeyPressed(sora::SoraKeyEvent& keyEvent) {
			//getGameApp()->setState("house");
			beginLeave("house");
		}

		void onMouseDragged(sora::SoraMouseEvent& from, sora::SoraMouseEvent& to) {
			//y += to.x - from.x;
			//getGameApp()->setState("house");
		}

		void onEnter() {

			if (!isInitialized)
			{
				//diva::MapMgr::Instance()->Load("map_manifest.json");

				//room = UIScreenRoom::Instance();
				sora::GCN_GLOBAL->initGUIChan(L"msyh.ttf", 20);
				titleScreen = TitleScreenUI::Instance();

				uiCanvas = new sora::SoraBaseCanvas(config[L"gameWidth"].asInt(), config[L"gameHeight"].asInt());

				isInitialized = true;
			}

			sora::SoraCore::Ptr->setFPS(60);
			uiCanvas->getCanvasSprite()->addEffect(sora::CreateEffectFade(0.0, 1.0, 0.5));
			titleScreen->Enter();

		}

	private:

		sora::SoraBaseCanvas* uiCanvas;
		//std::string nextState;
		TitleScreenUI* titleScreen;
		bool isInitialized;
		int state;
		std::string nextState;
		//sora::SoraGameApp* app;
	};

}