/*
 *  DivaDeathMode.h
 *
 *  Created by Hyf042 on 9/6/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_DEATH_MODE
#define DIVA_DEATH_MODE

#include "Core/DivaHook.h"
#include "Core/DivaCore.h"

namespace divacore
{
	class DeathMode : public Hook
	{
	public:
		std::string getName() {return "DeathMode";}

		DeathMode()
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
			return Hook::STATE;
		}
		bool hook(StateEvent &event)
		{
			if(event.tag=="inform")
			{
				if(event.dtHP<0)
				{
					if(GAME_MODE_PTR->getHP()+event.dtHP>0)
						event.dtHP = 1-GAME_MODE_PTR->getHP();
				}
			}
			return false;
		}
		float scoreBonusScale() {return 1.0;}
	};
}

#endif