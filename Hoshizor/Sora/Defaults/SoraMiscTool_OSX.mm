/*
 *  SoraMiscTool_OSX.mm
 *  Sora
 *
 *  Created by Griffin Bu on 5/31/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */


#include "SoraMiscTool_OSX.h"

#ifdef OS_OSX

#import <Cocoa/Cocoa.h>	
#import <Foundation/Foundation.h>

#include <sys/sysctl.h>

#include "SoraStringTokenlizer.h"
#include "SoraLogger.h"

namespace sora {
	
	inline int32 SoraMiscToolOSX::msgCodeTokCFNotificationLevel(int32 msgCode) {
		// to do
		if(msgCode & MB_ICONERROR)
			return kCFUserNotificationCautionAlertLevel;
		else if(msgCode & MB_ICONSTOP)
			return kCFUserNotificationStopAlertLevel;
		else if(msgCode & MB_ICONINFORMATION) 
			return kCFUserNotificationNoteAlertLevel;
		else if(msgCode & MB_ICONWARNING)
			return kCFUserNotificationPlainAlertLevel;
		
		return kCFUserNotificationPlainAlertLevel;
	}
	
	int32 SoraMiscToolOSX::messageBox(const StringType& sMessage, const StringType& sTitle, int32 msgCode) {
		CFStringRef header_ref   = CFStringCreateWithCString(NULL, sTitle.c_str(), sTitle.size());
		CFStringRef message_ref  = CFStringCreateWithCString(NULL, sMessage.c_str(), sMessage.size());
		
		CFOptionFlags result;
		int32 notificate = msgCodeTokCFNotificationLevel(msgCode);
		CFUserNotificationDisplayAlert(0,
									   notificate,
									   NULL,
									   NULL,
									   NULL,
									   header_ref,
									   message_ref,
									   NULL, // default "ok"
									   (msgCode&MB_OKCANCEL)?CFSTR("Cancel"):NULL, // alternate
									   NULL, //other button title
									   &result
									   );
		
		CFRelease( header_ref );
		CFRelease( message_ref );
		
		switch(result) {
			case kCFUserNotificationDefaultResponse: return IDOK;
			case kCFUserNotificationCancelResponse: return IDCANCEL;
			case kCFUserNotificationAlternateResponse: return IDCANCEL;
			case kCFUserNotificationOtherResponse: return IDABORT;
		}
		return 0;
	}

	StringType SoraMiscToolOSX::fileOpenDialog(const char* filter, const char* defaultPath) {
		NSOpenPanel* openDlg = [NSOpenPanel openPanel];
		
		[openDlg setAllowsMultipleSelection:NO];
		
		NSMutableArray* fileTypes = nil;
		if(filter != NULL) {
			fileTypes = [NSMutableArray arrayWithObject:@"*"];
			
            SoraStringTokenlizer tokens(filter);
            SoraStringTokenlizer::iterator it = tokens.begin();
            while(it != tokens.end()) {
                [fileTypes addObject: [[NSString alloc] initWithUTF8String: (*it).c_str()]];
                ++it;
			}
		}
		
		NSString* filePath;
		if(defaultPath)
			filePath = [[NSString alloc] initWithUTF8String: defaultPath];
		else
			filePath = [[NSString alloc] initWithUTF8String: "~/"];
		
		SoraHandle result = [openDlg runModalForDirectory:filePath
                                                       file:nil 
                                                       types:fileTypes];
		
		if (result == NSOKButton) {
            [filePath release];
            
			NSArray *filesToOpen = [openDlg filenames];
			SoraHandle count = [filesToOpen count];
			if(count > 0) {
				std::string fileName = [[filesToOpen objectAtIndex:0] UTF8String];
				return fileName;
			}
		}
		return "\0";			
	}
	
	StringType SoraMiscToolOSX::fileSaveDialog(const char* filter, const char* defaultPath, const char* defaultExt) {
		NSSavePanel* saveDlg = [NSSavePanel savePanel];
		
        NSString* fileExt = nil;
		if(defaultExt != NULL) {
			NSString* fileExt = [[NSString alloc] initWithUTF8String: defaultExt];
			[saveDlg setRequiredFileType:fileExt];
		}
		
		NSString* filePath;
		if(defaultPath != NULL)
			filePath= [[NSString alloc] initWithUTF8String: defaultPath];
		else
			filePath = [[NSString alloc] initWithUTF8String: "~/"];
		
		int result = [saveDlg runModalForDirectory:filePath
                                                   file:nil];
		
		if (result == NSOKButton) {
            [filePath release];
            [fileExt release];
            
			std::string fileName = [[saveDlg filename] UTF8String];
			return fileName;
		}
        [filePath release];
        [fileExt release];
        
		return "\0";
	}
    
    uint32 SoraMiscToolOSX::getProcessorSpeed() const {
        int mib[2] = { CTL_HW, HW_CPU_FREQ };
        u_int namelen = sizeof(mib) / sizeof(mib[0]);
        uint64_t freq = 0;
        size_t len = sizeof(freq);
        
        sysctl(mib, namelen, &freq, &len, NULL, 0);
        return freq / 1000000;
        
    }

    StringType SoraMiscToolOSX::getOSVersion() const {
        SInt32 versionMajor = 0;
        SInt32 versionMinor = 0;
        Gestalt( gestaltSystemVersionMajor, &versionMajor );
        Gestalt( gestaltSystemVersionMinor, &versionMinor );
        return vamssg("Mac OS X Version %d.%d", versionMajor, versionMinor);
    }
    
    uint64 SoraMiscToolOSX::getSystemMemorySize() const {
        int mib[2] = { CTL_HW, HW_MEMSIZE };
        u_int namelen = sizeof(mib) / sizeof(mib[0]);
        uint64_t size = 0;
        size_t len = sizeof(size);
        
        sysctl(mib, namelen, &size, &len, NULL, 0);
        return size;
    }
}

#endif // OS_OSX