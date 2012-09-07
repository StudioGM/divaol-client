/*
 *  DivaChaosMode.h
 *
 *  Created by Hyf042 on 9/6/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_CHAOS_MODE
#define DIVA_CHAOS_MODE

#include "Core/DivaHook.h"
#include "Core/DivaCore.h"

namespace divacore
{
	class ChaosMode : public Hook
	{
	public:
		std::string getName() {return "ChaosMode";}

		ChaosMode()
		{
			setPriority(Hook::NORMAL);
			setType(Hook::MODE);
		}
		bool condition()
		{
			return true;
		}
		int getAbility()
		{
			return Hook::NOTE;
		}
		bool hook(MapNote &note)
		{
			note.notePoint[0].type = sora::SoraCore::RandomInt(0,NOTE_NUM);
			
			return false;
		}
		float scoreBonusScale() {return 1;}
	};
}

#endif