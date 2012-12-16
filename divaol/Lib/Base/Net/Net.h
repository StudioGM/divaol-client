/****************************************************
 *  Net.h
 *
 *  Created by tempbuffer on 9/17/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  Base network tools
 ****************************************************/

#ifndef _NET_H_
#define _NET_H_

#include "../Common.h"

namespace Base
{
	namespace Net
	{
		class Utility
		{
		public:
			static dword htonl(dword v) {return NetUtilityImpl::htonl(v);}
			static dword ntohl(dword v) {return NetUtilityImpl::ntohl(v);}
		};
	}
}

#endif