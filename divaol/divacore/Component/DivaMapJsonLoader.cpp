/*
 *  DivaMapJsonLoader.cpp
 *
 *  Created by Hyf042 on 1/13/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#include "DivaMapJsonLoader.h"
#include "SoraResourceFile.h"
#include "Core/DivaDefault.h"
#include "Lib/Base/Io/FileUtility.h"

namespace divacore
{
	using namespace sora;

	void MapJsonLoader::gameReset()
	{
		LOGGER->msg("Set","MapJsonLoader");
		songFolder = sora::SoraCore::Ptr->loadResourcePack(getSongPath());
	}
	void MapJsonLoader::gameStop()
	{
		sora::SoraCore::Ptr->detachResourcePack(songFolder);
	}
	MapInfoPtr MapJsonLoader::load()
	{
		SAFE_DELETE(mapInfo);
		mapInfo = new MapInfo();

		size_t size;
		//SoraResourceFile data(getSongPath()+L"/"+getMapFileName());
		
        Json::Reader reader;
        Json::Value root;
        
        const char* cdata = (const char*)Base::FileUtility::readRawData(getSongPath()+L"/"+getMapFileName(),size);
		const char* rdata = cdata;
		//check bom
		if(size>=3&&uint8(cdata[0])==0xef&&uint8(cdata[1])==0xbb&&uint8(cdata[2])==0xbf)
			rdata = cdata+3, size -= 3;

		if(reader.parse(rdata, rdata+size, root))
		{
			_parserHeader(root);
			_parserResource(root);
			_parserNotes(root);
			_parserEvents(root);
			if (root.isMember("module"))
				mapInfo->module = root["module"].asString();
			else
				mapInfo->module = DEFAULT_INFO_PTR->getAsString("module");
		}
		else
			DIVA_EXCEPTION_MODULE(reader.getFormatedErrorMessages(),"MapLoader");

		if(cdata)
			free((void*)cdata);

		return mapInfo;
	}

	void MapJsonLoader::_parserHeader(const Json::Value& val)
	{
		if(val.isMember("header")&&val["header"].isObject())
		{
		    Json::Value header = val["header"];
			mapInfo->header.versionStr = JsonHelper::_loadAsString(header,"version");
			mapInfo->header.mapName = JsonHelper::_loadAsWString(header,"name");
			mapInfo->header.mainSound = JsonHelper::_loadAsString(header,"mainSound");
			mapInfo->header.hardLevel = JsonHelper::_loadAsInt(header,"hard");
			if(header.isMember("speedScale"))
				mapInfo->header.speedScale = JsonHelper::_loadAsDouble(header,"speedScale");
			else
				mapInfo->header.speedScale = 1;
			if(header.isMember("difficulty")&&header["difficulty"].isInt())
				mapInfo->header.difficulty = header["difficulty"].asInt();
			else
				mapInfo->header.difficulty = _parserDifficulty(JsonHelper::_loadAsString(header,"difficulty"));
			if(header.isMember("noter"))
			{
				Json::Value noter = header["noter"];
				if(noter.isString())
					mapInfo->header.noter.push_back(sora::s2ws(header["noter"].asString()));
				else if(noter.isArray())
					for(int i=0; i<noter.size(); i++)
						mapInfo->header.noter.push_back(sora::s2ws(noter[i]["name"].asString()));
				else
					DIVA_EXCEPTION_MODULE("'noter' must be string(s)!","MapLoader");
			}
			else
				DIVA_EXCEPTION_MODULE("missing 'noter'!","MapLoader");
			mapInfo->header.BPM = JsonHelper::_loadAsDouble(header,"bpm");
			//mapInfo->header.barNum = JsonHelper::_loadAsInt(header,"bar");
		}
		else
			DIVA_EXCEPTION_MODULE("missing 'header'!","MapLoader");
	}
	void MapJsonLoader::_parserResource(const Json::Value& val)
	{
		if(val.isMember("resource")&&val["resource"].isArray())
		{
			Json::Value resource = val["resource"];
			for(int i = 0; i < resource.size(); i++)
				if(resource[i].isObject())
				{
					MapResourceInfo resourceInfo;
					resourceInfo.ID = JsonHelper::_loadAsString(resource[i],"id");
					resourceInfo.filePath = JsonHelper::_loadAsWString(resource[i],"file");
					resourceInfo.type = _parserResourceType(JsonHelper::_loadAsString(resource[i],"type"));
					if(resourceInfo.type==MapResourceInfo::AUDIO)
					{
						if(resource[i].isMember("stream"))
							resourceInfo.flag = JsonHelper::_loadAsBool(resource[i],"stream");
						else
							resourceInfo.flag = false;
					}
					mapInfo->resources[resourceInfo.ID] = resourceInfo;
				}
				else
					DIVA_EXCEPTION_MODULE("items in 'resource' must be object!","MapLoader");
		}
		else
			DIVA_EXCEPTION_MODULE("missing 'resource'!","MapLoader");

		//check main sound
		MapInfo::RESOURCES::iterator mainSoundPtr = mapInfo->resources.find(mapInfo->header.mainSound);
		if(mainSoundPtr==mapInfo->resources.end())
			DIVA_EXCEPTION_MODULE("no main sound file!","MapLoader");
		if(mainSoundPtr->second.type!=MapResourceInfo::AUDIO)
			DIVA_EXCEPTION_MODULE("main sound file is not audio resource!","MapLoader");
	}
	NotePoint MapJsonLoader::_parserNote(const Json::Value& val)
	{
		NotePoint point;
		if(val.isInt())
			point.position = val.asInt();
		else if(val.isObject())
		{
			point.position = JsonHelper::_loadAsInt(val,"position");
			if(val.isMember("x"))
				point.x =  JsonHelper::_loadAsInt(val,"x");
			if(val.isMember("y"))
				point.y =  JsonHelper::_loadAsInt(val,"y");
			if(val.isMember("type"))
				point.type =  JsonHelper::_loadAsInt(val,"type");
			if(val.isMember("key"))
				point.key =  JsonHelper::_loadAsString(val,"key");
		}
		else
			DIVA_EXCEPTION_MODULE("'notePoint 'type error!","MapLoader");
		return point;
	}
	void MapJsonLoader::_parserNotes(const Json::Value& val)
	{
		if(val.isMember("note")&&val["note"].isArray())
		{
			Json::Value notes = val["note"];
			for(int i = 0; i < notes.size(); i++)
				if(notes[i].isObject())
				{
					Json::Value item = notes[i];
					MapNote note;
					note.noteType = JsonHelper::_loadAsString(item,"type");
					if(item.isMember("aheadBar"))
						note.aheadBar = JsonHelper::_loadAsInt(item,"aheadBar");
					if(item.isMember("notePoint"))
					{
						if(item["notePoint"].isInt()||item["notePoint"].isObject())
							note.notePoint.push_back(_parserNote(item["notePoint"]));
						else if(item["notePoint"].isArray())
							for(int j = 0; j < item["notePoint"].size(); j++)
								note.notePoint.push_back(_parserNote(item["notePoint"][j]));
					}
					else
						DIVA_EXCEPTION_MODULE("missing 'notePoint'!","MapLoader");
					Json::Value::Members members = item.getMemberNames();
					for(Json::Value::Members::iterator ptr = members.begin(); ptr != members.end(); ptr++)
						if((*ptr) != "type" && (*ptr) != "notePoint" && (*ptr) != "aheadBar")
							if(item[*ptr].isInt())
								note.arg[*ptr] = item[*ptr].asInt();
							else if(item[*ptr].isString())
								note.arg[*ptr] = item[*ptr].asString();
							else if(item[*ptr].isDouble())
								note.arg[*ptr] = item[*ptr].asDouble();
							else if(item[*ptr].isBool())
								note.arg[*ptr] = item[*ptr].asBool();
					mapInfo->notes.push_back(note);
				}
				else
					DIVA_EXCEPTION_MODULE("items in 'note' must be object!","MapLoader");
		}
		else
			DIVA_EXCEPTION_MODULE("missing 'note'!","MapLoader");
	}
	void MapJsonLoader::_parserEvents(const Json::Value& val)
	{
		if(val.isMember("event")&&val["event"].isArray())
		{
			Json::Value events = val["event"];
			for(int i = 0; i < events.size(); i++)
				if(events[i].isObject())
				{
					Json::Value item = events[i];
					MapEvent event;
					event.eventType = JsonHelper::_loadAsString(item,"type");
					event.position = JsonHelper::_loadAsInt(item,"position");
					Json::Value::Members members = item.getMemberNames();
					for(Json::Value::Members::iterator ptr = members.begin(); ptr != members.end(); ptr++)
						if((*ptr) != "type" && (*ptr) != "position")
							if(item[*ptr].isInt())
								event.arg[*ptr] = item[*ptr].asInt();
							else if(item[*ptr].isString())
								event.arg[*ptr] = item[*ptr].asString();
							else if(item[*ptr].isDouble())
								event.arg[*ptr] = item[*ptr].asDouble();
					mapInfo->events.push_back(event);
				}
				else
					DIVA_EXCEPTION_MODULE("items in 'event' must be object!","MapLoader");
		}
		else
			DIVA_EXCEPTION_MODULE("missing 'event'!","MapLoader");
	}
}