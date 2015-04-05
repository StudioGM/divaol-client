/*
 *  NetInfo.h
 *
 *  Created by tempbuffer on 8/9/13.
 *  Copyright 2013 tempbuffer. All rights reserved.
 *
 */

#ifndef _DIVAPOMELO_NETINFO_H_
#define _DIVAPOMELO_NETINFO_H_

#include "../Lib/base/base.h"
#include <memory>

namespace divapomelo
{
	using namespace Base;
	//!HINT: to solve ambiguous problems, use specific using
	using Base::uint32;
	using Base::uint64;
	using Base::int32;
	using Base::int64;

	class UserInfo
	{
	public:
		UserInfo() {}
		~UserInfo() {}

		std::string username;
		Base::String nickname;
		std::string uid;

		static const int TIME_OUT = 20;
		static const int TICK_TIME = 1;
		static const int RECONNECT_TIME = 10;
	};

	class ServerInfo
	{
	public:
		ServerInfo() {
			gatePort = 3014;
			gateIp = "10.155.0.198";
			//gateIp = "127.0.0.1";
		}
		~ServerInfo() {}

		uint32 gatePort;
		Base::String gateIp;
		uint32 connectorPort;
		Base::String connectorIp;
	};

}

#endif