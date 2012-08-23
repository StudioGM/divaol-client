/*
 *  DivaEditMode.h
 *
 *  Created by Hyf042 on 5/5/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_EDIT_MODE
#define DIVA_EDIT_MODE

#include "Core/DivaGameMode.h"
#include "Utility/DivaConfigLoader.h"
#include "Core/DivaCore.h"

namespace divacore
{
	//EditMode, ±‡º≠ƒ£ Ω
	class EditMode : public GameMode
	{
		class UnableKeyboard : public Hook
		{
			friend class EditMode;
			bool open;
		public:
			std::string getName() {return "UnableKeyboard";}

			UnableKeyboard()
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
				return Hook::INPUT;
			}

			bool hook(KeyEvent &key) 
			{
				if(open&&key.key>=0&&key.key<8)
				{
					setHookInfo(1);
					return true;
				}
				return false;
			}
		}unableHook;
	protected:
		bool mPlayable;
		bool mHitSound;
		bool mMissSound;
	public:		
		EditMode():mPlayable(false),mHitSound(true),mMissSound(true) {unableHook.open=!mPlayable;}
		~EditMode() {HOOK_MANAGER_PTR->del(&unableHook);}

		virtual std::string getName() {return "editMode";}

		bool getAlive() {return true;}
		int32 getScore() {return 0;}
		int32 getHP() {return 100;}
		float getHPinRatio() {return 1.0;}
		int32 getCombo() {return 0;}
	
		float getProtectedTime() {return 0.1;}

		bool checkNote(NotePtr note) {return true;}
		bool checkExtra(StateEvent& event) {return true;}
		bool checkPress(StateEvent& event) 
		{
			if(!EVALUATE_STRATEGY_PTR->evaluatePress(event))
				return false;
			event.type = StateEvent::PRESS;

			return true;
		}
		bool checkPoint(StateEvent& event) {return !mPlayable;}
		bool checkFailure(StateEvent& event) 
		{
			EVALUATE_STRATEGY_PTR->evaluateFaliure(event);
			event.type = StateEvent::FAILURE;
			event.breakCombo = event.breakNote = true;
			return true;
		}
		void inform(StateEvent& event) 
		{
			event.tag = "inform";
			if(mPlayable)
				HOOK_MANAGER_PTR->hook(event);
			//≤•∑≈hit“Ù
			if(event.type==StateEvent::PRESS||event.type==StateEvent::FAILURE)
			{
				if(event.rank<=4)
				{
					if(mHitSound)
						Core::Ptr->getMusicManager()->playDirect("hit","sound_effect");
				}
				else
				{
					if(mMissSound)
						Core::Ptr->getMusicManager()->playDirect("miss","sound_effect");
				}
			}
		}
		void update(float dt) {}
		
		bool isPlayable() {return mPlayable;}
		void setPlayble(bool flag) {mPlayable=flag,unableHook.open=!flag;}
		void setSound(bool hit, bool miss) {mHitSound=hit,mMissSound=miss;}
		void pressEffect(StateEvent &event) {}
		void addHookIn() {HOOK_MANAGER_PTR->insert(&unableHook);}

		void preStart() {}
		void preEvaluate() {}
	};
}

#endif