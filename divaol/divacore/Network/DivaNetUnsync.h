/*
 *  DivaNetSync.h
 *
 *  Created by Hyf042 on 4/1/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_NET_UNSYNC
#define DIVA_NET_UNSYNC

#include "Core/DivaCoreState.h"
#include "Core/DivaMapLoader.h"
#include "Core/DivaTask.h"
#include "mode/DivaMultiplay.h"
//#include "SoraFMODSoundSystem/SoraFMODSoundSystem.h"
//#include "SoraAudiereSoundSystem/SoraAudiereSoundSystem.h"

namespace divacore
{
	using namespace sora;

	/*
	CoreLoadState
	A state for loading resource and show loading interface
	*/
	class NetUnsync : public CoreState
	{
		bool bFinished;
		volatile bool bReadyToStart;

		sora::SoraFont* mFont;
		sora::SoraText mText;
		Task task;

		enum{DELAYSTATE_DELAY,DELAYSTATE_OVER,DELAYSTATE_START};
		int delayState;

	public:
		void onInitiate()
		{
			bFinished = false;
			bReadyToStart = false;

			mFont = sora::SoraFont::LoadFromFile("cour.ttf", 20);

			mText.setFont(mFont);
			mText.setText(L"|#FF0000|Loading");

			NETWORK_SYSTEM_PTR->receive(network::DIVA_NET_STC_START,new network::EventFunc(&NetUnsync::readyToStart,this));
		}
		void onEnter()
		{
			setCoreState(Core::PREPARE);

			bFinished = false;

			delayState = DELAYSTATE_DELAY;
		}
		void onLeave()
		{
			task.stop();
		}

		void onRender()
		{
#ifdef _DEBUG
			mText.render();
#endif
		}

		void readyToStart(Packet&)
		{
			bReadyToStart = true;
		}

		void onUpdate(float dt)
		{
			if(!bFinished)
			{
				if(delayState==DELAYSTATE_DELAY)
					delayState = DELAYSTATE_OVER;
				else if(delayState==DELAYSTATE_OVER)
				{
					delayState = DELAYSTATE_START;

					task.setMultiThread(false);
					task.setAsMemberFunc(&NetUnsync::load, this);
					task.start();
				}
				else if(!task.isRunning())
				{
					if(((MultiPlay*)GAME_MODE_PTR)->getBaseState()==MultiPlay::READY)
					{
						mText.setText(L"|#FF0000|Finished!");
						sora::log_notice("loading succeed!");

						//setCoreState(Core::RESULT);
						RENDER_SYSTEM_PTR->fadeOut(sora::Color::White.getHWColor());
						//core->setState("play");
					}
					else
					{
						mText.setText(L"|#FF0000|Load Failed!");
						sora::log_notice("loading failed!");
					}
					bFinished = true;
				}
				else
				{
					static int count = 0;
					count = (count+1)%40;
					SoraWString text = L"|#FF0000|";
					if(((MultiPlay*)GAME_MODE_PTR)->getBaseState()==MultiPlay::CONNECT)
						text += L"Connect";
					else if(((MultiPlay*)GAME_MODE_PTR)->getBaseState()==MultiPlay::FAILURE)
						text += L"Failed, wait for reconnecting";
					else if(((MultiPlay*)GAME_MODE_PTR)->getBaseState()==MultiPlay::GET_INFO)
						text += L"Server connected, now wait";
					else if(((MultiPlay*)GAME_MODE_PTR)->getBaseState()==MultiPlay::FULL)
						text += L"Server is full, please login later";
					else
						text += L"Loading";
					for(int i = 0; i < count/10; i++)
						text += L".";
					mText.setText(text);
				}
				
				NETWORK_SYSTEM_PTR->update(dt);

			}
			else if(((MultiPlay*)GAME_MODE_PTR)->getBaseState()==MultiPlay::READY)
			{
				if(!RENDER_SYSTEM_PTR->isFade())
					core->setState("play");
			}
		}

		void load(void* arg)
		{
			LOGGER->msg("Loading map file..","LoadState");

			try
			{
				MapInfoPtr mapInfo = core->getMapLoader()->load();
				setMapInfo(mapInfo);
				core->getMapParser()->parser(mapInfo);

				core->getCoreFlow()->ready();

				NETWORK_SYSTEM_PTR->gameLoad("netConfig.json");
				((divacore::MultiPlay*)GAME_MODE_PTR)->gameLoadUnsync("");

				if(((divacore::MultiPlay*)GAME_MODE_PTR)->getBaseState()!=divacore::MultiPlay::READY)
					return;

				TimeCounter timeCounter;
				double nowTime = timeCounter.getTime(),tmpTime;
				while(!bReadyToStart)
				{ 
					Sleep(100);
					tmpTime = timeCounter.getTime();
					//CORE_PTR->holdOnConnection(tmpTime-nowTime);
					NETWORK_SYSTEM_PTR->update(tmpTime-nowTime);
					nowTime = tmpTime;
				}
				//while(!bReadyToStart);

				core->ready();
			}
			catch(Exception &ev)
			{
				LOGGER->error(ev);
				sora::SoraCore::Ptr->shutDown();
			}

			LOGGER->msg("Completed, ready to start","LoadState");
		}
	};
}

#endif