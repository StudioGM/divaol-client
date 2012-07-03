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
	//EditMode, ±à¼­Ä£Ê½
	class EditMode : public GameMode
	{
	protected:
	public:		
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
			event.rank = 1;
			event.type = StateEvent::PRESS;
			return true;
		}
		bool checkPoint(StateEvent& event) {return true;}
		bool checkFailure(StateEvent& event) {return true;}
		void inform(StateEvent& event) 
		{
			//²¥·ÅhitÒô
			if(event.type==StateEvent::PRESS||event.type==StateEvent::FAILURE)
			{
				if(event.rank<=4)
					Core::Ptr->getMusicManager()->playDirect("hit","sound_effect");
				else
					Core::Ptr->getMusicManager()->playDirect("miss","sound_effect");
			}
		}
		void update(float dt) {}

		void pressEffect(StateEvent &event) {}

		void preStart() {}
		void preEvaluate() {}
	};
}

#endif