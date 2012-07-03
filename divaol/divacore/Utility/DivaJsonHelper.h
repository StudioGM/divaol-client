/*
 *  DivaJsonHelper.h
 *
 *  Created by Hyf042 on 2/7/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */
#ifndef DIVA_JSON_HELPER
#define DIVA_JSON_HELPER

#include "json/json.h"

namespace divacore
{
	class JsonHelper
	{
	public:
		static const std::string _loadAsString(const Json::Value& val, const std::string &name)
		{
			if(val.isMember(name))
			{
				if(val[name].isString())
					return val[name].asString();
				else
					DIVA_EXCEPTION_MODULE("'"+name+"' type error!","MapLoader");
			}
			else
				DIVA_EXCEPTION_MODULE("missing '"+name+"'!","MapLoader");
		}
		static int32 _loadAsInt(const Json::Value& val, const std::string &name)
		{
			if(val.isMember(name))
			{
				if(val[name].isInt())
					return val[name].asInt();
				else
					DIVA_EXCEPTION_MODULE("'"+name+"' type error!","MapLoader");
			}
			else
				DIVA_EXCEPTION_MODULE("missing '"+name+"'!","MapLoader");
		}
		static double _loadAsDouble(const Json::Value& val, const std::string &name)
		{
			if(val.isMember(name))
			{
				if(val[name].isDouble())
					return val[name].asDouble();
				else if(val[name].isInt())
					return val[name].asInt();
				else
					DIVA_EXCEPTION_MODULE("'"+name+"' type error!","MapLoader");
			}
			else
				DIVA_EXCEPTION_MODULE("missing '"+name+"'!","MapLoader");
		}
		static bool _loadAsBool(const Json::Value& val, const std::string &name)
		{
			if(val.isMember(name))
			{
				if(val[name].isBool())
					return val[name].asBool();
				else
					DIVA_EXCEPTION_MODULE("'"+name+"' type error!","MapLoader");
			}
			else
				DIVA_EXCEPTION_MODULE("missing '"+name+"'!","MapLoader");
		}
	};
}

#endif