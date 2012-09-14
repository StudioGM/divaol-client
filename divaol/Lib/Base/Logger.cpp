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
	String Logger::DEFAULT_GLOBAL_FILE = "base_log.txt";

	inline String Logger::logLevelToString(LogLevel level)
	{
		switch(level)
		{
		case LEVEL_ERROR:
			return L"*Error*";
		case LEVEL_WARNING:
			return L"*Warning*";
		case LEVEL_NOTICE:
			return L"*notice*";
		case LEVEL_INFO:
		default:
			return String();
		}
	}
	void Logger::redirect(const String &filename)
	{
		mOutput.close();
		mOutput.open(filename.asUnicode());
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

	void Logger::log(const String& log, LogLevel level)
	{
		String msg = headDecorate();

		if(mPrependNumber)
			msg += String::format("%d. ",mLogQueue.size()+1);
		if(mPrependTime)
			msg += String::format("[%.3f] ", GlobalTimeStamp::instance().elapsedInSecond());
		if(mPrependLevel)
			msg += logLevelToString(level)+L" ";

		msg += log;
		msg += tailDecorate();

		mLogQueue.push_back(msg);

		if(mOutput)
			mOutput << msg.asUnicode() << std::endl;
		if(mOutpoutToConsole)
			std::cout << msg.asAnsi() << std::endl;
	}

	Logger& Logger::operator<<(const String& log)
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