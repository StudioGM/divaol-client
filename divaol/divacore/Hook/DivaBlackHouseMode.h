/*
 *  DivaBlackHouseMode.h
 *
 *  Created by Hyf042 on 9/6/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_BLACKHOUSE_MODE
#define DIVA_BLACKHOUSE_MODE

#include "Core/DivaHook.h"
#include "Core/DivaCore.h"
#include "SoraShader.h"

namespace divacore
{
	class BlackHouseMode : public Hook
	{
		Point mHolePosition;
		Point mTargetPosition;
		sora::SoraShader *mSpotShader;
	public:
		std::string getName() {return "BlackHouseMode";}

		BlackHouseMode()
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
		bool hook(NotePtr note)
		{
			mTargetPosition = note->getReceiveScreenPosition();
			return false;
		}
		void onAwake() {
			mTargetPosition = mHolePosition = Point(RENDER_SYSTEM_PTR->getGameWidth()/2,RENDER_SYSTEM_PTR->getGameHeight()/2);
			
			mSpotShader = RENDER_SYSTEM_PTR->getInnerCanvas()->getCanvasSprite()->attachFragmentShader(HOOK_MANAGER_PTR->getConfig().getAsString("SpotLightShader"),"main");
		}
		void onSleep() {
			RENDER_SYSTEM_PTR->getInnerCanvas()->getCanvasSprite()->detachFragmentShader();
		}
		void update(float dt) {
			mHolePosition = mHolePosition+(mTargetPosition-mHolePosition)*0.33*dt*5;
			if((mTargetPosition-mHolePosition).mod()<10)
				mHolePosition = mTargetPosition;
			mSpotShader->setParameter2f("lightPoint",mHolePosition.x/RENDER_SYSTEM_PTR->getGameWidth(),mHolePosition.y/RENDER_SYSTEM_PTR->getGameHeight());
		}
		float scoreBonusScale() {return 0.5;}
	};
}

#endif