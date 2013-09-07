/*
 *  DivaCoreResultState.h
 *
 *  Created by Hyf042 on 3/25/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_CORE_RESULT_STATE
#define DIVA_CORE_RESULT_STATE

#include "divapomelo/diva/Client.h"

#include "Core/DivaCoreState.h"
#include "Component/DivaSimpleUIPainter.h"
#include "Component/DivaCommonEvaluateStrategy.h"
#include "divanetwork/DivaStageServer.h"
#include "Utility/DivaSettings.h"
#include "ui/Config/DivaUIConfig.h"

namespace divacore
{
	using namespace sora;

	static const int BGS_ID = 0;

#if defined(DIVA_GNET_OPEN) && !defined(DIVA_USE_POMELO)
	/*
	CoreLoadState
	A state for loading resource and show loading interface
	*/
	class BackButton : public SimpleUI::Button
	{
	public:
		void press()
		{
			sora::SoraBGMManager::Instance()->stopBGS(BGS_ID);
			
			CORE_PTR->over();
			STAGE_CLIENT.returnToStage("game_over");
		}
	};

	class CoreResultState : public CoreState
	{
		sora::SoraText mText;
		divacore::SimpleUIPainter *uiPainter;
	public:
		void onInitiate()
		{
#ifdef _DEBUG
			sora::SoraFont* mFont = sora::SoraFont::LoadFromFile(SETTINGS.getGlobalFontName().asUnicode(), 20);

			mText.setFont(mFont);
#endif
			mText.setText(L"|#FF0000|Game Over");
		}

		void getEvalInfo(GPacket *packet)
		{
			if(CORE_PTR->getState()!=Core::RESULT)
				return;

			//LOGGER->log((packet->getString()).c_str());
			EvalResult &result = EVALUATE_STRATEGY_PTR->getResult();
			gnet::Item<gnet::List> *list = packet->getItem(2)->as<gnet::ListItem>();
			MultiPlay *multiPlay = dynamic_cast<MultiPlay*>(GAME_MODE_PTR);

			for(int i = 0; i < result.evalData.size(); i++)
			{
				gnet::Item<gnet::Tuple> *tuple = dynamic_cast<gnet::Item<gnet::Tuple>*>(list->getItem(i));
				gnet::Item<gnet::Tuple> *addData = NULL;

				if(list->getItem(0)->getType() == gnet::GNET_TYPE_TUPLE)
					addData = dynamic_cast<gnet::Item<gnet::Tuple>*>(tuple->getItem(0));

				if(addData&&addData->size() == 3) {
					result.evalData[i].maxCombo = addData->getItem(0)->getInt();
					result.evalData[i].maxCTLevel = addData->getItem(1)->getInt();
				}
				else
				{
					result.evalData[i].maxCombo = 0;
					result.evalData[i].maxCTLevel = 0;
				}
				result.evalData[i].status = tuple->getItem(1)->getString();
				result.evalData[i].score = (tuple->getItem(2))->getInt();
				result.evalData[i].isOver = tuple->getItem(4)->getInt() > 0;
				result.evalData[i].hp = tuple->getItem(6)->getValue();
				result.evalData[i].uid = tuple->getItem(5)->getString();
				result.evalData[i].nickname = STAGE_CLIENT.waiterInfo(result.evalData[i].uid).nickname;
				PlayerInfo *info = multiPlay->getSpecificPlayerInfo(result.evalData[i].uid);
				if(info)
					result.evalData[i].index = info->teamIndex;
				else
					result.evalData[i].index = 0;
				gnet::Item<gnet::List> *evals = dynamic_cast<gnet::Item<gnet::List>*>(tuple->getItem(3));
				for(int j = 0; j < EvaluateStrategy::EVAL_NUM; j++)
					result.evalData[i].cntEval[j] = evals->getItem(j)->getInt();
			}

			((CommonEvaluateStrategy*)EVALUATE_STRATEGY_PTR)->updateInfo();
		}

		void onEnter()
		{
			if(CORE_PTR->getMode()=="multiPlay")
				GNET_RECEIVE_PACKET("game#evalL",&CoreResultState::getEvalInfo);

			uiPainter = (divacore::SimpleUIPainter*)UI_PAINTER_PTR;

			UI_PAINTER_PTR->gameLoad(GAME_MODULE_PTR->getResultFile());

			//EVALUATE_STRATEGY_PTR->finalEvaluate(); //必须在gameStart之前，不然会出现某些render进不去
			((CommonEvaluateStrategy*)EVALUATE_STRATEGY_PTR)->addMultiEvalUI();

			BackButton *button = new BackButton();
			uiPainter->construct(button,"back");
			button->init();
			uiPainter->addWidget(button);

			UI_PAINTER_PTR->init();

			UI_PAINTER_PTR->gameStart();

			RENDER_SYSTEM_PTR->fadeIn(sora::Color::White.getHWColor());

			if(uiPainter->hasWidget("z_globalInfo"))
			{
				SimpleUI::Text * globalInfo = dynamic_cast<SimpleUI::Text*>(uiPainter->getWidget("z_globalInfo"));
				if(globalInfo)
					globalInfo->setText(Base::String::format("X %.3f",HOOK_MANAGER_PTR->getHookFinalScale()).asUnicode());
			}

			// play BGM
			sora::SoraBGMManager::Instance()->play(diva::config[L"resultMusicFilename"].asString(), false, false);
			sora::SoraBGMManager::Instance()->play(diva::config[L"resultLoopMusicFilename"].asString(), true, true);
		}

		void onLeave()
		{
			sora::SoraBGMManager::Instance()->stopBGS(BGS_ID);
			sora::SoraBGMManager::Instance()->stop(false);

			if(CORE_PTR->getMode()=="multiPlay")
				GNET_UNRECEIVE_PACKET("game#evalL");
			
			UI_PAINTER_PTR->gameStop();
			core->over();
		}

		void onRender()
		{
#ifdef _DEBUG
			mText.render();
#endif
		}

		void onUpdate(float dt)
		{
			if(((CommonEvaluateStrategy*)EVALUATE_STRATEGY_PTR)->isNumberUp())
			{
				sora::SoraBGMManager::Instance()->playBGS(diva::config[L"resultNumberUpSound"].asString(), BGS_ID, 1, SETTINGS.getSEVolume(), 1.f);
			}
			else
			{
				sora::SoraBGMManager::Instance()->stopBGS(BGS_ID);
			}
		}

		void onKeyReleased(SoraKeyEvent& event)
		{
			//if(CORE_PTR->getState()==Core::OVER&&event.getKey()==SORA_KEY_R)
			//{
			//	core->restart();
			//}
		}
	};
