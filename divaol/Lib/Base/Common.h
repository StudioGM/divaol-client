/****************************************************
 *  Common.h
 *
 *  Created by tempbuffer on 7/31/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  A common header to include
 ****************************************************/

#ifndef _BASE_COMMON_
#define _BASE_COMMON_

#include "Type.h"
#include "Exception.h"
#include "Ptr.h"
#include "PreDeclare.h"
#include "String.h"

#include <cstdarg>
#include <cstdio>
#include <cassert>

namespace Base
{
	//safe delete ptr
	template<typename T>
	void DeletePtr(const T* ptr){
		if(ptr) {
			delete ptr;
			ptr = 0;
		}
	}

	//safe delete stl pair
	template<typename T1, typename T2>
	void DeleteSTLPairPtr(const std::pair<T1, T2>& ptr) {
		if(ptr.second) {
			delete ptr.second;
			ptr.second = 0;
		}
	}

	// defines
	
	// prevent multi macro spread 
	#define BASE_JOIN(x, y) BASE_DO_JOIN(x, y)
	#define BASE_DO_JOIN(x, y) BASE_DO_JOIN_2(x, y)
	#define BASE_DO_JOIN_2(x, y) x##y
	
	#define BASE_UNIQUE_NAME(name)\
		BASE_JOIN(name, __LINE__)

	#define BASE_REGISTER_CLASS_NAME(_name)\
		virtual std::string getName() {return #_name;} \
		static std::string name() {return #_name;}
}

#endif