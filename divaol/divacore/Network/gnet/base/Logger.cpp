/****************************************************
 *  Logger.cpp
 *
 *  Created by tempbuffer on 7/31/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  Common Logger
 ****************************************************/

#include "Logger.h"
#include "Timer.h"

namespace Base
{
	base_string Logger::DEFAULT_GLOBAL_FILE = "base_log.txt";

	inline base_string Logger::logLevelToString(LogLevel level)
	{
		switch(level)
		{
		case LEVEL_ERROR:
			return "*Error*";
		case LEVEL_WARNING:
			return "*Warning*";
		case LEVEL_NOTICE:
			return "*notice*";
		case LEVEL_INFO:
		default:
			return base_string();
		}
	}
	void Logger::redirect(const base_string &filename)
	{
		mOutput.close();
		mOutput.open(filename);
	}
	void Logger::setFeature(LoggerFeature feature, bool flag)
	{
		switch(feature)
		{
		case FEATURE_PREPEND_LEVEL_NAME:
			mPrependLevel = flag;
			break;
		case FEATURE_PREPEND_LINE_NUMBER:
			mPrependNumber = flag;
			break;
		case FEATURE_PREPEND_RUNNING_TIME:
			mPrependTime = flag;
			break;
		case FEATURE_OUTPUT_TO_CONSOLE:
			mOutpoutToConsole = flag;
			break;
		}
	}

	void Logger::log(const base_string& log, LogLevel level)
	{
		base_string msg = headDecorate();

		if(mPrependNumber)
			msg += FormatString("%d. ",mLogQueue.size()+1);
		if(mPrependTime)
			msg += FormatString("[%.3f] ", GlobalTimeStamp::instance().elapsedInSecond());
		if(mPrependLevel)
			msg += logLevelToString(level)+" ";

		msg += log;
		msg += tailDecorate();

		mLogQueue.push_back(msg);

		if(mOutput)
			mOutput << msg << std::endl;
		if(mOutpoutToConsole)
			std::cout << msg << std::endl;
	}

	Logger& Logger::operator<<(const base_string& log)
	{
		this->log(log);
		return *this;
	}

	void Logger::clear()
	{
		mLogQueue.clear();
	}

	size Logger::getLogSize() const
	{
		return mLogQueue.size();
	}
}