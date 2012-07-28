/*
 *  DivaCoreLoadState.h
 *
 *  Created by Hyf042 on 7/25/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_CORE_LOADER
#define DIVA_CORE_LOADER

#include "Core/DivaCoreState.h"
#include "Core/DivaMapLoader.h"
#include "Core/DivaTask.h"
#include "SoraText.h"
//#include "SoraFMODSoundSystem/SoraFMODSoundSystem.h"
//#include "SoraAudiereSoundSystem/SoraAudiereSoundSystem.h"

namespace divacore
{
	using namespace sora;

	/*
	 * class : CoreLoader
	 * description : a loader to load map and ready to play
	 */
	class CoreLoader : public CoreState
	{
		sora::SoraFont* mFont;
		sora::SoraText mText;
		Task task,readyCallback;

		bool bUnsync,bAutoPlay,bEmpty; //multithread or not
		enum{DELAY,READY,START,FINISH};

		int state; //delay a frame to render the preview image
	public:
		CoreLoader(bool bUnsync = true, bool bAutoPlay = true, bool bEmpty = false):bUnsync(bUnsync),bAutoPlay(bAutoPlay),bEmpty(bEmpty),state(DELAY){}
		void onInitiate()
		{
			state = DELAY;

			mFont = sora::SoraFont::LoadFromFile("cour.ttf", 20);

			mText.setFont(mFont);
			mText.setText(L"|#FF0000|Loading");
		}
		void onEnter()
		{
			setCoreState(Core::PREPARE);

			state = DELAY;
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

		void onUpdate(float dt)
		{
			if(state==DELAY)
				state = READY;
			else if(state==READY)
			{
				state = START;
				task.setMultiThread(!bUnsync);
				task.setAsMemberFunc(&CoreLoader::load, this);
				task.start();
			}
			else if(state==START)
			{
				static int count = 0;
				count = (count+1)%40;
				SoraWString text = L"|#FF0000|Loading";
				for(int i = 0; i < count/10; i++)
					text += L".";
				mText.setText(text);
				if(!task.isRunning())
				{
					mText.setText(L"|#FF0000|Finished!");
					sora::log_notice("loading succeed!");
					state = FINISH;

					//setCoreState(Core::RESULT);
					RENDER_SYSTEM_PTR->fadeOut(sora::Color::White.getHWColor());
					//core->setState("play");
				}
			}
			else
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
				if(!bEmpty)
				{
					MapInfoPtr mapInfo = core->getMapLoader()->load();
					setMapInfo(mapInfo);
				}

				core->getMapParser()->parser(MAP_INFO);
		
				core->getCoreFlow()->ready();

				core->ready();
			}
			catch(Exception &ev)
			{
				LOGGER->error(ev);
				sora::SoraCore::Ptr->shutDown();
			}

			LOGGER->msg("Completed, ready to start","LoadState");
		}

		MapInfoPtr makeEmptySong(int barNum, double BPM)
		{
			MapInfoPtr mapInfo = new MapInfo;

			//mapInfo->header.barNum = barNum;
			mapInfo->header.BPM = BPM;
			mapInfo->header.versionStr =  core->getVersion();
			mapInfo->header.difficulty = MapHeader::NORMAL;
			mapInfo->header.mapName = "new";
			mapInfo->header.speedScale = 1.0;
			mapInfo->module = "standard";

			setMapInfo(mapInfo);

			return mapInfo;
		}

		void setSong(const std::string &path, const std::string &mapFileName)
		{
			core->setSong(path, mapFileName);
		}

		void registerReadyCallback(Task &task) {readyCallback = task;}
	};
}

#endif