/*
 *  SoraMiscTool.h
 *  Hoshizora
 *
 *  Created by Robert Bu on 8/24/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#ifndef SORA_MISC_TOOL_H
#define SORA_MISC_TOOL_H

#include "SoraPlatform.h"
#include "SoraString.h"

namespace sora {
	
	class SORA_API SoraMiscTool {
	public:
		virtual ~SoraMiscTool() {}
		
		virtual int32 messageBox(const StringType& sMessage, const StringType& sTitle, int32 msgCode) = 0;
		
		virtual std::string fileOpenDialog(const char* filter = NULL, const char* defaultPath = NULL) = 0;
		virtual std::wstring fileOpenDialogW(const wchar_t* filter = NULL, const wchar_t* defaultPath = NULL) = 0;
		virtual std::string fileSaveDialog(const char* filter = NULL, const char* defaultPath = NULL, const char* defaultExt = NULL) = 0;
		virtual std::wstring fileSaveDialogW(const wchar_t* filter = NULL, const wchar_t* defaultPath = NULL, const wchar_t* defaultExt = NULL) = 0;
        
        virtual uint32 getProcessorSpeed() const = 0;
        virtual StringType getOSVersion() const = 0;
        virtual uint64 getSystemMemorySize() const = 0;
	};
} // namespace sora

#endif // SORA_MISC_TOOL_H_
