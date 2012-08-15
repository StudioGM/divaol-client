/****************************************************
 *  Platform.h
 *
 *  Created by tempbuffer on 7/31/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  Defines about the platform
 ****************************************************/

#ifndef _BASE_PLATFORM_
#define _BASE_PLATFORM_

/*******************************************
 * Compiler
 *******************************************/

// no cpp compiler
#if !defined(__cplusplus)
	#error C++ compiler required
#endif

// DEBUG
#if defined(DEBUG) || defined(_DEBUG)
	#define BASE_DEBUG
#endif 

// Turn off some annoying warnings
#if defined(_MSC_VER)
	#pragma warning(disable:4018) // signed/unsigned comparison
	#pragma warning(disable:4251) // ... needs to have dll-interface warning
	#pragma warning(disable:4355) // 'this' : used in base member initializer list
	#pragma warning(disable:4996) // VC++ 8.0 deprecation warnings
	#pragma warning(disable:4351) // new behavior: elements of array '...' will be default initialized
	#pragma warning(disable:4675) // resolved overload was found by argument-dependent lookup

	#pragma warning(disable:4786)  // identifier truncation warning
	#pragma warning(disable:4503)  // decorated name length exceeded - mainly a problem with STLPort
	#pragma warning(disable:4284)  // return type for operator -> is not UDT
#endif

#if defined(__intEL_COMPILER)
	#pragma warning(disable:1738) // base class dllexport/dllimport specification differs from that of the derived class
	#pragma warning(disable:1478) // function ... was declared "deprecated"
	#pragma warning(disable:1744) // field of class type without a DLL interface used in a class with a DLL interface
#endif

#if defined(__GNUC__)

	#define BASE_COMPILER_GCC

	#if __GNUC_MINOR__ >= 5
		#define BASE_COMPILER_VERSION 45
	#elif __GNUC_MINOR__ >= 4
		#define BASE_COMPILER_VERSION 44
	#elif __GNUC_MINOR__ >= 3
		#define BASE_COMPILER_VERSION 43
	#elif __GNUC_MINOR__ >= 2
		#define BASE_COMPILER_VERSION 42
	#elif __GNUC_MINOR__ >= 1
		#define BASE_COMPILER_VERSION 41
	#elif __GNUC_MINOR__ >= 0
		#define BASE_COMPILER_VERSION 40
	#else
		#error Unknown Compiler
	#endif

	#if __GNUC_MINOR__ >= 3
		#ifdef __GXX_EXPERIMENTAL_CXX0X__
			#define BASE_CXX0X_SUPPORT
		#endif // __GXX_EXPERIMENTAL_CXX0X__
	#endif // __GNUC_MINOR__

#elif defined(_MSC_VER)

	#define BASE_COMPILER_MSVC
	#define BASE_HAS_DECLSPEC

	#if _MSC_VER >= 1600
		#define BASE_COMPILER_VERSION 100
		#define BASE_CXX0X_SUPPORT
	#elif _MSC_VER >= 1500
		#define BASE_COMPILER_VERSION 90
	#elif _MSC_VER >= 1400
		#define BASE_COMPILER_VERSION 80
	#else
		#error Unknown Compiler
	#endif

#endif

/*******************************************
 * Operation System
 *******************************************/

//apple
#if defined(__APPLE__) || defined(__APPLE_CC__)
	#define BASE_OS_FAMILY_APPLE
	#define BASE_OS_FAMILY_UNIX
	#if !defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
		#define BASE_OS_OSX
	#else
		#define BASE_OS_IOS
	#endif

	#if MAC_OS_X_XVERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_7
		#define BASE_APPLE_OSX_LION
	#endif

//andiord
#elif defined(__ANDROID__)
	#define BASE_OS_ANDROID
	#define BASE_OS_FAMILY_GOOGLE
	#define BASE_OS_FAMILY_UNIX

//windows
#elif defined(_WIN32) || defined(_MSC_VER)
	#define BASE_OS_WINDOWS
	#define BASE_OS_FAMILY_MS

	#if defined(_WIN64)
		#define BASE_OS_WINDOWS_64
	#else
		#define BASE_OS_WINDOWS_32
	#endif

	#if defined(_WIN32)
		#if !defined(WIN32_LEAN_AND_MEAN)
			#define WIN32_LEAN_AND_MEAN
		#endif
	#endif

	#if defined(__MINGW32__)
		#define BASE_PLATFORM_MINGW
	#endif

	#if defined(__CYGWIN__)
		#define BASE_PLATFORM_CYGWIN
		#define BASE_HAS_DECLSPEC
	#endif

	#include <windows.h>
	#pragma comment(lib,"winmm.lib")

//linux
#elif defined(linux) || defined(__linux)
	#define BASE_OS_LINUX
	#define BASE_OS_FAMILY_UNIX

//psp
#elif defined(_PSP)
	#define BASE_OS_PSP

//Unknown Platforms
#else
	#error Unknown Platform
#endif

#endif