/****************************************************
 *  Sys.h
 *
 *  Created by tempbuffer on 9/16/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  Link to specific system
 ****************************************************/

#ifndef _BASE_SYS_
#define _BASE_SYS_

#include "Type.h"

#ifdef BASE_OS_WINDOWS
#include "SysUtilWin.h"
#elif BASE_OS_FAMILY_APPLE
#include "SysUtilApple.h"
#elif BASE_OS_LINUX
#include "SysUtilLinux.h"
#else
#define BASE_UNSUPPORTED_SYS
#endif

#endif