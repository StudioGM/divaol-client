/*
 *  DivaAutoplay.h
 *
 *  Created by Hyf042 on 2/3/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_AUTOPLAY
#define DIVA_AUTOPLAY

#include "Core/DivaHook.h"

namespace divacore
{
	class AutoPlay : public Hook
	{
		bool open;
	public:
		std::string getName() {return "AutoPlay";}

		AutoPlay()
		{
			setPriority(Hook::NORMAL);
			open = true;
		}
		bool condition()
		{
			return true;
		}
		int getAbility()
		{
			return Hook::STATE|Hook::INPUT;
		}

		bool hook(StateEvent &event)
		{
			if(!open)
				return false;
			if(event.type==StateEvent::POINT)
			{
				setHookInfo(1);
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
			uint32 color = sprite->getColor();
			setHookInfo(1);

			sprite->setColor(CSETA(color,int(CGETA(color)*0.2)));

			sprite->render(x,y);

			sprite->setColor(color);

			return true;
		}
	};
}

#endif