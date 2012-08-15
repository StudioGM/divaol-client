/****************************************************
 *  FileUtilityWin
 *
 *  Created by tempbuffer on 8/12/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  A bunch of funs that deal with files
 ****************************************************/

#if !defined(_FILE_UTILITY_IMPL_H_) && defined(BASE_OS_WINDOWS)
#define _FILE_UTILITY_IMPL_H_

#include "../Common.h"

#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

namespace Base
{
	class FileUtilityImpl
	{
	public:
		static bool exist(const String& filepath) {
			return PathFileExistsW(filepath.unicode_str())?true:false;
		}
	};
}

#endif