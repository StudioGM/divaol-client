/*
 *  DivaDisappearMode.h
 *
 *  Created by Hyf042 on 9/6/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_DISAPPEAR_MODE
#define DIVA_DISAPPEAR_MODE

#include "Core/DivaHook.h"
#include "Core/DivaCore.h"

namespace divacore
{
	class DisappearMode : public Hook
	{
	public:
		std::string getName() {return "DisappearMode";}

		DisappearMode()
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
			return Hook::RENDER|Hook::NOTE;
		}
		bool hook(NotePtr note)
		{
			// why I type true here before?
			return false;
		}
		bool hook(float x, float y, SoraSprite*sprite, const std::string&tag) 
		{
			if(tag.find("note+")!=std::string::npos||tag.find("note_cover+")!=std::string::npos)
			{
				if(!sprite->hasEffect())
					sprite->addEffect(sora::CreateEffectList(
										sora::CreateEffectFade(1,0,1/CORE_PTR->getSpeedScale()),
										sora::CreateEffectFade(0,0,INFINITE),
										sora::ImageEffectOnce));
			}
			return false;
		}
		float scoreBonusScale() {return 0.5;}
	};
}

#endif