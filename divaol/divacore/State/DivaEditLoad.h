/*
 *  DivaEditLoadState.h
 *
 *  Created by Hyf042 on 7/9/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_EDIT_LOAD_STATE
#define DIVA_EDIT_LOAD_STATE

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
	class EditLoadState : public CoreState
	{
		Task readyTask;

	public:
		MapInfoPtr makeEmptySong(int barNum, double BPM)
		{
			MapInfoPtr mapInfo = new MapInfo;

			mapInfo->header.barNum = barNum;
			mapInfo->header.BPM = BPM;
			mapInfo->header.versionStr =  core->getVersion();
			mapInfo->header.difficulty = MapHeader::NORMAL;
			mapInfo->header.mapName = "new";
			mapInfo->header.speedScale = 1.0;
			mapInfo->module = "standard";

			setMapInfo(mapInfo);

			return mapInfo;
		}

		void onEnter()
		{
			run();
		}

		void setMapInfo(MapInfoPtr mapInfo)
		{
			CoreState::setMapInfo(mapInfo);
		}

		void run()
		{
			if(!MAP_INFO)
			{
				MapInfoPtr mapInfo = core->getMapLoader()->load();
				setMapInfo(mapInfo);

				LOGGER->msg("load from file");
			}

			core->getMapParser()->parser(MAP_INFO);

			core->getCoreFlow()->ready();

			core->ready();

			//readyTask
			readyTask.start();
		}

		void registerReayCallback(Task &task) {readyTask = task;}
	};
}
#endif