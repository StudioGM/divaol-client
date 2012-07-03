/*
 *  SoraInternalLogger.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/28/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */


#include "SoraInternalLogger.h"

#include "SoraEventManager.h"
#include "SoraEvent.h"
#include "SoraCore.h"

#ifdef OS_PSP
#include <pspkernel.h>
#include <pspdebug.h>
#endif

namespace sora {
    
    SoraInternalLogger* SoraInternalLogger::mInstance = NULL;
    
    SoraInternalLogger* SoraInternalLogger::Instance() {
        if(!mInstance)
            mInstance = new SoraInternalLogger;
        return mInstance;
    }
    
    void SoraInternalLogger::Destroy() {
        if(mInstance) {
            delete mInstance;
            mInstance = NULL;
        }
    }
    
    SoraInternalLogger::LogMssg::LogMssg(const std::string& log, int32 level) {
        
        mLog = vamssg("[%.4f]", ((float)(SoraCore::GetRunningTime()) / 1000000)) + log;
        mLogLevel = level;
    }
	
	SoraInternalLogger::SoraInternalLogger() {
#ifdef OS_PSP
	//pspDebugScreenInit();
#endif
	}
	
	SoraInternalLogger::~SoraInternalLogger() {
	}
	
	SoraInternalLogger& SoraInternalLogger::operator<<(const StringType& mssg) {
		vMssg.push_back(LogMssg(mssg));
		return *this;
	}

	void SoraInternalLogger::printf(const char* format, ...) {
		va_list	ArgPtr;
		char Message[1024] = {0};
		va_start(ArgPtr, format);
		vsprintf(Message, format, ArgPtr);
		va_end(ArgPtr);
		
	#ifndef OS_PSP
		::printf("%s", Message);
	#else
//		pspDebugScreenPrintf(Message);
	#endif
	}
	
	void SoraInternalLogger::printf(const wchar_t* format, ...) {
		va_list	ArgPtr;
#ifdef HAS_WSTRING
		wchar_t Message[1024] = {0};
		va_start(ArgPtr, format);
		vswprintf(Message, 1024, format, ArgPtr);
		
		va_end(ArgPtr);
		::wprintf(L"%s\n", Message);
#else
		char Message[1024] = {0};
		va_start(ArgPtr, format);
		vsprintf(Message, ws2sfast(format).c_str(), ArgPtr);
		
		va_end(ArgPtr);
	#ifndef OS_PSP
		::printf("%s\n", Message);
	#else
//		pspDebugScreenPrintf(Message);
	#endif
#endif
	}
	
	void SoraInternalLogger::log(const StringType& log, int32 logLevel) {
		vMssg.push_back(LogMssg(log, logLevel));
#ifdef OS_IOS
        ::printf("%s\n", log.c_str());
#endif
	}
	
	void SoraInternalLogger::writeToFile(const char* fileName) {
		std::ofstream file(fileName);
		
		if(file.is_open()) {
            file << vamssg("Hoshizora Ver %d.%d Rev %d]n\n", SORA_VERSION_MAJOR, SORA_VERSION_MINOR, SORA_VERSION_REV);
			std::vector<LogMssg>::iterator itLog = vMssg.begin();
			while(itLog != vMssg.end()) {
				switch(itLog->mLogLevel) {
					case LOG_LEVEL_ERROR: file<<"** error ";  break;
				}
				file<<itLog->mLog<<std::endl;
				++itLog;
			}
		}
		
		file.close();
	}
    
    void SoraInternalLogger::normal(const StringType& mssg) {
        log(mssg, LOG_LEVEL_NORMAL);
    }
    
    void SoraInternalLogger::error(const StringType& mssg) {
        log(mssg, LOG_LEVEL_ERROR);
    }
    
    void SoraInternalLogger::warning(const StringType& mssg) {
        log(mssg, LOG_LEVEL_WARNING);
    }
    
    void SoraInternalLogger::notice(const StringType& mssg) {
        log(mssg, LOG_LEVEL_NOTICE);
    }
    
	size_t SoraInternalLogger::logSize() const {
		return vMssg.size();
	}
	
	const std::vector<SoraInternalLogger::LogMssg>& SoraInternalLogger::get() const { 
		return vMssg; 
	}
	
} // namespace sora
