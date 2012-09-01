/*
 *  DivaNetworkSystem.h
 *
 *  Created by tempbuffer on 8/9/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 */

#ifndef DIVA_GNETWORK_SYSTEM
#define DIVA_GNETWORK_SYSTEM

#include "thread/SoraMutex.h"
#include "Core/DivaEventManager.h"
#include "Network/DivaGNetEventHandler.h"

namespace divacore
{
	using namespace sora;

	class GNetworkSystem : public EventHandler, public gnetwork::GNetEventHandler
	{
	public:
		enum{INIT,READY,CONNECTING,DISCONNECTING,WELL};

		GNetworkSystem():mState(INIT) {}

		int getState() {return mState;}
	protected:
		void setState(int state) { this->mState=state; }
	public:
		virtual void init() {}
		virtual void destory() {}

		virtual void gameLoad(const std::string &configFile) {}
		virtual void setHostInfo(const std::string &host, int PORT) = 0;

		//virtual bool check() = 0;
		virtual void connect() = 0;
		virtual void disconnect() = 0;
		//virtual bool waitConnection(float time) = 0;
		virtual void send(GPacket*) = 0;
		virtual void send(const std::string &id, const char*format="", ...) = 0;
		virtual void read(GPacket *packet, const char* format, va_list ArgPtr) = 0;
		virtual void read(GPacket *packet, const char* format, ...) = 0;
		virtual void waitForNext() = 0;

		virtual bool isConnected() = 0;
		virtual void update(float) {}
		virtual void refresh() { update(0); }
		//virtual bool has() = 0;
		virtual std::string toID(const std::string &group, const std::string &type) { return group+"#"+type; }
		virtual void toGandT(const std::string& id, std::string &group, std::string &type) {
			size_t index = id.find('#');
			group = id.substr(0,index);
			type = id.substr(index+1,id.size()-index-1);
		}
	protected:
		int mState;
	};

	typedef GNetworkSystem* GNetworkSystemPtr;
}

#endif