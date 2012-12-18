/*
 *  connection.h
 *
 *  Created by tempbuffer on 8/7/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 */

#ifndef _GNET_CONNECTION_H_
#define _GNET_CONNECTION_H_

#include "gnet.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

namespace gnet
{
	class Connection
	{
	public:
		Connection() {}

		virtual void disconnect() = 0;
		virtual void connect(const std::string &host, const std::string &port) = 0;
		inline ItemBase* recv() { return _recvItem();}
		inline void send(ItemBase *item) { return _sendBytes(item->getItem()); }
		inline void sendEncodedWithRFA(ItemBase *item) { return _sendBytes(item->getRFAItem(recvRFA));}

	protected:
		virtual ItemBase* _recvItem();
		virtual ItemBase* _parseItem(Base::Raw &data);
		virtual void _sendBytes(const Bytes &bytes) {_sendRaw(reinterpret_cast<const char*>(&(const_cast<Bytes&>(bytes))[0]),bytes.size());}
		virtual Bytes& _recvBytes(size_t size) = 0;
		virtual void _sendRaw(const char* data, size_t size) = 0;
		void handshake();

	protected:
		Bytes mBuffer;
		Base::Codec::RFA sendRFA;
		Base::Codec::RFA recvRFA;
	};

	class TCPConnection : public Connection
	{
	public:
		TCPConnection();
		~TCPConnection();

		void connect(const std::string &host, const std::string &port);
		void disconnect();

	protected:
		void _sendRaw(const char* data, size_t size);
		Bytes& _recvBytes(size_t size);

	private:
		SOCKET mSocket;
	};
}

#endif