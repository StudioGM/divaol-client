/*
 *  DivaNetLoader.h
 *
 *  Created by tempbuffer on 8/9/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 */

#ifndef DIVA_NET_LOADER
#define DIVA_NET_LOADER

#include "divapomelo/diva/Client.h"
#include "mode/DivaMultiplay.h"

#include "Core/DivaCoreState.h"
#include "Core/DivaMapLoader.h"
#include "Core/DivaTask.h"
#include "Utility/DivaSettings.h"
//#include "SoraFMODSoundSystem/SoraFMODSoundSystem.h"
//#include "SoraAudiereSoundSystem/SoraAudiereSoundSystem.h"

namespace divacore
{
	using namespace sora;

	/*
	 * class : CoreLoader
	 * description : a loader to load map and ready to play
	 */
#if defined(DIVA_GNET_OPEN) && !defined(DIVA_USE_POMELO)
	class NetLoader : public CoreState
	{
		sora::SoraFont* mFont;
		sora::SoraText mText;
		Task task,readyCallback;

		bool bUnsync,bAutoPlay,bEmpty; //multithread or not
		enum{DELAY,READY,START,GAME_RUN,FAILURE,FAILED,FINISH,FADEOUT};

		int state; //delay a frame to render the preview image
	public:
		NetLoader(bool bUnsync = true, bool bAutoPlay = true, bool bEmpty = false):bUnsync(bUnsync),bAutoPlay(bAutoPlay),bEmpty(bEmpty),state(DELAY){}
		void onInitiate()
		{
			state = DELAY;

#ifdef _DEBUG
			mFont = sora::SoraFont::LoadFromFile(SETTINGS.getGlobalFontName().asUnicode(), 20);

			mText.setFont(mFont);
#endif
			mText.setText(L"|#FF0000|Loading");
		}
		void onEnter()
		{
			setCoreState(Core::PREPARE);
#if defined(DIVA_GNET_OPEN) && !defined(DIVA_USE_POMELO)
			GNET_RECEIVE_PACKET("game#start",&NetLoader::readyToStart);
#else
			POMELO_GAME_PEER->on(divapomelo::EventCode[divapomelo::ON_GAME_START], Base::Bind(this, &NetLoader::onStart));
#endif
			state = DELAY;
		}
		void onLeave()
		{
			GNET_UNRECEIVE_PACKET("game#start");

			task.stop();
		}

		void onRender()
		{
#ifdef _DEBUG
			mText.render();
#endif
		}

		void readyToStart(GPacket*)
		{
			state = GAME_RUN;
		}

		void onStart(divapomelo::MessageReq &req) {
			state = GAME_RUN;
		}

		void onUpdate(float dt)
		{
			if(state==DELAY)
				state = READY;
			else if(state==READY)
			{
				state = START;
				task.setMultiThread(!bUnsync);
				task.setAsMemberFunc(&NetLoader::load, this);
				task.start();
			}
			else if(state==START)
			{
				static int count = 0;
				count = (count+1)%40;
				SoraWString text = L"|#FF0000|";
				if(((MultiPlay*)GAME_MODE_PTR)->getBaseState()==MultiPlay::CONNECTING)
					text += L"Connect";
				else if(((MultiPlay*)GAME_MODE_PTR)->getBaseState()==MultiPlay::FAILURE)
					text += L"Failed";
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
			else if(state==FINISH)
			{
				if(!task.isRunning())
				{
					mText.setText(L"|#FF0000|Finished!");
					sora::log_notice("loading succeed!");
					state = FADEOUT;

					//setCoreState(Core::RESULT);
					RENDER_SYSTEM_PTR->fadeOut(sora::Color::White.getHWColor());
					//core->setState("play");
				}
			}
			else if(state==FAILURE)
			{
				SoraWString text = L"|#FF0000|";
				text += L"Failed";
				mText.setText(text);
			}
			else if(state==FAILED)
			{
#if defined(DIVA_GNET_OPEN) && !defined(DIVA_USE_POMELO)
				STAGE_CLIENT.returnToStage("start_failed");
#else
				POMELO_STAGE_PEER->returnToStage("start_failed");
#endif
				core->flowOver();
				core->over();
				state = FAILURE;
			}
			else if(state==FADEOUT)
			{
				if(!RENDER_SYSTEM_PTR->isFade())
				{
					readyCallback.run();
					if(bAutoPlay)
						core->setState("play");
				}
			}
		}

		void load(void* arg)
		{
			LOGGER->msg("Loading map file..","LoadState");
			
			try
			{
				MapInfoPtr mapInfo = core->getMapLoader()->load();
				setMapInfo(mapInfo);

				core->getMapParser()->parser(MAP_INFO);
		
				core->getCoreFlow()->ready();
				
				GAME_MODE_PTR->gameLoad("");

				LOGGER->msg("Wait for start...","NetworkSystem");

				while(state != GAME_RUN && ((divacore::MultiPlay*)GAME_MODE_PTR)->getBaseState()!=divacore::MultiPlay::FAILED)
				{
#if defined(DIVA_GNET_OPEN) && !defined(DIVA_USE_POMELO)
					NETWORK_SYSTEM_PTR->waitForNext();
					NETWORK_SYSTEM_PTR->refresh();
#else
					POMELO_CLIENT.logic();
#endif
				}

				if(state!=GAME_RUN)
				{
					state = FAILED;
					return;
				}

				state = FINISH;

				core->ready();
			}
			catch(Exception &ev)
			{
				LOGGER->error(ev);
				sora::SoraCore::Ptr->shutDown();
			}
			catch(...)
			{
				LOGGER->log("connection failed");
				state = FAILURE;
			}

			LOGGER->msg("Completed, ready to start","LoadState");
		}

		void setSong(const std::wstring &path, const std::wstring &mapFileName)
		{
			core->setSong(path, mapFileName);
		}

		void registerReadyCallback(Task &task) {readyCallback = task;}
	};
#else
	class NetLoader : public CoreState
	{
		sora::SoraFont* mFont;
		sora::SoraText mText;
		Task task,readyCallback;

