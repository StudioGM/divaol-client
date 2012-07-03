/*
 *  SoraMiscTool_iOS.h
 *  Sora-iPhone
 *
 *  Created by Griffin Bu on 4/20/11.
 *  Copyright 2011 Studio GameMaster. All rights reserved.
 *
 */

#ifndef SORA_MISCTOOL_IOS_H_
#define SORA_MISCTOOL_IOS_H_

#include "SoraStringConv.h"
#include "SoraPlatform.h"

#include "SoraDefaultMiscTool.h"

#ifdef OS_IOS

namespace sora {
	
	class SoraMiscTooliOS: public SoraDefaultMiscTool {
	public:		
		int32 messageBox(const StringType& sMessage, const StringType& sTitle, int32 msgCode);
		
		StringType fileOpenDialog(const char* filter = NULL, const char* defaultPath = NULL);
		
		StringType fileSaveDialog(const char* filter = NULL, const char* defaultPath = NULL, const char* defaultExt = NULL);
        
        uint32 getProcessorSpeed() const;
        StringType getOSVersion() const;
        uint64 getSystemMemorySize() const;

	};
	
} // namespace sora

#endif // OS_OSX

#endif