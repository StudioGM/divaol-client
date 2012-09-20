/*
 *  DivaRandomSwapMode.h
 *
 *  Created by Hyf042 on 9/6/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_RANDOM_SWAP_MODE
#define DIVA_RANDOM_SWAP_MODE

#include "Core/DivaHook.h"
#include "Core/DivaCore.h"

namespace divacore
{
	class RandomSwapMode : public Hook
	{
		static const int RANDOM_TIME = 100;
		uint8 mSwap[NOTE_NUM];
	public:
		std::string getName() {return "RandomSwapMode";}

		RandomSwapMode()
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
		void onAwake() 
		{
			for(int i = 0; i < NOTE_NUM; i++)
				mSwap[i] = i;
			for(int i = 0; i < RANDOM_TIME; i++)
			{
				int L = sora::SoraCore::RandomInt(0,NOTE_NUM), R = sora::SoraCore::RandomInt(0,NOTE_NUM);
				std::swap(mSwap[L],mSwap[R]);
			}
		}
		bool hook(MapNote &note)
		{
			note.notePoint[0].type = mSwap[note.notePoint[0].type];
			
			return false;
		}
		float scoreBonusScale() {return 0.1;}
	};
}

#endif