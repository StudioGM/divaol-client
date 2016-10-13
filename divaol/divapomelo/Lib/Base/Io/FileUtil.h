/****************************************************
 *  FileUtil.h
 *
 *  Created by tempbuffer on 8/12/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  A bunch of funs that deal with files
 ****************************************************/

#ifndef _BASE_IO_UTIL_H_
#define _BASE_IO_UTIL_H_

#include "../Common.h"
#include "Stream.h"

namespace Base
{
	class FileUtil
	{
	public:
		static bool FileExist(const String &path);
		static bool PathExist(const String &path);
		static bool IsFile(const String &path);
		static bool IsFolder(const String &path);
		static void* ReadRawData(const String& filepath, size_t &size);
	};
}

#endif