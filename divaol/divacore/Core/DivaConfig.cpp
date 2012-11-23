/*
 *  DivaConfig.cpp
 *
 *  Created by Hyf042 on 1/25/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#include "divacore/Core/DivaConfig.h"

namespace divacore
{
	void Config::clear()
	{
		items.clear();
	}
	sora::SoraAny& Config::get(const std::string &key)
	{
		if(items.find(key)==items.end())
			DIVA_EXCEPTION_MODULE("item "+key+" not found!","Config");
		return items[key];
	}
	std::wstring Config::getAsWString(const std::string &key)
	{
		return UTF8toUnicode(sora::AnyCast<std::string>(get(key)));
	}
	std::string Config::getAsString(const std::string &key)
	{
		return sora::AnyCast<std::string>(get(key));
	}
	int Config::getAsInt(const std::string &key)
	{
		return sora::AnyCast<int>(get(key));
	}
	double Config::getAsDouble(const std::string &key)
	{
		sora::SoraAny &any = get(key);
		if(IsAnyInt(any))
			return sora::AnyCast<int>(any);
		return sora::AnyCast<double>(any);
	}
	bool Config::getAsBool(const std::string &key)
	{
		return sora::AnyCast<bool>(get(key));
	}
	Rect Config::getAsRect(const std::string &key)
	{
		return sora::AnyCast<Rect>(get(key));
	}
	Point Config::getAsPoint(const std::string &key)
	{
		return sora::AnyCast<Point>(get(key));
	}
	AnyList Config::getAsList(const std::string &key)
	{
		return sora::AnyCast<AnyList>(get(key));
	}

	typedef Config* ConfigPtr;
}