/*
 *  DivaCoreLoadState.h
 *
 *  Created by Hyf042 on 1/14/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_CORE_LOAD_STATE
#define DIVA_CORE_LOAD_STATE

#include "Core/DivaCoreState.h"
#include "Core/DivaMapLoader.h"
#include "Core/DivaTask.h"
//#include "SoraFMODSoundSystem/SoraFMODSoundSystem.h"
//#include "SoraAudiereSoundSystem/SoraAudiereSoundSystem.h"

namespace divacore
{
	using namespace sora;

	/*
	CoreLoadState
	A state for loading resource and show loading interface
	*/
	class CoreLoadState : public CoreState
	{
		bool bFinished;

		sora::SoraFont* mFont;
		sora::SoraText mText;
		Task task;
	public:
		void onInitiate()
		{
			bFinished = false;

			mFont = sora::SoraFont::LoadFromFile("cour.ttf", 20);

			mText.setFont(mFont);
			mText.setText(L"|#FF0000|Loading");
		}
		void onEnter()
		{
			setCoreState(Core::PREPARE);

			bFinished = false;

			task.setMultiThread(true);
			task.setAsMemberFunc(&CoreLoadState::load, this);
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

		void onUpdate(float dt)
		{
			if(!bFinished)
			{
				NETWORK_SYSTEM_PTR->update(dt);

				if(!task.isRunning())
				{
					mText.setText(L"|#FF0000|Finished!");
					sora::log_notice("loading succeed!");
					bFinished = true;

					//setCoreState(Core::RESULT);
					RENDER_SYSTEM_PTR->fadeOut(sora::Color::White.getHWColor());
					//core->setState("play");
				}
				else
				{
					static int count = 0;
					count = (count+1)%40;
					SoraWString text = L"|#FF0000|Loading";
					for(int i = 0; i < count/10; i++)
						text += L".";
					mText.setText(text);
				}
			}
			else
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

	typedef CoreLoadState* CoreLoadStatePtr;
}

#endif