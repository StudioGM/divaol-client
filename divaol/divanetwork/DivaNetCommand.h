/*
 *  DivaNetCommand.h
 *
 *  Created by tempbuffer on 1/17/13.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  Command Manager of network
 */

#ifndef DIVA_NET_COMMAND
#define DIVA_NET_COMMAND

#include "DivaGNetSystem.h"
#include "Lib/Base/Ptr.h"
#include "Lib/Base/Pattern/Singleton.h"
#include "divanetwork/DivaAuthClient.h"

namespace divanet
{
	class NetCommand : public Base::Singleton<NetCommand>
	{
	public:
		bool Analysis(const Base::String &content)
		{
			if(content.size() == 0 || content[0] != L'-')
				return false;
			Base::String command = content(1,-1);
			if(command == L"ping")
			{
				AUTH_CLIENT.ping();
				return true;
			}
			return false;
		}
	protected:
		friend class Base::Singleton<NetCommand>;

		NetCommand() {}
		~NetCommand() {}

	public:
	};

	#define NET_COMMAND (divanet::NetCommand::instance())
}

#endif