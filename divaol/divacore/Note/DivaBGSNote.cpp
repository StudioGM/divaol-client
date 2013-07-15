/*
 *  DivaBGSNote.cpp
 *
 *  Created by Hyf042 on 1/28/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#include "DivaBGSNote.h"
#include "Core/DivaCore.h"
#include "Utility/DivaPath.h"

namespace divacore
{
	BGSNote::~BGSNote()
	{
	}

	void BGSNote::onInitiate() 
	{
	}
	void BGSNote::onEnter() {}
	void BGSNote::onLeave() 
	{
		Note::onLeave();
	}
	void BGSNote::onPoint(uint32 cnt, NotePoint point) 
	{
		if (getReceivePoint().key != "")
			Core::Ptr->getMusicManager()->playDirect(getReceivePoint().key, "sound_effect");
		over();
	}

	void BGSNote::onRender()
	{
	}
	void BGSNote::onUpdate(double dt, double position) 
	{
	}
	void BGSNote::onPressed(StateEvent& event)
	{
	}
	void BGSNote::onReleased(StateEvent& event)
	{
	}
	bool BGSNote::isReceive(KeyEvent& event, float time)
	{
		return false;
	}

	void BGSNote::recover(int point, int position, float time)
	{
	}
}
