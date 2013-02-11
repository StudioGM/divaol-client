/*
 *  DivaCoreGameState.cpp
 *
 *  Created by Hyf042 on 1/15/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#include "DivaCorePlayState.h"
#include "Component/DivaStandardCoreFlow.h"
#include "Mode/DivaEditMode.h"
#include "Utility/DivaSettings.h"
//#include "SoraSoundManager/SoraSoundEffectManager.h"

namespace divacore
{
	using namespace sora;

	void CorePlayState::onInitiate()
	{
#ifdef _DEBUG
		mFont = sora::SoraFont::LoadFromFile(SETTINGS.getGlobalFontName().asUnicode(), 20);

		mText.setFont(mFont);
#endif
		mText.setText(L"|#FF0000|Playing");

		bShowDetail = false;

		coreFlow = NULL;
	}
	void CorePlayState::onEnter()
	{
		coreFlow = core->getCoreFlow();
		
		if(coreFlow->getState()==CoreFlow::READY)
		{
			coreFlow->start();
			bFinish = false;
			start_time = new SoraTimestamp();
			volume = 1.0,bPressed = false;
		}
		setCoreState(Core::RUN);

		sora::SoraCore::Instance()->showMouse(false);

		//Task task;
		//task.setAsMemberFunc(&CorePlayState::pause,this);
		//coreFlow->registerEndCallback(task);
	}

	void CorePlayState::pause(void* args)
	{
		CORE_PTR->pause();
	}

	void CorePlayState::onLeave()
	{
		sora::SoraCore::Instance()->showMouse(true);

		SAFE_DELETE(start_time);
	}

	void CorePlayState::onRender()
	{	
		if(!bShowDetail)
			return;

#ifdef _DEBUG
		mText.render();

		char buffer[1000];
		float deltaTime = 0;
		try
		{
			deltaTime = MUSIC_MANAGER_PTR->getPosition(CORE_FLOW_PTR->MAIN_SOUND_CHANNEL);
			//LOGGER->log("music : %0.2lf",deltaTime);
			deltaTime = coreFlow->getRealTime()-deltaTime;
			//LOGGER->log("real : %0.2lf",coreFlow->getRealTime());
		}
		catch (...)
		{
			;
		}
		sprintf(buffer,"|#FF0000|Time: %0.6lf\nSong: %0.6lf\nScore: %d\nHP: %d\nCombo: %d\nCT: %d\nLevel: %d\nVolume:%0.2f\nPosition:%.3lf",coreFlow->getRealTime(),deltaTime,Core::Ptr->getScore(),Core::Ptr->getHP(),GAME_MODE_PTR->getCombo(),0,0,volume,CORE_FLOW_PTR->getPosition());
		mText.Render(sora::s2ws(buffer),0,200);
#endif
	}

	void CorePlayState::onUpdate(float dt)
	{
		if(bFinish)
		{
			if(!RENDER_SYSTEM_PTR->isFade())
			{
				setCoreState(Core::RESULT);

				core->flowOver();
				core->setState("result");
			}
		}
		else if(coreFlow->getState()==CoreFlow::END)
		{
			bFinish = true;
			mText.setText(L"|#FF0000|Finish!");

			RENDER_SYSTEM_PTR->fadeOut(Color::White.getHWColor());
		}
		else
		{
			static int count = 0;
			count = (count+1)%40;
			SoraWString text = CORE_FLOW_PTR->getState()==CoreFlow::RUN?L"|#FF0000|Playing":L"|#FF0000|Pause";
			for(int i = 0; i < count/10; i++)
				text += L".";
			mText.setText(text);
		}
	}

	void CorePlayState::load(sora::SoraTask* task)
	{
	}

	void CorePlayState::onDestroy()
	{
	}

	void CorePlayState::onKeyPressed(SoraKeyEvent& event)
	{
#ifdef _DEBUG
		if(event.getKey()==SORA_KEY_TAB)
		{
			bShowDetail = !bShowDetail;
		}
#endif
		if(event.getKey()==SORA_KEY_ESCAPE)
		{
			CORE_PTR->exitGame();
		}
	}
	void CorePlayState::onKeyReleased(SoraKeyEvent& event)
	{
#ifdef _DEBUG
		if(event.getKey()==SORA_KEY_P)
		{
			if(CORE_FLOW_PTR->getState()!=CoreFlow::PAUSE)
				core->pause();//core->setState("pause");
			else
				core->resume();
		}
#endif
	}
}