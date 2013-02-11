/****************************************************
 *  SysUtilWin.cpp
 *
 *  Created by tempbuffer on 12/16/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  A bunch of things that are related to windows 
 ****************************************************/

#include "../Type.h"

#if defined(BASE_OS_WINDOWS)

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <arpa/inet.h>
#endif

#include "SysUtilWin.h"

namespace Base
{
	namespace Net
	{
		dword NetUtilityImpl::htonl(dword v) {
			return ::htonl(v);
		}
		dword NetUtilityImpl::ntohl(dword v) {
			return ::ntohl(v);
		}
	}
}

#endif