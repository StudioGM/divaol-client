/*
 *  DivaConfigLoader.cpp
 *
 *  Created by Hyf042 on 1/25/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */
#include "Core/DivaConfig.h"
#include "json/json.h"
#include "SoraResourceFile.h"

namespace divacore
{
	namespace configloader
	{
		void _addWithJson(Config &config, std::string key, Json::Value &value)
		{
			if(value.isObject()&&value.isMember("_array"))
			{
				Json::Value array = value["_array"];
				AnyList anyList;
				for(int i = 0; i < array.size(); i++)
				{
					sora::SoraAny tmp;
					if(array[i].isInt())
						tmp = array[i].asInt();
					else if(array[i].isDouble())
						tmp = array[i].asDouble();
					else if(array[i].isString())
						tmp = array[i].asString();
					else if(array[i].isBool())
						tmp = array[i].asBool();
					else
						DIVA_EXCEPTION_MODULE("array type is not supported","ConfigJsonLoader");

					anyList.push_back(tmp);
				}
				config.add(key,anyList);
			}
			else if(value.isObject())
			{
				config.add(key,0);
				Json::Value::Members members = value.getMemberNames();
				for(Json::Value::Members::iterator ptr = members.begin(); ptr != members.end(); ptr++)
					if(key=="")
						_addWithJson(config,(*ptr),value[*ptr]);
					else
						_addWithJson(config,key+"_"+(*ptr),value[*ptr]);
			}
			else if(value.isInt())
				config.add(key,value.asInt());
			else if(value.isDouble())
				config.add(key,value.asDouble());
			else if(value.isString())
				config.add(key,value.asString());
			else if(value.isBool())
				config.add(key,value.asBool());
			else if(value.isArray()&&(value.size()==4||value.size()==2))
			{
				float item[4];
				for(int i = 0; i < value.size(); i++)
					if(value[i].isInt())
						item[i] = value[i].asInt();
					else if(value[i].isDouble())
						item[i] = value[i].asDouble();
					else
						DIVA_EXCEPTION_MODULE("item "+key+" can't load","ConfigJsonLoader");
				if(value.size()==4)
					config.add(key,Rect(item[0],item[1],item[2],item[3]));
				else
					config.add(key,Point(item[0],item[1]));
			}
			else
				DIVA_EXCEPTION_MODULE("item "+key+" can't load","ConfigJsonLoader");
		}
		void loadWithJson(Config &config, const std::string &file, bool ignore)
		{
			SoraResourceFile data(file);

			Json::Reader reader;
			Json::Value root;

			const char* cdata = (const char*)data;

			if(reader.parse(cdata, cdata+data.size(), root))
				_addWithJson(config,"",root);
			else if(!ignore)
				DIVA_EXCEPTION_MODULE(reader.getFormatedErrorMessages(),"JsonConfig - "+file);
		}
	}
}
