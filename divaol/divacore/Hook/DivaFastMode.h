/*
 *  DivaFastMode.h
 *
 *  Created by Hyf042 on 9/6/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_FAST_MODE
#define DIVA_FAST_MODE

#include "Core/DivaHook.h"
#include "Core/DivaCore.h"

namespace divacore
{
	class FastMode : public Hook
	{
	public:
		std::string getName() {return "FastMode";}

		FastMode()
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
			return 0;
		}
		void onAwake() 
		{
			CORE_PTR->setSpeedScale(1.25);
		}
		float scoreBonusScale() {return 0.8;}
	};
}

#endif