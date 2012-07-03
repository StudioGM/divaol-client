/*
 *  SoraException.h
 *  Hoshizora
 *
 *  Created by Robert Bu on 8/27/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#ifndef SORA_EXCEPTION_H
#define SORA_EXCEPTION_H

#include "SoraPlatform.h"

#define SORA_EXCEPTION(exp, mess) sora::exp(mess,   \
                                            __FUNCTION__,     \
                                            __FILE__,         \
                                            __LINE__)

#define SORA_EXCEPTION_D(mess) sora::SoraException(mess, \
                                                   __FUNCTION__, \
                                                   __FILE__, \
                                                   __LINE__)

#ifdef SORA_USE_EXCEPTION

#define THROW_SORA_EXCEPTION(exp, mess) throw SORA_EXCEPTION(exp, mess)
#define THROW_SORA_EXCEPTION_D(mess) throw SORA_EXCEPTION_D(mess)

#else

#define THROW_SORA_EXCEPTION(mssg) \
	log_mssg(vamssg("%s, function: %s, file: %s, line: %s", mssg, __FUNCTION__, __FILE__, __LINE__)); 

#endif

#include "SoraStringConv.h"

namespace sora {
    
	class SORA_API SoraException: public std::exception {
	public:
		SoraException(const std::string& mssg, const std::string& function, const std::string& file, int line):
				mMssg(mssg), mFunction(function), mFile(file), mLine(line) {}
		SoraException(const std::wstring& mssg, const std::string& function, const std::string& file, int line):
				mMssg(ws2s(mssg)), mFunction(function), mFile(file), mLine(line) {}
		SoraException(const std::string& mssg):
				mMssg(mssg), mFunction("?"), mFile("?"), mLine(-1) {}
        virtual ~SoraException() throw() {}
		
		// get a formatted exception mssg
		virtual const char* what() const throw();
        // the name of the exception
        virtual const char* name() const throw();
        // the class name of the exception
        virtual const char* className() const throw();
        
		const std::string& mssg() const;
		const std::string& function() const;
		const std::string& file() const;
		int32 line() const;
        
        virtual void rethrow();
        
        /*
         may not available for every platform
         */
        static std::string stackTrace();
		
	private:
		std::string mMssg, mFunction, mFile;
        int mLine;
	};
    
#define SORA_EXCEPTION_DEF(API, CLS, BASE) \
    class API CLS: public BASE { \
    public: \
        CLS(const std::string& mssg, const std::string& function, const std::string& file, int line); \
        CLS(const std::wstring& mssg, const std::string& function, const std::string& file, int line); \
        CLS(const std::string& mssg); \
        virtual ~CLS() throw() {} \
        \
        virtual const char* what() const throw(); \
        virtual const char* name() const throw(); \
        virtual const char* className() const throw(); \
        virtual void rethrow(); \
    };
    
