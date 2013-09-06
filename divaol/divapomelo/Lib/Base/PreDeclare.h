/****************************************************
 *  PreDeclare
 *
 *  Created by tempbuffer on 8/12/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 ****************************************************/

#ifndef _BASE_PRE_DECLARE_H_
#define _BASE_PRE_DECLARE_H_

#include "Ptr.h"

namespace Base
{
	class Exception;
	
	class Logger;
	
	class String;
	
	class Timer;

	class Stream;
	typedef SharedPtr<Stream> StreamPtr;
}

#endif