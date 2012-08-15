/****************************************************
 *  Type.h
 *
 *  Created by tempbuffer on 7/31/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  Define types that is needed
 ****************************************************/

#include <cstdlib>
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

	#if defined(BASE_OS_WINDOWS64)
		typedef signed long long	intPtr;
		typedef unsigned long long	uintPtr;
	#elif defined(BASE_OS_WINDOWS32)
		typedef signed long			intPtr;
		typedef unsigned long		uintPtr;
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
	
	#define base_malloc malloc
	
	/*
	 * String
	 */
	typedef std::string base_string;
	typedef std::wstring base_wstring;
	typedef wchar_t wchar;
}

#endif