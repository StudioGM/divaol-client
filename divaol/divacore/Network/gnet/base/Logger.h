/****************************************************
 *  Logger.h
 *
 *  Created by tempbuffer on 7/31/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  Common Logger
 ****************************************************/

#ifndef _BASE_LOGGER_
#define _BASE_LOGGER_

#include "Common.h"
#include "Singleton.h"
#include <iostream>
#include <fstream>
#include <deque>

namespace Base
{
	class Logger : public Singleton<Logger>
	{
	public:
		//some features to describe the log message
		enum LoggerFeature{
			FEATURE_PREPEND_RUNNING_TIME,
			FEATURE_PREPEND_LEVEL_NAME,
			FEATURE_PREPEND_LINE_NUMBER,
			FEATURE_OUTPUT_TO_CONSOLE
		};
		//log level
		enum LogLevel{
			LEVEL_NOTICE,
			LEVEL_ERROR,
			LEVEL_WARNING,
			LEVEL_INFO
		};
	public:
		Logger():
		  mPrependTime(true),mPrependLevel(true),mPrependNumber(true),mOutpoutToConsole(true) {mOutput.open(DEFAULT_GLOBAL_FILE);}
		Logger(const base_string &filename):
		  mPrependTime(true),mPrependLevel(true),mPrependNumber(true),mOutpoutToConsole(true) {mOutput.open(filename);}
		
	    void redirect(const base_string &filename);
		void setFeature(LoggerFeature feature, bool flag);

		void log(const base_string& log, LogLevel=LEVEL_INFO);

		Logger& operator<<(const base_string& log);

		void clear();

		size getLogSize() const;

		const std::deque<base_string>& getLogQueue() const {return mLogQueue;}

		//addtional part of head and tail
		virtual const char* headDecorate() {return "";}
		virtual const char* tailDecorate() {return "";}

		//global logger funcs
		static void log_set_file(const base_string& log) {DEFAULT_GLOBAL_FILE = log;}
		static void log_error(const base_string& log) {instance().log(log,Logger::LEVEL_ERROR);}
		static void log_notice(const base_string& log) {instance().log(log,Logger::LEVEL_NOTICE);}
		static void log_warning(const base_string& log) {instance().log(log,Logger::LEVEL_WARNING);}
		static void log_info(const base_string& log) {instance().log(log,Logger::LEVEL_INFO);}

	private:
		static inline base_string logLevelToString(LogLevel level);

		static base_string DEFAULT_GLOBAL_FILE;

		std::deque<base_string> mLogQueue;

		std::ofstream mOutput;

		bool mPrependTime;
		bool mPrependLevel;
		bool mPrependNumber;
		bool mOutpoutToConsole;
	};

	/*global logger*/
	#define BASE_LOGGER Base::Logger::instance()

	#define BASE_LOGGER_ASSERT(cond, log) \
	if(!(cond)) { \
		BASE_LOGGER.log_error(Base::FormatString("Base::assertion failed with mssg %s at function %s, file %s, line %d", log, __FUNCTION__, __FILE__, __LINE__)); \
		base_assert(0); \
	}
}

#endif