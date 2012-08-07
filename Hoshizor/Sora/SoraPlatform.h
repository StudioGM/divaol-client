/*
 *  SoraPlatform.h
 *  Hoshizora
 *
 *  Created by Robert Bu on 8/24/10.
 *  Copyright 2010 Griffin Bu(Project HoshiNoSora). All rights reserved.
 *
 */


#ifndef _SORA_PLATFORM_H_
#define _SORA_PLATFORM_H_

#define SORA_VERSION_MAJOR 4
#define SORA_VERSION_MINOR 0
#define SORA_VERSION_REV   1


#if !defined(__cplusplus)
    #error C++ compiler required.
#endif

#if defined(DEBUG) | defined(_DEBUG)
#define SORA_DEBUG
#endif

/*
 if gcc version >= 4
 then there is tr1 support
 visual studio 2008 sp1 or above also has tr1 support
 */
#if defined(__GNUC__)

    #define SORA_COMPILER_GCC

    #if __GNUC_MINOR__ >= 5
        #define SORA_COMPILER_VERSION 45
    #elif __GNUC_MINOR__ >= 4
        #define SORA_COMPILER_VERSION 44
    #elif __GNUC_MINOR__ >= 3
        #define SORA_COMPILER_VERSION 43
    #elif __GNUC_MINOR__ >= 2
        #define SORA_COMPILER_VERSION 42
    #elif __GNUC_MINOR__ >= 1
        #define SORA_COMPILER_VERSION 41
    #elif __GNUC_MINOR__ >= 0
        #define SORA_COMPILER_VERSION 40
    #else
        #error Unknown Compiler
    #endif

    #if __GNUC_MINOR__ >= 3
        #ifdef __GXX_EXPERIMENTAL_CXX0X__
            #define SORA_CXX0X_SUPPORT
        #endif
    #endif

	#if __GNUC__ >= 4
		#include <tr1/unordered_map>
        namespace sora {
            #define sora_hash_map std::tr1::unordered_map
        }
	#else
		#include <ext/hash_map>
        namespace sora {
            #define sora_hash_map __gnu_cxx::hash_map
        }
	#endif

#elif defined(_MSC_VER)

    #define SORA_COMPILER_MSVC
    #define SORA_HAS_DECLSPEC

    #if _MSC_VER >= 1600
        #define SORA_COMPILER_VERSION 100
        #define SORA_CXX0X_SUPPORT
    #elif _MSC_VER >= 1500
        #define SORA_COMPILER_VERSION 90
    #elif _MSC_VER >= 1400
        #define SORA_COMPILER_VERSION 80
    #else
        #error Unknown Compiler
    #endif

#if _MSC_VER >= 1600
    #include <unordered_map>
    namespace sora {
		#define sora_hash_map std::tr1::unordered_map
    }

#else
    namespace sora {
        #define sora_hash_map std::hash_map
    }
#endif 

#else

    #include <map>
    namespace sora {
        #define sora_hash_map std::map
    }
#endif


// apple os ( osx / ios )
#if defined(__APPLE__) || defined(__APPLE_CC__)
    #if !defined(__IPHONE_OS_VERSION_MAX_ALLOWED)
        #define OS_OSX
    #else
        #define OS_IOS
    #endif
// android
#elif defined(__ANDROID__)
    #define OS_ANDROID
// windows
#elif defined(_WIN32) || defined(_MSC_VER)
    #define OS_WIN32

    #if defined(_WIN64)
        #define SORA_PLATFORM_64
    #else
        #define SORA_PLATFORM_32
    #endif

    #ifndef NOMINMAX
        #define NOMINMAX
    #endif

    #if defined(__MINGW32__)
        #include <_mingw.h>
    #endif

    #if defined(__CYGWIN__)
        #define SORA_PLATFORM_CYGWIN
        #define SORA_HAS_DECLSPEC
    #endif

// linux
#elif defined(linux) || defined(__linux)
    #define OS_LINUX
// psp
#elif defined(_PSP)
    #define OS_PSP
#else
    #error Unknown Platform
#endif

#if defined(SORA_COMPILER_MSVC)
    #if defined(_M_X64) 
        #define SORA_CPU_X64
        #define SORA_COMPILER_TARGET x64
    #elif defined(_M_IX86)
        #define SORA_CPU_X86
        #define SORA_COMPILER_TARGET x86
    #endif
#elif defined(SORA_COMPILER_GCC)
    #if defined(__x86_64__)
        #define SORA_CPU_X64
        #define SORA_COMPILER_TARGET x64
    #elif defined(__i386__)
        #define SORA_CPU_X86
        #define SORA_COMPILER_TARGET x86
    #endif
