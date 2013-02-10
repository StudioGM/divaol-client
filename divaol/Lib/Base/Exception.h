/****************************************************
 *  Exception.h
 *
 *  Created by tempbuffer on 7/31/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  Common Exception
 ****************************************************/

#ifndef _BASE_EXCEPTION_
#define _BASE_EXCEPTION_

#include "Type.h"
#include <string>
#include <sstream>
#include <typeinfo>

#define BASE_THROW_EXCEPTION(err) throw Base::Exception(err, __FUNCTION__, __FILE__, __LINE__)

namespace Base
{
	class Exception: public std::exception
	{
	public:
		Exception(const std::string &msg, const char* func, const char* file, int32 line):
		  mMsg(msg.c_str()),mFunc(func),mFile(file),mLine(line) {}

		Exception(const char* msg, const char* func, const char* file, int32 line):
		  mMsg(msg),mFunc(func),mFile(file),mLine(line) {}
		
		Exception(const char* msg):
		  mMsg(msg),mFunc(0),mFile(0),mLine(-1) {}

		virtual ~Exception() throw() {}

		virtual base_string description() const throw() {
			std::ostringstream msg;
			msg << className() << ":\n\tclass=" << name() << "\n\tMessage=" << mMsg;
			if(mFunc != 0) msg << "\n\tFunction=" << mFunc;
			if(mFile != 0) msg << "\n\tFile=" << mFile;
			if(mLine != -1) msg << "\n\tLine=" << mLine;
			
			return msg.str();
		}

		virtual const char* name() const throw() {
			return "Base::Exception";
		}

		virtual const char* className() const throw() {
			return typeid(*this).name();
		}

		const char* msg() const throw() {
			return mMsg;
		}

		const char* func() const throw() {
			return mFunc;
		}

		int32 line() const throw() {
			return mLine;
		}

		virtual void rethrow() {
			throw *this;
		}
	private:
		const char* mMsg;
		const char* mFunc;
		const char* mFile;
		int32 mLine;
	};
}

#endif