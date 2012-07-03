/*
 *  SoraiOSFileUtility.mm
 *  Sora-iPhone
 *
 *  Created by GriffinBu on 1/28/11.
 *  Copyright 2011 Robert Bu. All rights reserved.
 *
 */


#include "SoraiOSFileUtility.h"
#include "SoraStringConv.h"
#include "Debug/SoraInternalLogger.h"

#ifdef OS_IOS

namespace sora {

	inline std::string NSString2String(NSString* nss) {
		std::string buffer([nss UTF8String]);
		return buffer;
	}
	
	inline NSString* string2NSString(const std::string& str) {
		NSString* buffer = [[NSString alloc] initWithUTF8String:str.c_str()];
		return buffer;
	}
	
	inline NSString* string2NSString(const char* str) {
		NSString* buffer = [[NSString alloc] initWithUTF8String:str];
		return buffer;
	}	
	
	StringType appResourcePath() {
		StringType path(appApplicationPath());
		path += "/";
		return path;
	}
	
	StringType appApplicationPath() {
		return [[[NSBundle mainBundle] bundlePath] UTF8String];
	}
	
	StringType appDocumentPath() {
		NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
		NSString *basePath = ([paths count] > 0) ? [paths objectAtIndex:0] : nil;
		return s2ws(NSString2String(basePath)+"/");
	}
	
	bool appFileExists(const StringType& path) {
		NSString* nsPath = [[NSString alloc] initWithUTF8String:path.c_str()];
		return [[NSFileManager defaultManager] fileExistsAtPath:nsPath];
	}
	
} // namespace sora

#endif // OS_IOS