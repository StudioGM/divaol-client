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
#include "Io/Stream.h"
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
		  mPrependTime(true),mPrependLevel(true),mPrependNumber(true),mOutpoutToConsole(true) {}
		Logger(const String &filename):
		  mPrependTime(true),mPrependLevel(true),mPrependNumber(true),mOutpoutToConsole(true) {mOutput = new FileStream(filename,Io_write);}
		Logger(const StreamPtr &stream):
		  mPrependTime(true),mPrependLevel(true),mPrependNumber(true),mOutpoutToConsole(true) {mOutput = stream;}

		static Logger& instance() {static Logger logger(DEFAULT_GLOBAL_FILE); return logger;}

	    void redirect(const StreamPtr &stream);
		void setFeature(LoggerFeature feature, bool flag);

		void log(const String& log, LogLevel=LEVEL_INFO);

		Logger& operator<<(const String& log);

		void clear();

		size getLogSize() const;

		const std::deque<String>& getLogQueue() const {return mLogQueue;}

		//addtional part of head and tail
		virtual const char* headDecorate() {return "";}
		virtual const char* tailDecorate() {return "";}

		//global logger funcs
		static void log_set_file(const String& log) {DEFAULT_GLOBAL_FILE = log;}
		static void log_error(const String& log) {instance().log(log,Logger::LEVEL_ERROR);}
		static void log_notice(const String& log) {instance().log(log,Logger::LEVEL_NOTICE);}
		static void log_warning(const String& log) {instance().log(log,Logger::LEVEL_WARNING);}
		static void log_info(const String& log) {instance().log(log,Logger::LEVEL_INFO);}

	private:
		static inline String logLevelToString(LogLevel level);

		static String DEFAULT_GLOBAL_FILE;

		std::deque<String> mLogQueue;

		StreamPtr mOutput;

		bool mPrependTime;
		bool mPrependLevel;
		bool mPrependNumber;
		bool mOutpoutToConsole;
	};

	/*global logger*/
	#define BASE_LOGGER Base::Logger::instance()

	#define BASE_LOGGER_ASSERT(cond, log) \
	if(!(cond)) { \
		BASE_LOGGER.log_error(Base::String::format("Base::assertion failed with mssg %s at function %s, file %s, line %d", log, __FUNCTION__, __FILE__, __LINE__)); \
		base_assert(0); \
	}
}

#endif