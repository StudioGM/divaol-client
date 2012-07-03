/*
 *  SoraMiscTool_OSX.h
 *  Sora
 *
 *  Created by GriffinBu on 1/23/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_MISCTOOL_OSX
#define SORA_MISCTOOL_OSX

#include "SoraPlatform.h"
#include "SoraStringConv.h"

#include "SoraDefaultMiscTool.h"

#ifdef OS_OSX

namespace sora {
	
	class SoraMiscToolOSX: public SoraDefaultMiscTool {
	public:		
		inline int32 msgCodeTokCFNotificationLevel(int32 msgCode);
		
		int32 messageBox(const StringType& sMessage, const StringType& sTitle, int32 msgCode);
        
		StringType fileOpenDialog(const char* filter = NULL, const char* defaultPath = NULL);
		StringType fileSaveDialog(const char* filter = NULL, const char* defaultPath = NULL, const char* defaultExt = NULL);
        
        uint32 getProcessorSpeed() const;
        StringType getOSVersion() const;
        uint64 getSystemMemorySize() const;
	};
	
} // namespace sora

#endif // OS_OSX

#endif // SORA_MISCTOOL_WIN32