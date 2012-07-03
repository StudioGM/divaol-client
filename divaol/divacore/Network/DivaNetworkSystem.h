/*
 *  DivaNetworkSystem.h
 *
 *  Created by Hyf042 on 2/10/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_NETWORK_SYSTEM
#define DIVA_NETWORK_SYSTEM

#include "thread/SoraMutex.h"
#include "Core/DivaEventManager.h"
#include "Network/DivaNetEventHandler.h"
#include "Network/DivaNetPacketID.h"

namespace divacore
{
	using namespace sora;
	using network::Packet;

	class NetworkSystem : public EventHandler, public network::NetEventHandler
	{
		sora::SoraMutex mutex;
		int state;
	protected:
		void setState(int state) 
		{
			sora::SoraMutexGuard lock(mutex);
			this->state=state;
		}
	public:
		enum{INIT,READY,CONNECTING,DISCONNECTING,WELL};
		NetworkSystem():state(INIT) {}

		int getState() {sora::SoraMutexGuard lock(mutex);return state;}

		virtual void init() {}
		virtual void gameLoad(const std::string &configFile) {}
		virtual void setHostInfo(int PORT) = 0;

		//virtual bool check() = 0;
		virtual void connect() = 0;
		virtual void disconnect() = 0;
		virtual bool waitConnection(float time) = 0;
		virtual void send(Packet&) = 0;
		virtual void send(uint32 id, float time = 0, const char*format="", ...) = 0;
		virtual bool isConnected() = 0;
		virtual void update(float) {}
		//virtual bool has() = 0;
	};

	typedef NetworkSystem* NetworkSystemPtr;
}

#endif