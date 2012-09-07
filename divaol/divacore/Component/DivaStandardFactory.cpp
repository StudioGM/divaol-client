/*
 *  DivaStandardFactory.cpp
 *
 *  Created by Hyf042 on 1/15/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#include "DivaStandardFactory.h"
#include "Note/DivaNormalNote.h"
#include "Note/DivaLongNote.h"
//#include "Note/DivaContiNote.h"
#include "Note/DivaPingpongNote.h"
#include "Utility/DivaConfigLoader.h"
#include "Core/DivaCore.h"

namespace divacore
{
	void StandardItemFactory::init()
	{
	}

	void StandardItemFactory::gameLoad(const std::string &configFile)
	{
		configloader::loadWithJson(config,configFile);
	}

	void StandardItemFactory::gameLoadFromConfig(Config &config)
	{
		this->config = config;
	}

	void StandardItemFactory::gameReset()
	{
		config.clear();
		LOGGER->msg("Set","StandardItemFactory");
	}

	void StandardItemFactory::gameStop()
	{
	}

	NotePtr StandardItemFactory::createNote(MapNote &noteInfo)
	{
		HOOK_MANAGER_PTR->hook(noteInfo);

		NotePtr note = NULL;
		if(noteInfo.noteType=="normal")
			note = new NormalNote(noteInfo);
		else if(noteInfo.noteType=="long")
			note = new LongNote(noteInfo);
//		else if(noteInfo.noteType=="conti")
//			note = new ContiNote(noteInfo);
		else if(noteInfo.noteType=="pingpong")
			note = new PingpongNote(noteInfo);
		if(note==NULL||!note->checkInfo())
			DIVA_EXCEPTION_MODULE("noteInfo unsuitable","ItemFactory");
		note->setConfig(&config);
		note->onInitiate();

		HOOK_MANAGER_PTR->hook(note);
		
		return note;
	}
}