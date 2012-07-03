/*
 *  DivaFactory.h
 *
 *  Created by Hyf042 on 1/15/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_FACTORY
#define DIVA_FACTORY

#include "DivaCommon.h"
#include "DivaNote.h"
#include "DivaMapInfo.h"
#include "DivaEventManager.h"

namespace divacore
{
	/*
	ItemFactory
	factory to create note, event, etc
	*/
	class ItemFactory : public EventHandler
	{
	public:
		virtual NotePtr createNote(MapNote&) = 0;
		//NotePtr createEvent(MapEvent);
	};

	typedef ItemFactory* ItemFactoryPtr;
}

#endif