#endif

#if defined(SORA_CPU_PPC)
    #define SORA_BIG_ENDIAN
#elif defined(SORA_CPU_X86) || defined(SORA_CPU_X64) || defined(OS_WIN32)
    #define SORA_LITTLE_ENDIAN
#else
    #define SORA_BIG_ENDIAN
#endif

#ifdef SORA_CPU_X64
    #define _SSE_SUPPORT
    #define _SSE2_SUPPORT
    #define _X64_SUPPORT
#elif defined SORA_CPU_X86
    #if defined(SORA_COMPILER_MSVC)
        #if _M_IX86 == 600
            #define _MMX_SUPPORT
        #endif

        #if _M_IX86_FP == 1
            #define _SSE_SUPPORT
        #elif _M_IX86_FP == 2
            #define _SSE_SUPPORT
            #define _SSE2_SUPPORT
        #endif
    #elif defined(SORA_COMPILER_GCC)
        #ifdef __MMX__
            #define _MMX_SUPPORT
        #endif
        #ifdef __SSE__
            #define _SSE_SUPPORT
        #endif
        #ifdef __SSE2__
            #define _SSE2_SUPPORT
        #endif
    #endif
#endif

/*
 flag for shaders in render system
 disable this to get rid of shader
 */
//#define SORA_USE_SHADER

/*
 use RTTI for SoraEvents and so on,
 if disabled, then SoraEvent must give it's unique name for type identification
 */
#if !defined(SORA_DLL_EXPORT) && !defined(SORA_DLL_IMPORT)
//#define SORA_USE_RTTI
#endif

/*
 if disabled, all exception would be disabled
 */
#define SORA_USE_EXCEPTION

/*
 if disabled, SoraSimpleFSM would not use a NULL state as initial state
 */
#define SORA_FSM_USE_NULL

/*
 enable this if you are using sora in multithread environment
 currently unimplemented, reverved for future use
 */
#define SORA_ENABLE_MULTI_THREAD

#define SORA_MAX_IO_THREAD 1

/*
 SoraThread Plugin Option
 If SORA_WIN32_PTHREAD have been defined, then SoraThread would try to use external pthread library under Windows
 Otherwise it would use Win32API to archive the goal
 Default: NO
 */
#ifdef OS_WIN32
//#define SORA_WIN32_PTHREAD
#endif


/*
 automatically register plugins(if you choose to compile the plugin into the app)
 depends on plugin implemention
 plugins can choose to use this macro to define whether to 
 register itself to sora when the app starts
 for example: 
 render systems can register itself when the app starts
 default: on
 */
#define SORA_AUTOMATIC_REGISTER


/**
 uncomment this to enable auto debug render event receiving
 and also you must enable DebugRender in SoraCore
 **/
//#define SORA_DEBUG_RENDER

/**
 * Enable core zip file pack support(in resource file manager)
 * Comment this to disable
 **/
#define SORA_ZIP_PACK_SUPPORT


/**
 * Defines the event receiving type
 * If defined, eventfunc registering is not required
 * All Event handling goes through SoraEventHandler::onEvent
 * this maybe faster but difficult to know the event type
 * Default: OFF
 **/
//#define SORA_EVENT_GENERIC


#define SORA_SINGLE_PRECISION

/**
 * data type definition compabilty 
 * some data types not available in sora4
 * for old plugins
 **/
// #define SORA_COMPABILITY_SORA3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef _PSP
#include <memory.h>
#else
#include <malloc.h>
#include <memory>
#endif

#include "SoraConfig.h"

typedef     unsigned long   SoraHandle;

#ifdef SORA_SINGLE_PRECISION
    typedef		float			real;
#else
    typedef     double          real;
#endif

#ifdef SORA_COMPABILITY_SORA3
    typedef unsigned long uint32
    typedef long long32;
#endif


#include "SoraMemory.h"
#include "SoraRenderParameters.h"

#include <stdint.h>
#include <cassert>

#if defined(SORA_STD_CALL)
#define SORACALL __stdcall
#else
#define SORACALL
#endif

#if defined(_WIN32)
#define strcmpnocase stricmp
#else
#define strcmpnocase strcasecmp
#endif

// are we building a dll or not
#if defined(OS_WIN32) && defined(SORA_HAS_DECLSPEC)
    #if defined(SORA_DLL_EXPORT)
        #define SORA_API __declspec(dllexport)
		#define SORA_EXTERN extern
    #elif defined(SORA_DLL_IMPORT)
        #define SORA_API __declspec(dllimport)
		#define SORA_EXTERN extern
	#else
		#define SORA_API
		#define SORA_EXTERN
	#endif
#else
    #define SORA_API
	#define SORA_EXTERN
