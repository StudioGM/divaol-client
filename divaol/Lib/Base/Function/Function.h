/****************************************************
 *  Function.h
 *
 *  Created by tempbuffer on 9/16/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  A class to package function to a object
 ****************************************************/

#ifndef _BASE_FUNCTION_H_
#define _BASE_FUNCTION_H_

#include "../Common.h"
//#include "MemFunc.h"
#include "FunctionBase.h"
#include "FunctionInclude.h"

namespace Base
{
	using function_detial::MemFunc;
	using function_detial::Binded;
	using function_detial::Function;
	using function_detial::Bind;
	using function_detial::MakeFunction;

	typedef Function<void()> function;
}
#endif