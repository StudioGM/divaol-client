/*
 *  DivaCoreLoadState.h
 *
 *  Created by Hyf042 on 1/14/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_NET_LOAD_STATE
#define DIVA_NET_LOAD_STATE

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
	class NetLoadState : public CoreState
	{
		bool bFinished;
		volatile bool bReadyToStart;

		sora::SoraFont* mFont;
		sora::SoraText mText;
		Task task;
	public:
		void onInitiate()
		{
			bFinished = false;
			bReadyToStart = false;

			mFont = sora::SoraFont::LoadFromFile("cour.ttf", 20);

			mText.setFont(mFont);
			mText.setText(L"|#FF0000|Loading");

			NETWORK_SYSTEM_PTR->receive(network::DIVA_NET_STC_START,new network::EventFunc(&NetLoadState::readyToStart,this));
		}
		void onEnter()
		{
			setCoreState(Core::PREPARE);

			bFinished = false;



			task.setMultiThread(true);
			task.setAsMemberFunc(&NetLoadState::load, this);
			task.start();
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
				NETWORK_SYSTEM_PTR->update(dt);

				if(!task.isRunning())
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
				GAME_MODE_PTR->gameLoad("");

				if(((divacore::MultiPlay*)GAME_MODE_PTR)->getBaseState()!=divacore::MultiPlay::READY)
					return;

				while(!bReadyToStart);

				core->ready();

				//Sleep(10000);
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