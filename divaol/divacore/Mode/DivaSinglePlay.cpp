/*
 *  DivaSinglePlay.cpp
 *
 *  Created by Hyf042 on 2/3/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#include "DivaSinglePlay.h"
//#include "DivaRelayUI.h"
#include "Component/DivaCommonEvaluateStrategy.h"

namespace divacore
{
	//保证血量在正常范围内
	void SinglePlay::modifyHP(int dtHP, int &nowHP) 
	{
		if(!getAlive())
			return;
		nowHP += dtHP;
		if(nowHP>MAX_HP)
			nowHP = MAX_HP;
		if(nowHP<0)
		{
			//CORE_FLOW_PTR->over();
			nowHP = 0;
			setAlive(false);
		}
	}
	void SinglePlay::modifyScore(int dtScore, int &nowScore)
	{
		if(getAlive())
			nowScore += dtScore;
	}

	void SinglePlay::gameReset() 
	{
		LOGGER->msg("Set","StandardStateManager");

		config.clear();
		
		stateList.clear();
		stateQueue.clear();

		nowScore = combo = 0;
		nowHP = ORIGIN_HP;

		setAlive(true);
	}
	bool SinglePlay::checkNote(NotePtr note) 
	{
		stateList.push_back(KeyState(note));
		return true;
	}
	bool SinglePlay::checkExtra(StateEvent& event) 
	{
		event.type = StateEvent::EXTRA;

		HOOK_MANAGER_PTR->hook(event);
		return true;
	}
	bool SinglePlay::checkPress(StateEvent& event)
	{	
		if(!EVALUATE_STRATEGY_PTR->evaluatePress(event))
			return false;
		event.type = StateEvent::PRESS;
	
		if(HOOK_MANAGER_PTR->hook(event))
			return HOOK_MANAGER_PTR->hookInfo();
		return true;
	}
	bool SinglePlay::checkPoint(StateEvent& event) 
	{
		event.type = StateEvent::POINT;

		if(HOOK_MANAGER_PTR->hook(event))
			return HOOK_MANAGER_PTR->hookInfo();
		return false;
	}
	bool SinglePlay::checkFailure(StateEvent& event) 
	{
		EVALUATE_STRATEGY_PTR->evaluateFaliure(event);
		event.type = StateEvent::FAILURE;
		event.breakCombo = event.breakNote = true;

		if(HOOK_MANAGER_PTR->hook(event))
			return HOOK_MANAGER_PTR->hookInfo();

		return true;
	}
	void SinglePlay::inform(StateEvent& event)
	{
		event.tag = "inform";
		HOOK_MANAGER_PTR->hook(event);
		
		if(!getAlive())
			event.dtHP = event.dtScore = 0, event.breakCombo = true;

		modifyHP(event.dtHP,nowHP);
		modifyScore(event.dtScore,nowScore);

		if(getAlive())
			EVALUATE_STRATEGY_PTR->registerState(event);

		//播放hit音
		if(event.type==StateEvent::PRESS||event.type==StateEvent::FAILURE)
		{
			if(event.rank<=4)
				Core::Ptr->getMusicManager()->playDirect("hit","sound_effect");
			else
				Core::Ptr->getMusicManager()->playDirect("miss","sound_effect");
		}

		//如果是PRESS考虑其combo加成
		if(event.type==StateEvent::PRESS)
		{
			//show effect
			pressEffect(event);

			if(event.breakCombo)
				combo = 0;
			else
			{
				combo++;
				StateEvent ret = EVALUATE_STRATEGY_PTR->evaluateCombo(event,combo);
				modifyHP(ret.dtHP,nowHP);
				modifyScore(ret.dtScore,nowScore);
			}
		}
		else if(event.type==StateEvent::FAILURE)
			combo = 0;

		//加入list和queue
		stateList[event.note->getID()].addKey(event);
		stateQueue.push_back(event);
	}
	void SinglePlay::update(float dt)
	{
	}
	void SinglePlay::pressEffect(StateEvent &event)
	{
		if(event.rank==1||event.rank==2||event.rank==3)
		{
			if(getCombo()<100)
				EFFECT_SYSTEM_PTR->addEffect("PressSafe",event.getScreenPosition().x,event.getScreenPosition().y,event.note->getTailTag());
			else
				EFFECT_SYSTEM_PTR->addEffect("PressSafeBetter",event.getScreenPosition().x,event.getScreenPosition().y,event.note->getTailTag());
			EFFECT_SYSTEM_PTR->addEffect("PressFlame",event.getScreenPosition().x,event.getScreenPosition().y,event.note->getTailTag());
		}
		if(event.rank==1)
		{
			if(getCombo()<100)
				EFFECT_SYSTEM_PTR->addEffect("PressFine",event.getScreenPosition().x,event.getScreenPosition().y,event.note->getTailTag());
			else
				EFFECT_SYSTEM_PTR->addEffect("PressFineBetter",event.getScreenPosition().x,event.getScreenPosition().y,event.note->getTailTag());
		}
		if(event.rank==3)
			EFFECT_SYSTEM_PTR->addEffect("PressCool",event.getScreenPosition().x,event.getScreenPosition().y,event.note->getTailTag());
	}

	void SinglePlay::preStart()
	{
		//add player panel
		if(CORE_PTR->getState()!=Core::RESULT)
			UI_PAINTER_PTR->addWidget("singlePlayer");
	}

	void SinglePlay::preEvaluate()
	{
		// add the bonus score caused by hooks
		setScore(getScore() * HOOK_MANAGER_PTR->getHookFinalScale());

		EVALUATE_STRATEGY_PTR->getResult().myScore = getScore();
		EVALUATE_STRATEGY_PTR->getResult().myID = 0;
		EVALUATE_STRATEGY_PTR->getResult().evalData.push_back(EvalData("",getScore(),EVALUATE_STRATEGY_PTR->getResult().myCntEval,0,NET_INFO.nickname));
	}

	void SinglePlay::afterEvaluate()
	{
		//((CommonEvaluateStrategy*)EVALUATE_STRATEGY_PTR)->addSingleEvalUI();
	}
}