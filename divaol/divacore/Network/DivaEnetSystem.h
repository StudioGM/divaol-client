/*
 *  EnetSystem.h
 *
 *  Created by Hyf042 on 2/29/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#include "DivaNetworkSystem.h"
#include "enet/enet.h"

#pragma comment(lib,"enet.lib")

namespace divacore
{
	using namespace std;

	class EnetSystem : public NetworkSystem
	{
		static const int CHANNEL_NUM = 3;
		int PORT;
		std::string IP_ADDRESS;
		ENetHost *client;
		ENetPeer *peer;
		//sora::SoraMutex msg_mutex;

	public:
		EnetSystem():client(NULL),peer(NULL) {}
		
		bool isConnected() {return getState()==WELL;}

		void setHostInfo(int PORT)
		{
			this->PORT = PORT;
		}

		void init()
		{
			//初始化enet库
			if(enet_initialize())
				DIVA_EXCEPTION_MODULE("enet initialize error","EnetNetworkSystem");
			//创建本地HOST对象
			client=enet_host_create(NULL,
				1,   //只允许连接一个服务器
				CHANNEL_NUM,   //3个通道
				0,0);
			if(client==NULL)
				DIVA_EXCEPTION_MODULE("enet create host","EnetNetworkSystem");

			LOGGER->msg("Enet initialized","EnetNetworkSystem");

			setState(READY);

			IP_ADDRESS = "127.0.0.1";
			PORT = 1234;
		}

		void destroy()
		{
			disconnect();

			if(peer)
				enet_peer_reset(peer);

			enet_deinitialize();
		}

		virtual void gameLoad(const std::string &configFile) 
		{
			Config config;
			configloader::loadWithJson(config,configFile);
			IP_ADDRESS = config.getAsString("IP");
		}

		bool waitConnection(float time)
		{
			ENetEvent event;
			if (enet_host_service (client, &event, int(time*1000)) > 0 &&
				event.type == ENET_EVENT_TYPE_CONNECT)
				return true;
			else
			{
				enet_peer_reset (peer);
				peer = NULL;
				LOGGER->msg("Connction Failure","EnetNetworkSystem");
				return false;
			}
		}
		void connect()
		{
			LOGGER->msg("Connecting...","EnetNetworkSystem");

			//连接到服务器
			ENetAddress svraddr;
			enet_address_set_host(&svraddr,IP_ADDRESS.c_str());
			svraddr.port=PORT;

			peer=enet_host_connect(client,&svraddr,CHANNEL_NUM,0); //client连接到svraddr对象，共分配三个通道
			if(peer==NULL)
				DIVA_EXCEPTION_MODULE("can not connect to server","EnetNetworkSystem");

			setState(CONNECTING);
		}
		void disconnect()
		{
			if(peer)
			{
				enet_peer_disconnect (peer,0);
				setState(DISCONNECTING);

				ENetEvent event;
				while (enet_host_service (client, & event, 3000) > 0)
				{
					switch (event.type)
					{
					case ENET_EVENT_TYPE_RECEIVE:
						enet_packet_destroy (event.packet);
						break;

					case ENET_EVENT_TYPE_DISCONNECT:
						LOGGER->msg("Disconnection Succeeded","EnetNetworkSystem");
						peer = NULL;
						return;
					}
				}
				enet_peer_reset(peer);
				peer = NULL;
				LOGGER->msg("Forced Reset","EnetNetworkSystem");
			}
		}
		void dispatchMessage(ENetPacket*_packet)
		{
			Packet packet((const char*)_packet->data,_packet->dataLength);
			dispatch(packet.id,packet);
		}
		void send(Packet& packet)
		{
			if(peer==NULL)
				return;
			ENetPacket *_packet=enet_packet_create(NULL,packet.size(),ENET_PACKET_FLAG_RELIABLE); //创建包
			memcpy((char*)_packet->data,packet.getPointer(),packet.size());
			enet_peer_send(peer,0,_packet);
			enet_host_flush (client);
		}
		void send(uint32 id, float time = 0, const char*format="", ...)
		{
			Packet packet(id,time);

			va_list	ArgPtr;
			va_start(ArgPtr, format);
			packet.write(format, ArgPtr);
			va_end(ArgPtr);

			send(packet);
		}
		void update(float dt)
		{
			if(getState()==INIT)
				return;

			ENetEvent event;
			// Check if there are any new events
			while (enet_host_service(client, &event, 0) > 0)
			{
				switch (event.type)
				{
				case ENET_EVENT_TYPE_CONNECT:
					LOGGER->log("Connected to port %d.", PORT);
					// Store any relevant server information here.
					event.peer->data = 0;
					setState(WELL);
					break;

				case ENET_EVENT_TYPE_RECEIVE:
					dispatchMessage(event.packet);
					enet_packet_destroy (event.packet);
					break;

				case ENET_EVENT_TYPE_DISCONNECT:
					LOGGER->log("Disconnected.");
					// Reset the server information.
					event.peer->data = 0;
					peer = NULL;

					setState(READY);
					break;

				case ENET_EVENT_TYPE_NONE:
				default:
					break;
				}
			}
		}
	};
}