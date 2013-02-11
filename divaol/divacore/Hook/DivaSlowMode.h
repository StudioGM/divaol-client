/*
 *  DivaSlowMode.h
 *
 *  Created by Hyf042 on 9/6/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_SLOW_MODE
#define DIVA_SLOW_MODE

#include "Core/DivaHook.h"
#include "Core/DivaCore.h"

namespace divacore
{
	class SlowMode : public Hook
	{
	public:
		std::string getName() {return "SlowMode";}

		SlowMode()
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
			CORE_PTR->setSpeedScale(0.75);
		}
		float scoreTotalScale() {return 0.2;}
	};
}

#endif