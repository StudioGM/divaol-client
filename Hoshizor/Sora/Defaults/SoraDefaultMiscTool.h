/*
 *  SoraMiscTool.h
 *  Hoshizora
 *
 *  Created by Robert Bu on 8/24/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#ifndef SORA_DEFAULT_MISC_TOOL_H
#define SORA_DEFAULT_MISC_TOOL_H

#include "SoraMiscTool.h"
#include "SoraLogger.h"

namespace sora {
	
	class SoraDefaultMiscTool: public SoraMiscTool {
	public:
		SoraDefaultMiscTool(){}
		virtual ~SoraDefaultMiscTool() { }

		virtual int32 messageBox(const StringType& sMessage, const StringType& sTitle, int32 msgCode) {
			log_mssg(std::string("MessageBox: "+sTitle + ": " + sMessage));
			return 0;
		}
        
		std::string fileOpenDialog(const char* filter = NULL, const char* defaultPath = NULL) {
			return "\0";
		}

		std::wstring fileOpenDialogW(const wchar_t* filter = NULL, const wchar_t* defaultPath = NULL, bool multiFile = false){
			return L"\0";
		}
		
		std::string fileSaveDialog(const char* filter = NULL, const char* defaultPath = NULL, const char* defaultExt = NULL) {
			return "\0";
		}

		std::wstring fileSaveDialogW(const wchar_t* filter = NULL, const wchar_t* defaultPath = NULL, const wchar_t* defaultExt = NULL){
			return L"\0";
		}
		
		uint32 getProcessorSpeed() const {
            return 0;
        }
        
        StringType getOSVersion() const {
            return "null";
        }
        
        uint64 getSystemMemorySize() const {
            return 0;
        }
        
	private:
	};
} // namespace sora

#endif // SORA_DEFAULT_MISC_TOOL_H
