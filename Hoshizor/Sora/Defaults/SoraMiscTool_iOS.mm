/*
 *  SoraMiscTool_iOS.cpp
 *  Sora-iPhone
 *
 *  Created by Griffin Bu on 4/20/11.
 *  Copyright 2011 Studio GameMaster. All rights reserved.
 *
 */
#include "SoraMiscTool_iOS.h"

#ifdef OS_IOS
#include "SoraLogger.h"

#include <CoreFoundation/CoreFoundation.h>
#include <sys/sysctl.h>

namespace sora {

	// to do, delegate
	int32 SoraMiscTooliOS::messageBox(const StringType& sMessage, const StringType& sTitle, int32 msgCode) {
		NSString* nsMessage = [[NSString alloc] initWithUTF8String:sMessage.c_str()];
		NSString* nsTitle = [[NSString alloc] initWithUTF8String:sTitle.c_str()];
		
		UIAlertView* alert = [[UIAlertView alloc]
							  initWithTitle:nsTitle
							  message:nsMessage
							  delegate:nil 
							  cancelButtonTitle:(msgCode&MB_OK)?@"OK":nil
							  otherButtonTitles:nil];
		[alert show];
		return 1;
	}
    
    StringType SoraMiscTooliOS::fileOpenDialog(const char* filter, const char* defaultPath) {
        return StringType();
    }
    
    StringType SoraMiscTooliOS::fileSaveDialog(const char* filter, const char* defaultPath, const char* defaultExt) {
        return StringType();
    }
    
    uint32 SoraMiscTooliOS::getProcessorSpeed() const {
        return 0;
    }
    
    StringType SoraMiscTooliOS::getOSVersion() const {
        NSString*  systemVersion=[[UIDevice currentDevice] systemVersion];
        NSString*  model=[[UIDevice currentDevice] model];
        
        return vamssg("iOS %s %s", 
                      [systemVersion UTF8String],
                      [model UTF8String]);
    }
    
    uint64 SoraMiscTooliOS::getSystemMemorySize() const {
        int mib[2] = { CTL_HW, HW_MEMSIZE };
        u_int namelen = sizeof(mib) / sizeof(mib[0]);
        uint64_t size = 0;
        size_t len = sizeof(size);
        
        sysctl(mib, namelen, &size, &len, NULL, 0);
        return 0;
    }

} // namespace sora

#endif // OS_IOS