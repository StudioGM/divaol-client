#ifndef SORA_CORE_LOGGER_H
#define SORA_CORE_LOGGER_H

#include "SoraPlatform.h"
#include "SoraSingleton.h"
#include "SoraStringConv.h"
#include "SoraString.h"

#include <vector>
#include <fstream>

namespace sora {
	//namespace sorainternal {
	class SoraCore;
	
	
    enum {
		LOG_LEVEL_NORMAL = 1,
		LOG_LEVEL_ERROR = 2, 
		LOG_LEVEL_WARNING = 3,
		LOG_LEVEL_NOTICE = 4,
	};

	class SORA_API SoraInternalLogger {
		friend class SoraCore;
		
	protected:
		SoraInternalLogger();
		~SoraInternalLogger();
		
	public:
        static SoraInternalLogger* Instance();
        static void Destroy();
        
        struct LogMssg {
			SoraString mLog;
			int32 mLogLevel;
			
			LogMssg(): mLogLevel(LOG_LEVEL_NORMAL) {}
			LogMssg(const std::string& log, int32 level=LOG_LEVEL_NORMAL);
        };
		
		SoraInternalLogger& operator<<(const StringType& mssg);
		
		static void printf(const char* format, ...);
		static void printf(const wchar_t* format, ...);
		
		void log(const StringType& log, int32 logLevel=LOG_LEVEL_NORMAL);
        
        void normal(const StringType& log);
        void error(const StringType& log);
        void warning(const StringType& log);
        void notice(const StringType& lob);
        
		void writeToFile(const char* fileName);
		
		const std::vector<LogMssg>& get() const;
		
		void clearLogs();
		
		size_t logSize() const;
		
	private:
        static SoraInternalLogger* mInstance;
        
		std::vector<LogMssg> vMssg;
	};
	
	static std::wstring vamssgw(const wchar_t* format, ...) {
		va_list	ArgPtr;
#ifdef HAS_WSTRING
		wchar_t Message[1024] = {0};
		va_start(ArgPtr, format);
		vswprintf(Message, 1024, format, ArgPtr);
		va_end(ArgPtr);
		return Message;

#else
		char Message[1024] = {0};
		va_start(ArgPtr, format);
		vsprintf(Message, ws2sfast(format).c_str(), ArgPtr);
		va_end(ArgPtr);
		return s2wsfast(Message);

#endif
	}
	
	static std::string vamssg(const char* format, ...) {
		va_list	ArgPtr;
		char Message[512] = {0};
		va_start(ArgPtr, format);
		vsprintf(Message, format, ArgPtr);
		va_end(ArgPtr);
		return Message;
	}
	
    SORA_STATIC_RUN_CODE_I(sora_start_logger, SoraInternalLogger::Instance());
	static SoraInternalLogger& Logger = *SoraInternalLogger::Instance();
    
    inline void log_mssg(const StringType& log, int32 level=LOG_LEVEL_NORMAL) {
        SoraInternalLogger::Instance()->log(log, level);
    }
    
    inline void log_error(const StringType& log) {
        SoraInternalLogger::Instance()->error(log);
    }
    
    inline void log_warning(const StringType& log) {
        SoraInternalLogger::Instance()->warning(log);
    }
    
    inline void log_notice(const StringType& log) {
        SoraInternalLogger::Instance()->notice(log);
    }
    
    inline void log_normal(const StringType& log) {
        SoraInternalLogger::Instance()->normal(log);
    }
	
} // namespace sora

#endif