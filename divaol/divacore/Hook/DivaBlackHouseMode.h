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

namespace divacore
{
	class BlackHouseMode : public Hook
	{
		Point mHolePosition;
		Point mTargetPosition;
		sora::SoraSprite *mBlackCover;
		sora::SoraSprite *mBlackHole;
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
			mBlackHole = sora::SoraCore::Ptr->createSprite("spotlight.png");
			mBlackHole->setCenter(mBlackHole->getSpriteWidth()/2,mBlackHole->getSpriteHeight()/2);
			mBlackCover = sora::SoraCore::Ptr->createSprite("spotlight.png");
			mBlackCover->setTextureRect(0,0,100,100);
		}
		void render() {
			if(mHolePosition.x>=0) {
				mBlackHole->render(mHolePosition.x,mHolePosition.y);
				Rect drawRect = Rect(mHolePosition.x-mBlackHole->getSpriteWidth()/2, mHolePosition.y-mBlackHole->getSpriteHeight()/2,
					mBlackHole->getSpriteWidth(), mBlackHole->getSpriteHeight());
				drawBlackBar(Rect(0,0,drawRect.x+10,RENDER_SYSTEM_PTR->getGameHeight()));
				drawBlackBar(Rect::makeRectWidthPoints(drawRect.x+drawRect.w-10,0,RENDER_SYSTEM_PTR->getGameWidth(),RENDER_SYSTEM_PTR->getGameHeight()));
				drawBlackBar(Rect(drawRect.x-10,0,drawRect.w+20,drawRect.y+10));
				drawBlackBar(Rect::makeRectWidthPoints(drawRect.x-10,drawRect.y+drawRect.h-10,drawRect.x+drawRect.w+10,RENDER_SYSTEM_PTR->getGameHeight()));
			}
		}
		void update(float dt) {
			mHolePosition = mHolePosition+(mTargetPosition-mHolePosition)*0.33*dt*5;
			if((mTargetPosition-mHolePosition).mod()<10)
				mHolePosition = mTargetPosition;
		}
		void drawBlackBar(Rect drawRect) {
			if(drawRect.w<=0||drawRect.h<=0)
				return;
			mBlackCover->setScale(drawRect.w/mBlackCover->getSpriteWidth(),drawRect.h/mBlackCover->getSpriteHeight());
			mBlackCover->render(drawRect.x,drawRect.y);
		}
		float scoreBonusScale() {return 0.5;}
	};
}

#endif