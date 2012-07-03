/*
 *  DivaStandardFactory.h
 *
 *  Created by Hyf042 on 1/15/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_STANDARD_FACTORY
#define DIVA_STANDARD_FACTORY

#include "Core/DivaCommon.h"
#include "Core/DivaNote.h"
#include "Core/DivaMapInfo.h"
#include "Core/DivaFactory.h"
#include "Core/DivaConfig.h"

namespace divacore
{
	const std::string NOTE_MAP[8] = {"circle","rectangle","cross","triangle","right","left","down","up"};
	
	class StandardItemFactory : public ItemFactory
	{
		Config config;
	public:
		void init();
		void gameLoad(const std::string &configFile);
		void gameReset();
		void gameStop();
		virtual NotePtr createNote(MapNote&);
		//NotePtr createEvent(MapEvent);
	}; 
}

#endif