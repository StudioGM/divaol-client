#include "SoraException.h"
#include "SoraLogger.h"

#ifdef __GNUC__
#include <execinfo.h>
#endif

#ifdef OS_WIN32
#include <Windows.h>
#include <dbghelp.h>

#pragma comment(lib, "dbghelp.lib")
#endif

namespace sora {
    #ifdef OS_WIN32
    
#define INVALID_FP_RET_ADDR_VALUE 0x0000000
    
    inline std::string DisplaySymbolDetails(DWORD dwAddress) {
        DWORD64 displacement = 0;
        ULONG64 buffer[(sizeof(SYMBOL_INFO) + MAX_SYM_NAME*sizeof(TCHAR) + sizeof(ULONG64) - 1) / sizeof(ULONG64)];
        
        PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;
        pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        pSymbol->MaxNameLen = MAX_SYM_NAME;
        
        std::string info;
        
        if (SymFromAddr(GetCurrentProcess(), dwAddress, &displacement, pSymbol)) {
			IMAGEHLP_MODULE64 moduleinfo;
			moduleinfo.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
			if(SymGetModuleInfo64(GetCurrentProcess(), pSymbol->Address, &moduleinfo)) {
				info.append(moduleinfo.ModuleName);
			} else 
				info.append("<Error_Module_Info>");
            
			if (pSymbol->MaxNameLen > 0) {
				info.append(pSymbol->Name);
			}
			else {
				info.append("<Unknown_Function>");
			}
            
        } else { 
            info.append(" <Unable to get symbol details>");
        }
        return info;
	}
    
    inline std::string WalkTheStack(){
        DWORD _ebp = INVALID_FP_RET_ADDR_VALUE;
        DWORD dwIPOfCurrentFunction = (DWORD)&WalkTheStack;
        
        // Get the current Frame pointer
        __asm {
            mov [_ebp], ebp
        }
        
        // We cannot walk the stack (yet!) without a frame pointer
        if (_ebp == INVALID_FP_RET_ADDR_VALUE)
            return false;
        
        //current Frame Pointe
        DWORD *pCurFP = (DWORD *)_ebp;
        BOOL fFirstFP = TRUE;
        
        std::string stackTrace("CurrFP\t\tRetAddr\t\tFunction\n");
        while (pCurFP != INVALID_FP_RET_ADDR_VALUE) {
            DWORD pRetAddrInCaller = (*((DWORD *)(pCurFP + 1)));
            
            stackTrace.append(vamssg("%.8p%*p        ", pCurFP, 20, (DWORD *)pRetAddrInCaller));
            
            if (fFirstFP) {
                fFirstFP = FALSE;
            }
            stackTrace.append(DisplaySymbolDetails(dwIPOfCurrentFunction));
            
            dwIPOfCurrentFunction = pRetAddrInCaller;
            
            stackTrace.append("\n");
            if (pRetAddrInCaller == INVALID_FP_RET_ADDR_VALUE) {
                break;
            }
            DWORD pCallerFP = *((DWORD *)pCurFP);
            pCurFP = (DWORD *)pCallerFP;
        }
        return stackTrace;
	}
    
    std::string getStackTrace() {
		HANDLE process = GetCurrentProcess();
		if(SymInitialize(process, NULL, TRUE)) {
			std::string result = WalkTheStack();
			SymCleanup(process);
			
			return result;
		}
		return "";
	}
#endif
    std::string SoraException::stackTrace() {
        std::string mStackTrace;
#ifdef __GNUC__
        const int len = 200;
        void* buffer[len];
        int nptrs = backtrace(buffer, len);
        char** strings = backtrace_symbols(buffer, nptrs);
        if (strings) {
            for (int i = 0; i < nptrs; ++i) {
                mStackTrace.append(strings[i]);
                mStackTrace.push_back('\n');
            }
            free(strings);
        }
#elif defined(OS_WIN32)
        mStackTrace = getStackTrace();
#endif
        return mStackTrace;
    }
    
    const char* SoraException::what() const throw() {
        std::ostringstream mssg;
        mssg << name() << ": " << mMssg;
        if(mFunction[0] != '?') mssg << "\nFunction:" << mFunction;
        if(mFile[0] != '?') mssg << "\nFile:" << mFile;
        if(mLine != -1) mssg << "\nLine:" << mLine;
        
        return mssg.str().c_str();
    }
    
    const char* SoraException::name() const throw() {
        return "SoraException";
    }
    
    const char* SoraException::className() const throw() {
        return typeid(*this).name();
    }
    
    const std::string& SoraException::mssg() const { 
        return mMssg;
    }
    
    const std::string& SoraException::function() const { 
        return mFunction;
    }
    
    const std::string& SoraException::file() const {
        return mFile;
    }
    
    int32 SoraException::line() const { 
        return mLine;
    }
    
