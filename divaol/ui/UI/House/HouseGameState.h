#pragma once

#include "ui/Config/DivaUIConfig.h"
#include "app/SoraGameState.h"
#include "app/SoraGameApp.h"
#include "SoraEvent.h"
#include "SoraCanvas.h"
#include "SoraCore.h"
#include "SoraSprite.h"
#include "guichansetup.h"

#include "HouseUI.h"

namespace diva
{
	namespace HouseUI
	{
		class HouseGameState: public sora::SoraGameState, public sora::SoraEventHandler 
		{
		public:
			bool initialized;
			sora::SoraBaseCanvas* uiCanvas;
			HouseUI* houseUI;
			int state;
			std::string nextState;

			HouseGameState()
			{
				initialized = false;
				state = 0;
			}

			void onEnter()
			{
				if (!initialized)
				{
					uiCanvas = new sora::SoraBaseCanvas(setConfig[L"gameWidth"].asInt(), setConfig[L"gameHeight"].asInt());
					houseUI = HouseUI::Instance();
					houseUI->SetFatherState(this);
					//HouseUI

					initialized = true;
				}
				sora::SoraCore::Instance()->setFPS(60);
				uiCanvas->getCanvasSprite()->addEffect(sora::CreateEffectFade(0.0, 1.0, 0.3));
				houseUI->Enter();
			}

			void onUpdate(float dt) {
				if (state == 1)
				{
					if (!uiCanvas->getCanvasSprite()->hasEffect())
						NextState = nextState, state = 0;
				}
				if (NextState!="" && NextState!="house")
				{
					getGameApp()->setState(NextState);
					NextState = "";
					return;
				}
				houseUI->Update(dt);
				uiCanvas->update(dt);
			}

			void beginLeave(const std::string& nextState)
			{
				if (nextState == "house" || state == 1)
					return;
				this->nextState = nextState;
				houseUI->BeginLeave();
				uiCanvas->getCanvasSprite()->addEffect(sora::CreateEffectFade(1.0, 0.0, 0.5));
				state = 1;
			}

			void onRender() {

				uiCanvas->beginRender(0);

				sora::GCN_GLOBAL->gcnDraw();
				houseUI->Render();

				uiCanvas->finishRender();

				// ------------------------------

				sora::SoraGameApp::BeginScene();

				sora::SoraSprite* sprite = uiCanvas->getCanvasSprite();
				sprite->setScale(double(setConfig[L"windowWidth"].asInt()) / setConfig[L"gameWidth"].asInt(), double(setConfig[L"windowHeight"].asInt()) / setConfig[L"gameHeight"].asInt());
				sprite->render(0, 0);

				sora::SoraGameApp::EndScene();
			}

			void onLeave()
			{
				houseUI->Leave();
			}

		};

	}

}