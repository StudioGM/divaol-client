/****************************************************
 *  Type.h
 *
 *  Created by tempbuffer on 7/31/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  Define types that is needed
 ****************************************************/

#include <cstdlib>
#include <cassert>
#include <string>
#include "Platform.h"

#ifndef _BASE_TYPE_
#define _BASE_TYPE_

namespace Base
{
	/*
	 * All kinds of Intergers
	 */
	typedef signed char			int8;
	typedef unsigned char		uint8;
	typedef signed short		int16;
	typedef unsigned short		uint16;
	typedef signed int			int32;
	typedef unsigned int		uint32;
	typedef signed long long	int64;
	typedef unsigned long long	uint64;

	#if defined(BASE_OS_WINDOWS_64)
		typedef signed long long	intPtr;
		typedef unsigned long long	uintPtr;
	#elif defined(BASE_OS_WINDOWS_32)
		typedef signed long			intPtr;
		typedef unsigned long		uintPtr;
	#endif
    
#if !defined(BASE_OS_WINDOWS_64) && !defined(BASE_OS_WINDOWS_32)
	typedef signed long            intPtr;
	typedef unsigned long          uintPtr;
#if defined(__LP64__)
    #define _PTR_IS_64_BIT 1
    #define _LONG_IS_64_BIT 1
    
    #ifdef _UINT64
        #undef _UINT64
    #endif
    
    #ifdef __INT64
        #undef __INT64
    #endif
	typedef int64_t        int64;
	typedef uint64_t		uint64;
#else
	typedef signed long long   int64;
	typedef unsigned long long uint64;
#endif
#endif
	/*
	 * Raw Data
	 */
	typedef unsigned char		byte;
	typedef unsigned short		word;
	typedef unsigned int		dword;
	typedef unsigned long long	qword;
	typedef unsigned int		size;
	typedef uint8*				binary;

	/*
	 * String
	 */
	typedef std::string base_string;
	typedef std::wstring base_wstring;
	typedef wchar_t wchar;

	/*
	 * special
	 */

	#define base_malloc malloc
	#define base_malloc_withtype(T,SIZE) ((T*)malloc(sizeof(T)*(SIZE)))
	#define base_free free
	#define base_assert assert
}

#endif