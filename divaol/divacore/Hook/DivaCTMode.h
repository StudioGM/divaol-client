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
	const float COST[MAX_LEVEL] = {0,100,125,150,175,200,250};
	const int DTCP[MAX_LEVEL][5] = {
		{60,30,0,0,0},
		{60,30,0,0,0},
		{60,30,0,0,0},
		{60,30,0,-50,-100},
		{60,20,0,-150,-300},
		{60,10,0,-250,-500},
		{60,0,0,-500,-1000}};

	class CTMode : public Hook
	{
		static const int LIMIT_CT = 1000;
		static const int ORIGIN_CT = 500;
		int ct;
		int level;
		int cntPosition;
	public:
		std::string getName() {return "CTMode";}

		CTMode()
		{
			setPriority(Hook::TOP);
			setLevel(0);
		}
		void setLevel(int level)
		{
			ct = ORIGIN_CT;
			this->level = level;
			cntPosition = 0;
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

			cntPosition += nowPosition-lastPosition;
			if(cntPosition>=GRID_PER_BAR*2)
			{
				cntPosition -= GRID_PER_BAR*2;
				ct -= COST[level]*2;
				if(ct<0)
					setLevel(0);
			}

			lastPosition = nowPosition;
		}
		bool hook(StateEvent &event)
		{
			if(GAME_MODE_PTR->getAlive()&&event.isOwner()&&event.tag=="inform")
			{
				if(event.type==StateEvent::PRESS)
				{
					ct += DTCP[level][event.rank-1];
					if(ct>LIMIT_CT)
						ct = LIMIT_CT;
					else if(ct<0)
						setLevel(0);
				}
				event.dtScore *= RATE[level];
			}
			return false;
		}

		bool hook(KeyEvent &key) 
		{
			if(key.key==DIVA_KEY_CT)
				if(ct==LIMIT_CT&&level+1<MAX_LEVEL)
				{
					setLevel(level+1);
					MUSIC_MANAGER_PTR->playDirectWithFile("Data/ct.mp3",true);
				}
			return false;
		}

		int getCT() {return ct;}
		int getLevel() {return level;}
	};
}

#endif