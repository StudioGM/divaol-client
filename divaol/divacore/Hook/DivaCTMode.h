/*
 *  DivaPVMode.h
 *
 *  Created by Hyf042 on 2/4/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_CTMODE
#define DIVA_CTMODE

#include "Core/DivaHook.h"

namespace divacore
{
	const int MAX_LEVEL = 7;
	const float RATE[MAX_LEVEL] = {1,1.5,2,2.5,3,3.5,4};
	const float SpeedUp[MAX_LEVEL] = {1,1,2,4,5,6,7};
	const float Duration[MAX_LEVEL] = {10,10,10,10,10,10,10};
	const float CostValueWithTime[MAX_LEVEL][2] = {{0,GRID_PER_BAR},
									{100,GRID_PER_BAR},
									{125,GRID_PER_BAR},
									{150,GRID_PER_BAR},
									{175,GRID_PER_BAR},
									{200,GRID_PER_BAR},
									{250,GRID_PER_BAR}};
	const int CostValueWithRank[MAX_LEVEL][5] = {
		{60,30,0,-20,-20},
		{60,30,0,-20,-20},
		{60,30,0,-20,-20},
		{60,30,0,-50,-100},
		{60,20,0,-150,-300},
		{60,10,0,-250,-500},
		{60,0,0,-500,-1000}};

	class CTMode : public Hook
	{
		static const int LIMIT_CT = 1000;
		static const int ORIGIN_CT = 0;
		double ct;
		int maxLevel;
		int level;
		float duration;
		bool bFull;
		float originSpeedScale;
	public:
		std::string getName() {return "CTMode";}

		CTMode()
		{
			setPriority(Hook::TOP);
			setType(Hook::MODE);
			originSpeedScale = MAP_INFO->header.speedScale;
			setLevel(0);
			maxLevel = 0;
		}
		void setLevel(int level, bool keep = false)
		{
			maxLevel = std::max(level,maxLevel);
			if(!keep) {
				bFull = false;
				ct = ORIGIN_CT;
			}
			this->level = level;
			if(level>0)
				duration = Duration[level];
			MAP_INFO->header.speedScale = originSpeedScale*SpeedUp[level];
		}
		void onAwake()
		{
			setLevel(0);
		}
		bool condition()
		{
			if(CORE_FLOW_PTR->getState()==CoreFlow::RUN)
				return true;
			return false;
		}
		float getCTRatio() {return ct/float(LIMIT_CT);}
		int getAbility()
		{
			return Hook::STATE|Hook::INPUT;
		}
		void update(float dt)
		{
			static int lastPosition =  CORE_PTR->getRunPosition();
			int nowPosition = CORE_PTR->getRunPosition();

			if(!bFull) {
				ct -= CostValueWithTime[level][0]*(nowPosition-lastPosition)/CostValueWithTime[level][1];
				if(ct<0)
					ct = 0;
			}

			if(level>0) {
				duration -= dt;
				if(duration<0) {
					duration = 0;
					setLevel(0, true);
				}
			}

			lastPosition = nowPosition;
		}
		bool hook(StateEvent &event)
		{
			if(GAME_MODE_PTR->getAlive()&&event.isOwner()&&event.tag=="inform")
			{
				if(!bFull&&(event.type==StateEvent::PRESS||event.type==StateEvent::FAILURE))
				{
					ct += CostValueWithRank[level][event.rank-1];
					if(ct>LIMIT_CT) {
						ct = LIMIT_CT;
						bFull = true;
					}
					else if(ct<0)
						ct = 0;
				}
				event.dtScore *= RATE[level];
			}
			return false;
		}

		bool hook(KeyEvent &key) 
		{
			if(key.key==DIVA_KEY_CT)
				if(bFull&&level+1<MAX_LEVEL)
				{
					setLevel(level+1);
					MUSIC_MANAGER_PTR->playDirectWithFile("Data/ct.mp3",true,"se");
				}
			return false;
		}

		int getCT() {return (int)ct;}
		int getLevel() {return level;}
		int getMaxLevel() {return maxLevel;}
	};
}

#endif