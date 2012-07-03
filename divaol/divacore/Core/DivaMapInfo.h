/*
 *  DivaMapInfo.h
 *
 *  Created by Hyf042 on 1/13/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_MAP_INFO
#define DIVA_MAP_INFO

#include <vector>
#include <map>
#include "SoraCore.h"
#include "SoraAny.h"
#include "DivaCommon.h"

namespace divacore
{
	/*
	MapHeader
	Some information about map
	*/
	class MapHeader
	{
	public:
		enum DIFFICULITY{EASY,NORMAL,HARD,EXTRA,DIE};
		//version string
		std::string versionStr;
		//music name
		std::string mapName;
		//hard level number
		uint8 hardLevel;
		//difficulty 0-Easy 1-Normal 2-Hard 3-Extra 4-Die
		uint8 difficulty;
		//noters list
		std::vector<std::string> noter;
		//author name
		//std::string authorName;
		//the scale of note's speed
		float speedScale;
		//bar per minute
		double BPM;
		//number of bars
		int32 barNum;
	};

	/*
	MapResourceInfo
	the info of all the resource
	*/
	class MapResourceInfo
	{
	public:
		enum ResourceType{VIDEO,AUDIO,IMAGE};
		std::string filePath;
		std::string ID;
		uint8 type;
		bool flag;
	};

	/*
	MapNote
	info about a note
	argu is the extra arguments about this note
	*/
	class NotePoint
	{
	public:
		uint32 position;
		int32 x;
		int32 y;
		uint8 type;
		std::string key;
		double time;

		NotePoint():position(0),x(0),y(0),key(""),type(0),time(0) {}
	};

	class MapNote
	{
	public:
		typedef std::vector<NotePoint> NOTEPOINT;

		uint32 aheadBar;
		double aheadTime;
		std::string noteType;
		NOTEPOINT notePoint;
		ARGUMENTS arg;
		void *pointer;

		MapNote():aheadBar(1),noteType("normal"),pointer(NULL) {}
	};

	/*
	MapEvent
	a event with position and arguments
	*/
	class MapEvent
	{
	public:
		uint32 position;
		std::string eventType;
		ARGUMENTS arg;
		double time;

		MapEvent():position(0),time(0) {}
	};

	/*
	MapInfo
	collection of all the data
	*/
	class MapInfo
	{
	public:
		typedef std::map<std::string,MapResourceInfo> RESOURCES;
		typedef std::vector<MapEvent> EVENTS;
		typedef std::vector<MapNote> NOTES;

		MapHeader header;
		RESOURCES resources;
		std::vector<MapEvent> events;
		std::vector<MapNote> notes;
		std::string module;
		double totalTime;
	};

	typedef MapInfo* MapInfoPtr;
}

#endif