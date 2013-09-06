/****************************************************
 *  FileUtil.cpp
 *
 *  Created by tempbuffer on 8/12/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  A bunch of funs that deal with files
 ****************************************************/

#include "FileUtil.h"

#include <fstream>

namespace Base
{
	bool FileUtil::FileExist(const String &path)
	{
#ifndef BASE_UNSUPPORTED_SYS
		return FileSystemUtilImpl::FileExist(path);
#endif
		std::wifstream file;
		file.open(path.unicode_str());
		if(!file.is_open()) return false;
		file.close();

		return true;
	}
	bool FileUtil::PathExist(const String &path)
	{
#ifndef BASE_UNSUPPORTED_SYS
		return FileSystemUtilImpl::PathExist(path);
#endif
		std::wifstream file;
		file.open(path.unicode_str());
		if(!file.is_open()) return false;
		file.close();

		return true;
	}
	bool FileUtil::IsFile(const String &path) {
		DWORD result = GetFileAttributesW(path.unicode_str());
		if(result==0xFFFFFFFF)
			return false;
		return (result|FILE_ATTRIBUTE_DIRECTORY)==0;
	}
	bool FileUtil::IsFolder(const String &path) {
		DWORD result = GetFileAttributesW(path.unicode_str());
		if(result==0xFFFFFFFF)
			return false;
		return (result|FILE_ATTRIBUTE_DIRECTORY)>0;
	}
	void* FileUtil::ReadRawData(const String& filepath, size_t &size)
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
}