		bool bUnsync,bAutoPlay,bEmpty; //multithread or not
		enum{DELAY,READY,START,GAME_RUN,FAILURE,FAILED,FINISH,FADEOUT};

		int state; //delay a frame to render the preview image
	public:
		NetLoader(bool bUnsync = true, bool bAutoPlay = true, bool bEmpty = false):bUnsync(bUnsync),bAutoPlay(bAutoPlay),bEmpty(bEmpty),state(DELAY){}
		void onInitiate()
		{
			state = DELAY;

#ifdef _DEBUG
			mFont = sora::SoraFont::LoadFromFile(SETTINGS.getGlobalFontName().asUnicode(), 20);

			mText.setFont(mFont);
#endif
			mText.setText(L"|#FF0000|Loading");
		}
		void onEnter()
		{
			setCoreState(Core::PREPARE);
			POMELO_GAME_PEER->on(divapomelo::EventCode[divapomelo::ON_GAME_START], Base::Bind(this, &NetLoader::onStart));
			state = DELAY;
		}
		void onLeave()
		{
			POMELO_GAME_PEER->remove(divapomelo::EventCode[divapomelo::ON_GAME_START]);

			task.stop();
		}

		void onRender()
		{
#ifdef _DEBUG
			mText.render();
#endif
		}

		void readyToStart(GPacket*)
		{
			state = GAME_RUN;
		}

		void onStart(divapomelo::MessageReq &req) {
			state = GAME_RUN;
		}

		void onUpdate(float dt)
		{
			if(state==DELAY)
				state = READY;
			else if(state==READY)
			{
				state = START;
				task.setMultiThread(!bUnsync);
				task.setAsMemberFunc(&NetLoader::load, this);
				task.start();
			}
			else if(state==START)
			{
				static int count = 0;
				count = (count+1)%40;
				SoraWString text = L"|#FF0000|";
				if(((MultiPlay*)GAME_MODE_PTR)->getBaseState()==MultiPlay::CONNECTING)
					text += L"Connect";
				else if(((MultiPlay*)GAME_MODE_PTR)->getBaseState()==MultiPlay::FAILURE)
					text += L"Failed";
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
			else if(state==FINISH)
			{
				if(!task.isRunning())
				{
					mText.setText(L"|#FF0000|Finished!");
					sora::log_notice("loading succeed!");
					state = FADEOUT;

					//setCoreState(Core::RESULT);
					RENDER_SYSTEM_PTR->fadeOut(sora::Color::White.getHWColor());
					//core->setState("play");
				}
			}
			else if(state==FAILURE)
			{
				SoraWString text = L"|#FF0000|";
				text += L"Failed";
				mText.setText(text);
			}
			else if(state==FAILED)
			{
				POMELO_STAGE_PEER->returnToStage("start_failed");
				core->flowOver();
				core->over();
				state = FAILURE;
			}
			else if(state==FADEOUT)
			{
				if(!RENDER_SYSTEM_PTR->isFade())
				{
					readyCallback.run();
					if(bAutoPlay)
						core->setState("play");
				}
			}
		}

		void load(void* arg)
		{
			LOGGER->msg("Loading map file..","LoadState");
			
			try
			{
				MapInfoPtr mapInfo = core->getMapLoader()->load();
				setMapInfo(mapInfo);

				core->getMapParser()->parser(MAP_INFO);
		
				core->getCoreFlow()->ready();
				
				GAME_MODE_PTR->gameLoad("");

				LOGGER->msg("Wait for start...","NetworkSystem");

				while(state != GAME_RUN && ((divacore::MultiPlay*)GAME_MODE_PTR)->getBaseState()!=divacore::MultiPlay::FAILED)
				{
					POMELO_CLIENT.logic();
				}

				if(state!=GAME_RUN)
				{
					state = FAILED;
					return;
				}

				state = FINISH;

				core->ready();
			}
			catch(Exception &ev)
			{
				LOGGER->error(ev);
				sora::SoraCore::Ptr->shutDown();
			}
			catch(...)
			{
				LOGGER->log("connection failed");
				state = FAILURE;
			}

			LOGGER->msg("Completed, ready to start","LoadState");
		}

		void setSong(const std::wstring &path, const std::wstring &mapFileName)
		{
			core->setSong(path, mapFileName);
		}

		void registerReadyCallback(Task &task) {readyCallback = task;}
	};
#endif
}

#endif