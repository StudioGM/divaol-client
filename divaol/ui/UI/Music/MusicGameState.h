#pragma once

#include "ui/Config/DivaUIConfig.h"
#include "app/SoraGameState.h"
#include "app/SoraGameApp.h"
#include "SoraEvent.h"
#include "SoraCanvas.h"
#include "SoraCore.h"
#include "SoraSprite.h"
#include "guichansetup.h"

#include "MusicUI.h"

namespace diva
{
	namespace MusicUI
	{
		class MusicGameState: public sora::SoraGameState, public sora::SoraEventHandler 
		{
		public:
			bool initialized;
			sora::SoraBaseCanvas* uiCanvas;
			MusicUI* musicUI;
			int state;
			std::string nextState;

			MusicGameState()
			{
				initialized = false;
				state = 0;
			}

			void onEnter()
			{
				if (!initialized)
				{
					uiCanvas = new sora::SoraBaseCanvas(config[L"gameWidth"].asInt(), config[L"gameHeight"].asInt());
					musicUI = MusicUI::Instance();
					musicUI->SetFatherState(this);

					initialized = true;
				}
				sora::SoraCore::Instance()->setFPS(60);
				uiCanvas->getCanvasSprite()->addEffect(sora::CreateEffectFade(0.0, 1.0, 0.5));
				musicUI->Enter();
			}

			void beginLeave(const std::string& nextState)
			{
				if (nextState == "music" || state == 1)
					return;
				this->nextState = nextState;
				musicUI->BeginLeave();
				uiCanvas->getCanvasSprite()->addEffect(sora::CreateEffectFade(1.0, 0.0, 0.5));
				state = 1;
			}

			void onLeave()
			{
				musicUI->Leave();
			}

			void onUpdate(float dt) {
				if (state == 1)
				{
					if (!uiCanvas->getCanvasSprite()->hasEffect())
						NextState = nextState, state = 0;
				}
				if (NextState!="" && NextState!= "music")
				{
					//uiCanvas->getCanvasSprite()->addEffect(sora::CreateEffectFade(1.0, 0.0, 0.3));
					getGameApp()->setState(NextState);
					NextState = "";
					return;
				}
				musicUI->Update(dt);
				uiCanvas->update(dt);
			}

			void onRender() {

				uiCanvas->beginRender(0);

				sora::GCN_GLOBAL->gcnDraw();
				musicUI->Render();

				uiCanvas->finishRender();

				// ------------------------------

				sora::SoraGameApp::BeginScene();

				sora::SoraSprite* sprite = uiCanvas->getCanvasSprite();
				sprite->setScale(double(config[L"windowWidth"].asInt()) / config[L"gameWidth"].asInt(), double(config[L"windowHeight"].asInt()) / config[L"gameHeight"].asInt());
				sprite->render(0, 0);

				sora::SoraGameApp::EndScene();
			}
		};

	}

}