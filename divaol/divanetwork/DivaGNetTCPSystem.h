/*
 *  DivaTCPGNetworkSystem.h
 *
 *  Created by tempbuffer on 8/9/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 */

#ifndef DIVA_TCP_GNETWORK_SYSTEM
#define DIVA_TCP_GNETWORK_SYSTEM

#include "thread/SoraMutex.h"
#include "Core/DivaEventManager.h"
#include "divanetwork/DivaGNetSystem.h"
#include "gnet/netManager.h"
#include "Core/DivaStringUtil.h"

namespace divanet
{
	using gnet::Item;
	using gnet::Tuple;
	using gnet::Atom;
	using namespace sora;

	class TCPGNetworkSystem : public GNetworkSystem
	{
	public:
		TCPGNetworkSystem() {}

		void init()  {
			mHostIP = "127.0.0.1";
			mPort = "9899";
		}
		void destory() {
			mNetManager.disconnect();
		}
		void gameLoad(const std::string &configFile) {
			divacore::Config config;
			divacore::configloader::loadWithJson(config,configFile);

			if(config.has("IP"))
				mHostIP = config.getAsString("IP");
			if(config.has("PORT"))
				mPort = divacore::iToS(config.getAsInt("PORT"));
		}
		void setHostInfo(const std::string &host, int port) {
			if(host!="")
				mHostIP = host;
			if(port>=0)
				mPort = divacore::iToS(port);
		}

		//virtual bool check() = 0;
		void connect() {
			LOGGER->msg("Connecting...","TCPGNetSystem");

			setState(CONNECTING);
			mNetManager.connect(mHostIP,mPort);
			setState(WELL);
		}
		void disconnect() {
			setState(DISCONNECTING);
			mNetManager.disconnect();
			setState(READY);
		}
		//virtual bool waitConnection(float time) = 0;
		void send(GPacket *packet) {mNetManager.send(packet);}
		void send(const std::string &id, const char*format="", ...) {

			va_list	ArgPtr;
			va_start(ArgPtr, format);
			GPacket *packet = gnet::ItemUtility::formatTuple(format, ArgPtr);
			va_end(ArgPtr);

			std::string group,type;
			toGandT(id,group,type);
			packet->appendAhead<Atom>(type);
			packet->appendAhead<Atom>(group);

			mNetManager.send(packet);
		}
		void read(GPacket *packet, const char* format, va_list ArgPtr) {
			GPacket tmp = *packet;
			tmp.deleteItem(0);
			tmp.deleteItem(0);
			gnet::ItemUtility::formatReadTuple(&tmp,format,ArgPtr);
			tmp.clear(true);
		}
		void read(GPacket *packet, const char* format, ...) {
			va_list	ArgPtr;

			va_start(ArgPtr, format);
			//vsprintf(Message, format, ArgPtr);

			read(packet, format,ArgPtr);

			va_end(ArgPtr);
		}
		void waitForNext()
		{
			mNetManager.waitNext();
		}

		virtual bool isConnected() {
			return getState()==WELL;
		}
		virtual void update(float) {
			if(getState()==INIT||getState()==READY)
				return;

			GPacket *event;

			while(!mNetManager.empty())
			{
				event = dynamic_cast<GPacket*>(mNetManager.recv());

				Item<Atom>* group_item = dynamic_cast<Item<Atom>*>(event->getItem(0));

				std::string group = group_item->getData();
				std::string type = dynamic_cast<Item<Atom>*>(event->getItem(1))->getData();

				dispatch(toID(group,type),event);

				delete event;
			}
		}
		//virtual bool has() = 0;
	private:
		gnet::NetManager mNetManager;
		std::string mHostIP;
		std::string mPort;
	};

	typedef GNetworkSystem* GNetworkSystemPtr;
}

#endif