#define SORA_EXCEPTION_IMPL(CLS, BASE, NAME) \
    CLS::CLS(const std::string& mssg, const std::string& function, const std::string& file, int line): \
        BASE(mssg, function, file, line) {} \
    CLS::CLS(const SoraWString& mssg, const std::string& function, const std::string& file, int line): \
        BASE(mssg, function, file, line) {} \
    CLS::CLS(const std::string& mssg): \
        BASE(mssg) {} \
    \
    const char* CLS::what() const throw() { \
        return BASE::what(); \
    } \
    const char* CLS::name() const throw() { \
        return NAME; \
    } \
    const char* CLS::className() const throw() { \
        return typeid(*this).name(); \
    } \
    void CLS::rethrow() { \
        throw *this; \
    }
    
    SORA_EXCEPTION_DEF(SORA_API, LogicException,                SoraException)
    SORA_EXCEPTION_DEF(SORA_API, AssertionViolationException,   LogicException)
    SORA_EXCEPTION_DEF(SORA_API, NullPointerException,          LogicException)
    SORA_EXCEPTION_DEF(SORA_API, NullValueException,            LogicException)
    SORA_EXCEPTION_DEF(SORA_API, BugcheckException,             LogicException)
    SORA_EXCEPTION_DEF(SORA_API, InvalidArgumentException,      LogicException)
    SORA_EXCEPTION_DEF(SORA_API, NotImplementedException,       LogicException)
    SORA_EXCEPTION_DEF(SORA_API, RangeException,                LogicException)
    SORA_EXCEPTION_DEF(SORA_API, IllegalStateException,         LogicException)
    SORA_EXCEPTION_DEF(SORA_API, InvalidAccessException,        LogicException)
    SORA_EXCEPTION_DEF(SORA_API, SignalException,               LogicException)
    SORA_EXCEPTION_DEF(SORA_API, UnhandledException,            LogicException)
    
    SORA_EXCEPTION_DEF(SORA_API, RuntimeException,              SoraException)
    SORA_EXCEPTION_DEF(SORA_API, NotFoundException,             RuntimeException)
    SORA_EXCEPTION_DEF(SORA_API, ExistsException,               RuntimeException)
    SORA_EXCEPTION_DEF(SORA_API, TimeoutException,              RuntimeException)
    SORA_EXCEPTION_DEF(SORA_API, SystemException,               RuntimeException)
    SORA_EXCEPTION_DEF(SORA_API, RegularExpressionException,    RuntimeException)
    SORA_EXCEPTION_DEF(SORA_API, LibraryLoadException,          RuntimeException)
    SORA_EXCEPTION_DEF(SORA_API, LibraryAlreadyLoadedException, RuntimeException)
    SORA_EXCEPTION_DEF(SORA_API, NoThreadAvailableException,    RuntimeException)
    SORA_EXCEPTION_DEF(SORA_API, PropertyNotSupportedException, RuntimeException)
    SORA_EXCEPTION_DEF(SORA_API, PoolOverflowException,         RuntimeException)
    SORA_EXCEPTION_DEF(SORA_API, NoPermissionException,         RuntimeException)
    SORA_EXCEPTION_DEF(SORA_API, OutOfMemoryException,          RuntimeException)
    SORA_EXCEPTION_DEF(SORA_API, DataException,                 RuntimeException)
    
    SORA_EXCEPTION_DEF(SORA_API, DataFormatException,           DataException)
    SORA_EXCEPTION_DEF(SORA_API, SyntaxException,               DataException)
    SORA_EXCEPTION_DEF(SORA_API, CircularReferenceException,    DataException)
    SORA_EXCEPTION_DEF(SORA_API, PathSyntaxException,           SyntaxException)
    SORA_EXCEPTION_DEF(SORA_API, IOException,                   RuntimeException)
    SORA_EXCEPTION_DEF(SORA_API, ProtocolException,             IOException)
    SORA_EXCEPTION_DEF(SORA_API, FileException,                 IOException)
    SORA_EXCEPTION_DEF(SORA_API, FileExistsException,           FileException)
    SORA_EXCEPTION_DEF(SORA_API, FileNotFoundException,         FileException)
    SORA_EXCEPTION_DEF(SORA_API, PathNotFoundException,         FileException)
    SORA_EXCEPTION_DEF(SORA_API, FileReadOnlyException,         FileException)
    SORA_EXCEPTION_DEF(SORA_API, FileAccessDeniedException,     FileException)
    SORA_EXCEPTION_DEF(SORA_API, CreateFileException,           FileException)
    SORA_EXCEPTION_DEF(SORA_API, OpenFileException,             FileException)
    SORA_EXCEPTION_DEF(SORA_API, WriteFileException,            FileException)
    SORA_EXCEPTION_DEF(SORA_API, ReadFileException,             FileException)
    SORA_EXCEPTION_DEF(SORA_API, UnknownURISchemeException,     RuntimeException)
    
    SORA_EXCEPTION_DEF(SORA_API, ApplicationException,  SoraException)
    SORA_EXCEPTION_DEF(SORA_API, BadCastException,      RuntimeException)
    SORA_EXCEPTION_DEF(SORA_API, RttiException,         RuntimeException)

    
} // namespace sora

#endif