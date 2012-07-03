/*
 *  SoraOSXFileUtility.cpp
 *  Sora
 *
 *  Created by GriffinBu on 1/12/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "SoraOSXFileUtility.h"
#include "SoraStringConv.h"

#ifdef OS_OSX

#include <Foundation/Foundation.h>

namespace sora {
	
    StringType osxApplicationPath() {
        return [[[NSBundle mainBundle] bundlePath] UTF8String];
    }
    
	bool osxFileExists(const StringType& path) {
        NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
        
		NSString* nsPath = [[NSString alloc] initWithUTF8String:path.c_str()];
		bool result = [[NSFileManager defaultManager] fileExistsAtPath:nsPath];
        [nsPath release];
        
        [pool release];
        return result;
	}
} // namespace sora

#endif