#else
	/*
	CoreLoadState
	A state for loading resource and show loading interface
	*/
	class BackButton : public SimpleUI::Button
	{
	public:
		void press()
		{
			sora::SoraBGMManager::Instance()->stopBGS(BGS_ID);
			
			CORE_PTR->over();
			POMELO_STAGE_PEER->returnToStage("game_over");
		}
	};

	class CoreResultState : public CoreState
	{
		sora::SoraText mText;
		divacore::SimpleUIPainter *uiPainter;
	public:
		void onInitiate()
		{
#ifdef _DEBUG
			sora::SoraFont* mFont = sora::SoraFont::LoadFromFile(SETTINGS.getGlobalFontName().asUnicode(), 20);

			mText.setFont(mFont);
#endif
			mText.setText(L"|#FF0000|Game Over");
		}

		void onEval(Json::Value &msg) {
			if(CORE_PTR->getState()!=Core::RESULT)
				return;

			//LOGGER->log((packet->getString()).c_str());
			EvalResult &result = EVALUATE_STRATEGY_PTR->getResult();
			Json::Value &list = msg["player"];
			MultiPlay *multiPlay = dynamic_cast<MultiPlay*>(GAME_MODE_PTR);

			for(int i = 0; i < result.evalData.size(); i++)
			{
				Json::Value &item = list[i];
				Json::Value &extra = item["extra"];

				if(extra.isObject()) {
					result.evalData[i].maxCombo = extra["maxCombo"].asInt();
					result.evalData[i].maxCTLevel = extra["maxCTLevel"].asInt();
				}
				else
				{
					result.evalData[i].maxCombo = 0;
					result.evalData[i].maxCTLevel = 0;
				}
				result.evalData[i].status = "";
				result.evalData[i].score = item["score"].asInt();
				result.evalData[i].isOver = item["complete"].asBool();
				result.evalData[i].hp = item["hp"].asDouble();
				result.evalData[i].uid = item["info"]["uid"].asString();
				result.evalData[i].nickname = POMELO_STAGE_PEER->waiterInfo(result.evalData[i].uid).nickname;
				PlayerInfo *info = multiPlay->getSpecificPlayerInfo(result.evalData[i].uid);
				if(info)
					result.evalData[i].index = info->teamIndex;
				else
					result.evalData[i].index = 0;
				Json::Value &eval = item["eval"];
				for(int j = 0; j < EvaluateStrategy::EVAL_NUM; j++)
					result.evalData[i].cntEval[j] = eval[j].asInt();
			}

			((CommonEvaluateStrategy*)EVALUATE_STRATEGY_PTR)->updateInfo();

		}

		void onEnter()
		{
			if(CORE_PTR->getMode()=="multiPlay")
				POMELO_GAME_PEER->on(divapomelo::EventCode[divapomelo::ON_GAME_HEARTBEAT], [&](divapomelo::MessageReq &req) {
					onEval(req.msg());
				});

			uiPainter = (divacore::SimpleUIPainter*)UI_PAINTER_PTR;

			UI_PAINTER_PTR->gameLoad(GAME_MODULE_PTR->getResultFile());

			//EVALUATE_STRATEGY_PTR->finalEvaluate(); //必须在gameStart之前，不然会出现某些render进不去
			((CommonEvaluateStrategy*)EVALUATE_STRATEGY_PTR)->addMultiEvalUI();

			BackButton *button = new BackButton();
			uiPainter->construct(button,"back");
			button->init();
			uiPainter->addWidget(button);

			UI_PAINTER_PTR->init();

			UI_PAINTER_PTR->gameStart();

			RENDER_SYSTEM_PTR->fadeIn(sora::Color::White.getHWColor());

			if(uiPainter->hasWidget("z_globalInfo"))
			{
				SimpleUI::Text * globalInfo = dynamic_cast<SimpleUI::Text*>(uiPainter->getWidget("z_globalInfo"));
				if(globalInfo)
					globalInfo->setText(Base::String::format("X %.3f",HOOK_MANAGER_PTR->getHookFinalScale()).asUnicode());
			}

			// play BGM
			sora::SoraBGMManager::Instance()->play(diva::config[L"resultMusicFilename"].asString(), false, false);
			sora::SoraBGMManager::Instance()->play(diva::config[L"resultLoopMusicFilename"].asString(), true, true);
		}

		void onLeave()
		{
			sora::SoraBGMManager::Instance()->stopBGS(BGS_ID);
			sora::SoraBGMManager::Instance()->stop(false);

			if(CORE_PTR->getMode()=="multiPlay")
				POMELO_GAME_PEER->remove(divapomelo::EventCode[divapomelo::ON_GAME_HEARTBEAT]);
			
			UI_PAINTER_PTR->gameStop();
			core->over();
		}

		void onRender()
		{
#ifdef _DEBUG
			mText.render();
#endif
		}

		void onUpdate(float dt)
		{
			if(((CommonEvaluateStrategy*)EVALUATE_STRATEGY_PTR)->isNumberUp())
			{
				sora::SoraBGMManager::Instance()->playBGS(diva::config[L"resultNumberUpSound"].asString(), BGS_ID, 1, SETTINGS.getSEVolume(), 1.f);
			}
			else
			{
				sora::SoraBGMManager::Instance()->stopBGS(BGS_ID);
			}
		}

		void onKeyReleased(SoraKeyEvent& event)
		{
			//if(CORE_PTR->getState()==Core::OVER&&event.getKey()==SORA_KEY_R)
			//{
			//	core->restart();
			//}
		}
	};
#endif
}

#endif