#endif

#ifndef OS_PSP
#include <string>
typedef std::string SoraString;
typedef std::wstring SoraWString;

#define HAS_WSTRING

#else
#include <string>
typedef std::string SoraString;
namespace std {
	typedef std::basic_string<wchar_t> wstring;
	typedef basic_ostringstream<wchar_t> wostringstream;
}
typedef std::wstring SoraWString;
#endif


#ifdef OS_WIN32
#define sora_fopenw(path, mode) _wfopen(path, TEXT(mode))

#include <windows.h>
static void msleep(uint32 msec) {
	Sleep(msec);
}

#else
#define sora_fopenw(path, mode) fopen(ws2s(path).c_str(), mode)
#include <time.h>
#include <errno.h>
// sleep for milliseconds
// see my http://cc.byexamples.com/2007/05/25/nanosleep-is-better-than-sleep-and-usleep/ for more information
static void msleep(uint32_t msec) {
	struct timespec timeout0;
	struct timespec timeout1;
	struct timespec* tmp;
	struct timespec* t0 = &timeout0;
	struct timespec* t1 = &timeout1;
	
	t0->tv_sec = msec / 1000;
	t0->tv_nsec = (msec % 1000) * (1000 * 1000);	
	while ((nanosleep(t0, t1) == (-1)) && (errno == EINTR)) {

		tmp = t0;
		t0 = t1;
		t1 = tmp;
	}
}


#define MB_OK				1
#define MB_OKCANCEL			2
#define MB_ICONERROR		4
#define MB_ICONSTOP			8
#define MB_ICONWARNING		16
#define MB_ICONINFORMATION	32

#define IDOK				1
#define IDCANCEL			2
#define IDABORT				3
#define IDRETRY				4
#define IDYES				6
#define IDNO				7

#endif


#define sora_assert assert

namespace sora {
    template<bool x> struct STATIC_ASSERTION_FAILURE;
    template<> struct STATIC_ASSERTION_FAILURE<true> { enum {Value = 1}; };
    template<int x> struct static_assert_test{};
}


#define SORA_JOIN(x, y) SORA_DO_JOIN(x, y)
#define SORA_DO_JOIN(x ,y) SORA_DO_JOIN_2(x, y)
#define SORA_DO_JOIN_2(x, y) x##y

#define sora_static_assert(B) \
    typedef ::sora::static_assert_test<\
        sizeof(::sora::STATIC_ASSERTION_FAILURE<(bool)(B)>)>\
            SORA_JOIN(__sora_static_assert_typedef_, __LINE__)



#define SORA_STATIC_INSTANCE_DECLARE(T) \
    static T* Instance(); \

#define SORA_STATIC_INSTANCE_IMP(T) \
    T* T::Instance() { \
        static T instance; \
        return &instance; \
    }

// this macro runs a seg of code when the app starts using static init for classes
// useful for plugins to register itself to sora
// however, static run code's name may conflict if they are in the same line of the project
// so use of SORA_STATIC_INIT_I is suggested

#define SORA_UNIQUE_NAME(name) \
    SORA_JOIN(name, __LINE__)

#define SORA_STATIC_RUN_CODE_I(name, code) \
namespace { \
    static struct name { \
       name() { \
            code; \
        } \
    } SORA_JOIN(g_, name); \
}

#define SORA_STATIC_RUN_CODE(code) \
    SORA_STATIC_RUN_CODE_I(SORA_UNIQUE_NAME(sora_static_), code) 

#define SORA_STATIC_INIT_I(name, FN) \
namespace { \
    static struct name { \
        name() { \
            static int counter = 0; \
            if(counter++ > 0) return; \
            FN(); \
        } \
    } g_##name; \
}

#define SORA_STATIC_INIT(FN) \
    SORA_STATIC_INIT_I(sora_static_init_##FN, FN)

#define SORA_STATIC_RUN_FN(FN) \
    SORA_STATIC_INIT(FN)

#define SORA_STATIC_RUN_CODE_FN(name, code) \
namespace { \
    static void sora_static_fn_##name() { \
        code; \
    } \
    SORA_STATIC_INIT(sora_static_fn_##name) \
}

/*
 Thread Independent Variables
 */
#ifdef _MSC_VER
    #define ThreadLocal __declspec(thread)
#elif defined(__GNUC__) && !defined(OS_OSX) && !defined(OS_IOS)
    #define ThreadLocal __thread
#else
// some platforms such as Mac OS X does not support TLS
// so we might need some other solutions
#define ThreadLocal
#endif

#ifndef __GNUC__
#define snprintf _snprintf
#endif


#endif // _SORA_PLATFORM_H_

