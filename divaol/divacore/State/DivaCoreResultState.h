/*
 *  DivaCoreResultState.h
 *
 *  Created by Hyf042 on 3/25/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_CORE_RESULT_STATE
#define DIVA_CORE_RESULT_STATE

#include "Core/DivaCoreState.h"
#include "Component/DivaSimpleUIPainter.h"
#include "Component/DivaCommonEvaluateStrategy.h"

namespace divacore
{
	using namespace sora;

	/*
	CoreLoadState
	A state for loading resource and show loading interface
	*/
	class BackButton : public SimpleUI::Button
	{
	public:
		void press()
		{
			CORE_PTR->over();
		}
	};

	class CoreResultState : public CoreState
	{
		sora::SoraText mText;
		divacore::SimpleUIPainter *uiPainter;
	public:
		void onInitiate()
		{
			sora::SoraFont* mFont = sora::SoraFont::LoadFromFile("cour.ttf", 20);

			mText.setFont(mFont);
			mText.setText(L"|#FF0000|Game Over");
		}

		void getEvalInfo(GPacket *packet)
		{
			LOGGER->log((packet->getString()).c_str());
			EvalResult &result = EVALUATE_STRATEGY_PTR->getResult();
			gnet::Item<gnet::List> *list = packet->getItem(2)->as<gnet::ListItem>();

			for(int i = 0; i < result.evalData.size(); i++)
			{
				gnet::Item<gnet::Tuple> *tuple = dynamic_cast<gnet::Item<gnet::Tuple>*>(list->getItem(i));
				result.evalData[i].status = tuple->getItem(1)->getString();
				result.evalData[i].score = (tuple->getItem(2))->getInt();
				result.evalData[i].index = i;
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
		}

		void onLeave()
		{
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
		}

		void onKeyReleased(SoraKeyEvent& event)
		{
			//if(CORE_PTR->getState()==Core::OVER&&event.getKey()==SORA_KEY_R)
			//{
			//	core->restart();
			//}
		}
	};
}

#endif