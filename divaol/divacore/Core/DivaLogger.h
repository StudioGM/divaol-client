/*
 *  DivaLogger.h
 *
 *  Created by Hyf042 on 2/4/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_LOGGER
#define DIVA_LOGGER

#include <fstream>
#include "DivaException.h"

namespace divacore
{
	/*
	 * Logger, do some notes
	 */
	class Logger
	{
		std::ofstream writer;
	public:
		static Logger* instance() {static Logger instance; return &instance;}
		Logger(const std::string& file="diva_log.txt")
		{
			writer.open(file.c_str());
			bar();
			print("Diva Online Logger");
			bar();
		}
		~Logger()
		{
			writer.flush();
			writer.close();
		}
		void bar(int length = 50, char meta = '-')
		{
			for(int i = 0; i < length; i++)
				writer << meta;
			writer << std::endl;
		}
		void space_line()
		{
			writer << std::endl;
		}
		void print(const std::string &inf)
		{
			writer << inf << std::endl;
		}
		void error(Exception &ev)
		{
			space_line();
			writer << "Exception IN " << ev.getModuleName() << " AT " << ev.getLinenum() << std::endl;
			writer << ">>> " << ev.getContent() << std::endl;
		}
		void msg(const std::string &inf, const std::string &module = "Global")
		{
			space_line();
			writer << module << ": " << inf << std::endl;
		}
		void log(const char* format, ...)
		{
			va_list	ArgPtr;
			char Message[1024] = {0};
			va_start(ArgPtr, format);
			vsprintf(Message, format, ArgPtr);
			va_end(ArgPtr);

			space_line();
			print(Message);
		}
		void notice(const std::string &inf)
		{
			msg("Notice "+inf);
		}
	};
#define LOGGER divacore::Logger::instance()
}

#endif