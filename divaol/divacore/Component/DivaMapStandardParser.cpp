/*
 *  DivaMapStandardParser.cpp
 *
 *  Created by Hyf042 on 1/13/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#include <algorithm>
#include "DivaMapStandardParser.h"
#include "SoraFileUtility.h"
#include "SoraCore.h"

namespace divacore
{
	std::wstring MapStandardParser::getFullPath(const std::wstring fileName)
	{
		if(SoraFileUtility::FileExists(MAP_LOADER_PTR->getSongPath()+L"/"+fileName))
			return MAP_LOADER_PTR->getSongPath()+L"/"+fileName;
		else
			return fileName;
	}

	void MapStandardParser::gameReset()
	{
		LOGGER->msg("Set","MapStandardParser");
		clear();
	}
	void MapStandardParser::gameStop()
	{
	}

	void MapStandardParser::clear()
	{
		mapInfo = NULL;
		noteTime.clear();
		eventTime.clear();
	}

	void MapStandardParser::_parseResource()
	{
		for(MapInfo::RESOURCES::iterator ptr = mapInfo->resources.begin(); ptr != mapInfo->resources.end(); ptr++)
		{
			std::string filePath = sora::ws2s(getFullPath(ptr->second.filePath));

			if(ptr->second.type==MapResourceInfo::AUDIO)
			{
				core->getMusicManager()->load(filePath,ptr->second.ID,ptr->second.flag);
			}
			else if(ptr->second.type==MapResourceInfo::VIDEO)
			{
				core->getDisplay()->loadVideo(filePath,ptr->second.ID);
			}
			else if(ptr->second.type==MapResourceInfo::IMAGE)
			{
				core->getDisplay()->loadImage(filePath,ptr->second.ID);
			}
			/*else*/
		}
		if(mapInfo->resources.find("hit")==mapInfo->resources.end())
			core->getMusicManager()->load("Data/hit.wav","hit",false);
		if(mapInfo->resources.find("miss")==mapInfo->resources.end())
			core->getMusicManager()->load("Data/miss.mp3","miss",false);

		MUSIC_MANAGER_PTR->play(mapInfo->header.mainSound,CORE_FLOW_PTR->MAIN_SOUND_CHANNEL);
		MUSIC_MANAGER_PTR->pause(CORE_FLOW_PTR->MAIN_SOUND_CHANNEL);
	}
	void MapStandardParser::_parseEvents()
	{
	}
	void MapStandardParser::_parseNotes()
	{
	}
	void MapStandardParser::_parseTime() 
	{
		std::vector<ParserNode> nodes;
		for(int i = 0; i < mapInfo->events.size(); i++)
		{
			MapEvent &event = mapInfo->events[i];
			if(event.eventType=="bpm")
				nodes.push_back(ParserNode(ParserNode::BPM,event.position,NULL,sora::AnyCast<double>(event.arg["value"])));
			else if(event.eventType=="bar")
				nodes.push_back(ParserNode(ParserNode::BAR,event.position,NULL,sora::AnyCast<double>(event.arg["value"])));
			nodes.push_back(ParserNode(ParserNode::EVENT,event.position,&event.time));
		}
		for(int i = 0; i < mapInfo->notes.size(); i++)
		{
			MapNote &noteInfo = mapInfo->notes[i];
			uint32 tPos = 0;
			if(noteInfo.notePoint[0].position>=noteInfo.aheadBar*GRID_PER_BAR)
				tPos = noteInfo.notePoint[0].position-noteInfo.aheadBar*GRID_PER_BAR;
			nodes.push_back(ParserNode(ParserNode::NOTEPOINT,tPos,&noteInfo.aheadTime));
			for(int j = 0; j < mapInfo->notes[i].notePoint.size(); j++)
			{
				NotePoint &notePoint =noteInfo.notePoint[j];
				nodes.push_back(ParserNode(ParserNode::NOTEPOINT,notePoint.position,&notePoint.time));
			}
		}

		std::sort(nodes.begin(),nodes.end());

		//time calculate
		double nowBar = 4;
		double nowBPM = mapInfo->header.BPM;
		int nowPosition = 0;
		double nowTime = 0, newTime;

		for(std::vector<ParserNode>::iterator ptr = nodes.begin(); ptr != nodes.end(); ptr++)
		{
			newTime = nowTime + (ptr->position-nowPosition)*SECOND_PER_MINUTE/(nowBPM*GRID_PER_BAR/nowBar);

			if(ptr->type==ParserNode::BPM)
			{
				nowBPM = ptr->tValue;
				nowPosition = ptr->position;
				nowTime = newTime;
			}
			else if(ptr->type==ParserNode::BAR)
			{
				nowBar = ptr->tValue;
				nowPosition = ptr->position;
				nowTime = newTime;
			}
			else
				(*ptr->pRetTime) = newTime;
		}

		mapInfo->totalTime = MUSIC_MANAGER_PTR->getLength(CORE_FLOW_PTR->MAIN_SOUND_CHANNEL);
		mapInfo->totalGrid = (int)ceil(nowPosition + (mapInfo->totalTime-nowTime)/SECOND_PER_MINUTE*(nowBPM*GRID_PER_BAR/nowBar));
	}
	void MapStandardParser::_parseModule()
	{
		GAME_MODULE_PTR->load(mapInfo->module);
		GAME_MODULE_PTR->distribute();
	}
	void MapStandardParser::parser(MapInfoPtr mapInfo)
	{
		clear();

		this->mapInfo = mapInfo;
		_parseResource();
		_parseModule();
		_parseTime();
		_parseEvents();
		_parseNotes();
	}
	void MapStandardParser::reParser(uint32 parserFlag)
	{
		if(parserFlag&PARSE_RESOURCES)
			_parseResource();
		if(parserFlag&PARSE_MODULE)
			_parseModule();
		if(parserFlag&PARSE_TIME)
			_parseTime();
	}
	void MapStandardParser::loadResource(MapResourceInfo info)
	{
		if(mapInfo->resources.find(info.ID)!=mapInfo->resources.end())
			DIVA_EXCEPTION_MODULE("RESOURCE "+info.ID+" already exists","StandardParser");

		mapInfo->resources[info.ID] = info;

		std::string filePath = sora::ws2s(getFullPath(info.filePath));

		if(info.type==MapResourceInfo::AUDIO)
			core->getMusicManager()->load(filePath,info.ID,info.flag);
		else if(info.type==MapResourceInfo::VIDEO)
			core->getDisplay()->loadVideo(filePath,info.ID);
		else if(info.type==MapResourceInfo::IMAGE)
			core->getDisplay()->loadImage(filePath,info.ID);
	}

	void MapStandardParser::unloadResource(MapResourceInfo info)
	{
		if(mapInfo->resources.find(info.ID)==mapInfo->resources.end())
			return;

		if(info.type==MapResourceInfo::AUDIO)
			core->getMusicManager()->unload(info.ID);
		else if(info.type==MapResourceInfo::VIDEO)
			core->getDisplay()->unloadVideo(info.ID);
		else if(info.type==MapResourceInfo::IMAGE)
			core->getDisplay()->unloadImage(info.ID);
	}
}