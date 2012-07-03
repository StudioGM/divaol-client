/*
 *  DivaConfig.h
 *
 *  Created by Hyf042 on 1/25/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_CONFIG
#define DIVA_CONFIG

#include "DivaCommon.h"
#include "DivaMapInfo.h"

namespace divacore
{
	using namespace sora;
	/*
	 * Config, manager key and variable, and can use Utility/ConfigLoader to load items from individual file
	 */
	class Config
	{
		ARGUMENTS items;
		sora::SoraAny& get(const std::string &key);
	public:
		void clear();
		inline bool has(const std::string &key) {return items.find(key)!=items.end();}
		void add(const std::string &key, int value) {items[key] = value;}
		void add(const std::string &key, double value) {items[key] = value;}
		void add(const std::string &key, const std::string &value) {items[key] = value;}
		void add(const std::string &key, const char* value) {add(key,std::string(value));}
		void add(const std::string &key, bool value) {items[key] = value;}
		void add(const std::string &key, Rect value) {items[key] = value;}
		void add(const std::string &key, Point value) {items[key] = value;}
		std::string getAsString(const std::string &key);
		int getAsInt(const std::string &key);
		double getAsDouble(const std::string &key);
		bool getAsBool(const std::string &key);
		Rect getAsRect(const std::string &key);
		Point getAsPoint(const std::string &key);
	};

	typedef Config* ConfigPtr;
}

#endif