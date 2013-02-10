/****************************************************
 *  FileUtility.h
 *
 *  Created by tempbuffer on 8/12/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  A bunch of funs that deal with files
 ****************************************************/

#ifndef _BASE_FILE_UTILITY_H_
#define _BASE_FILE_UTILITY_H_

#include "../Common.h"

#include "FileUtilityImplWin.h"
#include "Stream.h"

namespace Base
{
	class FileUtility
	{
	public:
		static bool exist(const String& filepath)
		{
#ifdef BASE_OS_WINDOWS
			return FileUtilityImpl::exist(filepath);
#endif
			std::wifstream file;
			file.open(filepath.ansi_str());
			if(!file.is_open()) return false;
			file.close();

			return true;
		}
		static void* readRawData(const String& filepath, size_t &size)
		{
			FileStream file;
			if(!file.open(filepath))
				return 0;
			size = file.size();
			file.seek(0,Seek_begin);

			binary data = reinterpret_cast<binary>(base_malloc(sizeof(byte)*size));
			file.read(data,size);
			return data;
		}
	};
}

#endif