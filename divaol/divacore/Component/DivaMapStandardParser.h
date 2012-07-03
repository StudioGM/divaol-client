/*
 *  DivaMapStandardParser.h
 *
 *  Created by Hyf042 on 1/13/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_MAP_STANDARD_PARSER
#define DIVA_MAP_STANDARD_PARSER

#include "Core/DivaMapInfo.h"
#include "Core/DivaMapParser.h"

namespace divacore
{
	class ParserNode
	{
	public:
		enum{NOTEPOINT,EVENT,BPM,BAR};
		uint32 type;
		uint32 position;
		double *pRetTime;
		double tValue;

		ParserNode(uint32 type, uint32 position, double *pRetTime, double tValue = 0)
			:type(type),position(position),pRetTime(pRetTime),tValue(tValue) {}

		friend bool operator<(const ParserNode &a, const ParserNode &b)
		{
			return a.position<b.position;
		}
	};

	/*
	MapStandardParser
	calculate the timing and do the preparing work for game starting
	*/
	class MapStandardParser : public MapParser
	{
		MapInfoPtr mapInfo;
		std::vector<double> noteTime;
		std::vector<double> eventTime;
	protected:
		virtual void _parseResource();
		virtual void _parseTime();
		virtual void _parseEvents();
		virtual void _parseNotes();
		virtual void _parseModule();
		void clear();
		std::string getFullPath(const std::string fileName);
	public:
		MapStandardParser():mapInfo(NULL) {}
		virtual void parser(MapInfoPtr mapInfo);
		virtual void gameReset();
		virtual void gameStop();
	};
}

#endif