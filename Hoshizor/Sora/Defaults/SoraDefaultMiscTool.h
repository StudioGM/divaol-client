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
        
		StringType fileOpenDialog(const char* filter = NULL, const char* defaultPath = NULL) {
			return "\0";
		}
		
		StringType fileSaveDialog(const char* filter = NULL, const char* defaultPath = NULL, const char* defaultExt = NULL) {
			return "\0";
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
