/*
 *  DivaClient.h
 *
 *  Created by tempbuffer on 9/27/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 */

#ifndef DIVA_NET_INFO
#define DIVA_NET_INFO

#include "DivaGNetSystem.h"
#include "Lib/Base/Ptr.h"
#include "Lib/Base/Pattern/Singleton.h"

namespace divanet
{
	struct ServiceInfo
	{
		Base::String ip;
		uint32 port;
		Base::String description;

		ServiceInfo(const Base::String &ip, uint32 port, Base::String description = "")
			:ip(ip),port(port),description(description) {}
		ServiceInfo() {}
	};
	typedef std::map<std::string,ServiceInfo> ServiceInfos;
	
	class NetInfo : public Base::Singleton<NetInfo>
	{
	public:
		void getServerInfo() {
<<<<<<< HEAD
			info["auth"] = ServiceInfo("58.215.170.145",9699);
			info["chat"] = ServiceInfo("58.215.170.145",9799);
			info["stage"] = ServiceInfo("58.215.170.145",9899);
			info["scheduler"] = ServiceInfo("58.215.170.145",9099);
=======
			//static const char* const SERVER_IP = "10.155.0.105";
			static const char* const SERVER_IP = "divaolserver00.gmgate.net";

			info["auth"] = ServiceInfo(SERVER_IP,9699);
			info["chat"] = ServiceInfo(SERVER_IP,9799);
			info["stage"] = ServiceInfo(SERVER_IP,9899);
			info["scheduler"] = ServiceInfo(SERVER_IP,9099);
>>>>>>> 8fac03783867a4916e28db1e466348ee4dc2cf87
		}

		ServiceInfo server(const std::string server) {
			return info[server];
		}

	protected:
		friend class Base::Singleton<NetInfo>;

		NetInfo():login(false) {}
		~NetInfo() {}

	public:
		bool login;
		std::string username;
		Base::String nickname;
		std::string uid;
		gnet::Bytes token;
		ServiceInfos info;
<<<<<<< HEAD
		static const int TIME_OUT = 10;
=======
		static const int TIME_OUT = 20;
>>>>>>> 8fac03783867a4916e28db1e466348ee4dc2cf87
		static const int TICK_TIME = 1;
		static const int RECONNECT_TIME = 10;
	};

#define NET_INFO (divanet::NetInfo::instance())
}

#endif