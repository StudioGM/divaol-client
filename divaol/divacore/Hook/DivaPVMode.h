/*
 *  DivaPVMode.h
 *
 *  Created by Hyf042 on 2/4/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_PVMODE
#define DIVA_PVMODE

#include "Core/DivaHook.h"
#include "Core/DivaCore.h"

namespace divacore
{
	class PVMode : public Hook
	{
	public:
		std::string getName() {return "PVMode";}

		PVMode()
		{
			setPriority(Hook::HIGH);
		}
		bool condition()
		{
			UI_PAINTER_PTR->hide();
			return true;
		}
		int getAbility()
		{
			return Hook::STATE|Hook::INPUT|Hook::SOUND|Hook::RENDER;
		}

		bool hook(StateEvent &event)
		{
			if(event.type==StateEvent::POINT)
			{
				setHookInfo(1);
				return true;
			}
			else if(event.tag=="inform")
			{
				event.dtHP = event.dtScore = 0, event.breakCombo = true;
				return true;
			}
			return false;
		}

		bool hook(KeyEvent &key) 
		{
			if(key.key>=0&&key.key<8)
			{
				setHookInfo(1);
				return true;
			}
			return false;
		}

		bool hook(float x, float y, SoraSprite*sprite, const std::string&tag) 
		{
			return true;
		}

		bool hook(const std::string &ID, const std::string &channel, const std::string &tag) 
		{
			if(tag=="sound_effect")
			{
				setHookInfo(1);
				return true;
			}
			return false;
		}
	};
}

#endif