    void SoraException::rethrow() {
        throw *this;
    }
    
    SORA_EXCEPTION_IMPL(LogicException,                 SoraException,      "Logic exception")
    SORA_EXCEPTION_IMPL(AssertionViolationException,    LogicException,     "Assertion violation")
    SORA_EXCEPTION_IMPL(NullPointerException,           LogicException,     "Null pointer")
    SORA_EXCEPTION_IMPL(NullValueException,             LogicException,     "Null value")
    SORA_EXCEPTION_IMPL(BugcheckException,              LogicException,     "Bugcheck")
    SORA_EXCEPTION_IMPL(InvalidArgumentException,       LogicException,     "Invalid argument")
    SORA_EXCEPTION_IMPL(NotImplementedException,        LogicException,     "Not implemented")
    SORA_EXCEPTION_IMPL(RangeException,                 LogicException,     "Out of range")
    SORA_EXCEPTION_IMPL(IllegalStateException,          LogicException,     "Illegal state")
    SORA_EXCEPTION_IMPL(InvalidAccessException,         LogicException,     "Invalid access")
    SORA_EXCEPTION_IMPL(SignalException,                LogicException,     "Signal received")
    SORA_EXCEPTION_IMPL(UnhandledException,             LogicException,     "Unhandled exception")
    
    SORA_EXCEPTION_IMPL(RuntimeException,               SoraException,      "Runtime exception")
    SORA_EXCEPTION_IMPL(NotFoundException,              RuntimeException,   "Item Not found")
    SORA_EXCEPTION_IMPL(ExistsException,                RuntimeException,   "Item Exists")
    SORA_EXCEPTION_IMPL(TimeoutException,               RuntimeException,   "Timeout")
    SORA_EXCEPTION_IMPL(SystemException,                RuntimeException,   "System exception")
    SORA_EXCEPTION_IMPL(RegularExpressionException,     RuntimeException,   "Error in regular expression")
    SORA_EXCEPTION_IMPL(LibraryLoadException,           RuntimeException,   "Cannot load library")
    SORA_EXCEPTION_IMPL(LibraryAlreadyLoadedException,  RuntimeException,   "Library already loaded")
    SORA_EXCEPTION_IMPL(NoThreadAvailableException,     RuntimeException,   "No thread available")
    SORA_EXCEPTION_IMPL(PropertyNotSupportedException,  RuntimeException,   "Property not supported")
    SORA_EXCEPTION_IMPL(PoolOverflowException,          RuntimeException,   "Pool overflow")
    SORA_EXCEPTION_IMPL(NoPermissionException,          RuntimeException,   "No permission")
    SORA_EXCEPTION_IMPL(OutOfMemoryException,           RuntimeException,   "Out of memory")
    SORA_EXCEPTION_IMPL(DataException,                  RuntimeException,   "Data error")
    
    SORA_EXCEPTION_IMPL(DataFormatException,        DataException,      "Bad data format")
    SORA_EXCEPTION_IMPL(SyntaxException,            DataException,      "Syntax error")
    SORA_EXCEPTION_IMPL(CircularReferenceException, DataException,      "Circular reference")
    SORA_EXCEPTION_IMPL(PathSyntaxException,        SyntaxException,    "Bad path syntax")
    SORA_EXCEPTION_IMPL(IOException,                RuntimeException,   "I/O error")
    SORA_EXCEPTION_IMPL(ProtocolException,          IOException,        "Protocol error")
    SORA_EXCEPTION_IMPL(FileException,              IOException,        "File access error")
    SORA_EXCEPTION_IMPL(FileExistsException,        FileException,      "File exists")
    SORA_EXCEPTION_IMPL(FileNotFoundException,      FileException,      "File not found")
    SORA_EXCEPTION_IMPL(PathNotFoundException,      FileException,      "Path not found")
    SORA_EXCEPTION_IMPL(FileReadOnlyException,      FileException,      "File is read-only")
    SORA_EXCEPTION_IMPL(FileAccessDeniedException,  FileException,      "Access to file denied")
    SORA_EXCEPTION_IMPL(CreateFileException,        FileException,      "Cannot create file")
    SORA_EXCEPTION_IMPL(OpenFileException,          FileException,      "Cannot open file")
    SORA_EXCEPTION_IMPL(WriteFileException,         FileException,      "Cannot write file")
    SORA_EXCEPTION_IMPL(ReadFileException,          FileException,      "Cannot read file")
    SORA_EXCEPTION_IMPL(UnknownURISchemeException,  RuntimeException,   "Unknown URI scheme")
    
    
    SORA_EXCEPTION_IMPL(ApplicationException,   SoraException,      "Application exception")
    SORA_EXCEPTION_IMPL(BadCastException,       RuntimeException,   "Bad cast exception")
    SORA_EXCEPTION_IMPL(RttiException,          RuntimeException,   "Rtti exception")

} // namespace sora