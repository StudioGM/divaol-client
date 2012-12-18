/*
 *  connection.cpp
 *
 *  Created by tempbuffer on 8/7/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 */

#include "connection.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

namespace gnet
{
	ItemBase* Connection::_recvItem()
	{
		uint32 typeID = BinaryUtility::convertToHost<size_t>(_recvBytes(sizeof(size_t)));
		size_t size = 0;

		switch(typeID)
		{
		case GNET_TYPE_INT_8:
			return new Item<int8>(_recvBytes(sizeof(int8)));
		case GNET_TYPE_UINT_8:
			return new Item<uint8>(_recvBytes(sizeof(uint8)));
		case GNET_TYPE_INT_32:
			return new Item<int32>(_recvBytes(sizeof(int32)));
		case GNET_TYPE_UINT_32:
			return new Item<uint32>(_recvBytes(sizeof(uint32)));
		case GNET_TYPE_INT_64:
			return new Item<int64>(_recvBytes(sizeof(int64)));
		case GNET_TYPE_UINT_64:
			return new Item<uint64>(_recvBytes(sizeof(uint64)));
		case GNET_TYPE_BINARY:
			{
				Bytes tmpBytes = _recvBytes(sizeof(size_t));
				size = BinaryUtility::convertToHost<size_t>(tmpBytes);
				tmpBytes = tmpBytes+_recvBytes(size);
				return new Item<Binary>(tmpBytes);
			}
		case GNET_TYPE_ATOM:
			{
				Bytes tmpBytes = _recvBytes(sizeof(size_t));
				size = BinaryUtility::convertToHost<size_t>(tmpBytes);
				tmpBytes = tmpBytes+_recvBytes(size);
				return new Item<Atom>(tmpBytes);
			}
		case GNET_TYPE_TUPLE:
			{
				size = BinaryUtility::convertToHost<size_t>(_recvBytes(sizeof(size_t)));
				Item<Tuple> *item = new Item<Tuple>();
				for(size_t index = 0; index < size; index++)
					item->appendItem(recv());
				return item;
			}
		case GNET_TYPE_LIST:
			{
				size = BinaryUtility::convertToHost<size_t>(_recvBytes(sizeof(size_t)));
				Item<List> *item = new Item<List>();
				for(size_t index = 0; index < size; index++)
					item->appendItem(recv());
				return item;
			}
		case GNET_TYPE_RFA:
			{
				Base::Raw header = _recvBytes(8);
				uint32 length = sendRFA.decodeLength(header);
				uint32 recvLength = (length+3)/4*4;
				Base::Raw body = _recvBytes(recvLength);
				Base::Raw content = sendRFA.decode(header+body);
				content.seek(0);
				return _parseItem(content);
			}
		default:
			return 0;
		}
	}

	ItemBase* Connection::_parseItem(Base::Raw &data)
	{
		Base::Raw buffer = data.take(sizeof(uint32));
		uint32 typeID = BinaryUtility::convertToHost<size_t>(buffer.raw());
		size_t size = 0;

		switch(typeID)
		{
		case GNET_TYPE_INT_8:
			buffer = data.take(sizeof(int8));
			return new Item<int8>(buffer.raw());
		case GNET_TYPE_UINT_8:
			buffer = data.take(sizeof(uint8));
			return new Item<int8>(buffer.raw());
		case GNET_TYPE_INT_32:
			buffer = data.take(sizeof(int32));
			return new Item<int8>(buffer.raw());
		case GNET_TYPE_UINT_32:
			buffer = data.take(sizeof(uint32));
			return new Item<int8>(buffer.raw());
		case GNET_TYPE_INT_64:
			buffer = data.take(sizeof(int64));
			return new Item<int8>(buffer.raw());
		case GNET_TYPE_UINT_64:
			buffer = data.take(sizeof(uint64));
			return new Item<int8>(buffer.raw());
		case GNET_TYPE_BINARY:
			{
				Bytes tmpBytes = data.take(sizeof(size_t)).raw();
				size = BinaryUtility::convertToHost<size_t>(tmpBytes);
				tmpBytes = tmpBytes+data.take(size).raw();
				return new Item<Binary>(tmpBytes);
			}
		case GNET_TYPE_ATOM:
			{
				Bytes tmpBytes = data.take(sizeof(size_t)).raw();
				size = BinaryUtility::convertToHost<size_t>(tmpBytes);
				tmpBytes = tmpBytes+data.take(size).raw();
				return new Item<Atom>(tmpBytes);
			}
		case GNET_TYPE_TUPLE:
			{
				buffer = data.take(sizeof(size_t));
				size = BinaryUtility::convertToHost<size_t>(buffer.raw());
				Item<Tuple> *item = new Item<Tuple>();
				for(size_t index = 0; index < size; index++)
					item->appendItem(_parseItem(data));
				return item;
			}
		case GNET_TYPE_LIST:
			{
				buffer = data.take(sizeof(size_t));
				size = BinaryUtility::convertToHost<size_t>(buffer.raw());
				Item<List> *item = new Item<List>();
				for(size_t index = 0; index < size; index++)
					item->appendItem(_parseItem(data));
				return item;
			}
		case GNET_TYPE_RFA:
			{
				Base::Raw header = data.take(8);
				uint32 length = sendRFA.decodeLength(header);
				uint32 recvLength = (length+3)/4*4;
				Base::Raw body = data.take(recvLength);
				Base::Raw content = sendRFA.decode(header+body);
				return _parseItem(content);
			}
		default:
			return 0;
		}
	}

	void Connection::handshake()
	{
		_sendRaw("GNET\x00\x00\x00\x01",8);
		_recvBytes(2);
		if(mBuffer[0]!=byte('O')||mBuffer[1]!=byte('K'))
			throw "Handshake failed";
	}

	
	TCPConnection::TCPConnection(){
#ifdef _WIN32
		WSADATA wsaData;
		WORD	wVersionRequested;

		wVersionRequested = MAKEWORD(1,1);
		if(WSAStartup(wVersionRequested, &wsaData))
			throw "Error at WSAStartup!";
#endif
	}
	TCPConnection::~TCPConnection()
	{
		disconnect();
#ifdef _WIN32
		WSACleanup();
#endif
	}

	void TCPConnection::disconnect()
	{
		if(mSocket!=INVALID_SOCKET)
		{
			closesocket(mSocket);
			mSocket = INVALID_SOCKET;
		}
	}

	void TCPConnection::connect(const std::string &host, const std::string &port)
	{
		addrinfo *result = NULL, *ptr = NULL, addr;

		memset(&addr,0,sizeof(addr));
		addr.ai_family = AF_UNSPEC;
		addr.ai_socktype = SOCK_STREAM;
		addr.ai_protocol = IPPROTO_TCP;

		if(getaddrinfo(host.c_str(),port.c_str(),&addr,&result))
			throw "Getaddrinfo failed";

		mSocket = INVALID_SOCKET;

		mSocket = socket(result->ai_family,result->ai_socktype,result->ai_protocol);

		if(mSocket == INVALID_SOCKET)
			throw "Error at socket";

		if(::connect(mSocket,result->ai_addr,(int)result->ai_addrlen))
			throw "Error at connect";

		freeaddrinfo(result);

		if(mSocket == INVALID_SOCKET)
			throw "Error at socket";

		handshake();
	}
	void TCPConnection::_sendRaw(const char* data, size_t size)
	{
		size_t writeSize = 0;
		while(writeSize<size)
		{
			int retVal = ::send(mSocket,data,size,0);
			if(retVal == SOCKET_ERROR)
			{
				int errcode = WSAGetLastError();
				throw "Send Failed";
			}
			writeSize += retVal;
		}
	}
	Bytes& TCPConnection::_recvBytes(size_t size)
	{
		mBuffer = Bytes(size);
		size_t readSize = 0;
		while(readSize<size)
		{
			int retVal = ::recv(mSocket,reinterpret_cast<char*>(&mBuffer[0]),size-readSize,0);
			
			if (SOCKET_ERROR == retVal)
			{
				retVal = 0;
				//int errcode = WSAGetLastError();
				//throw "Recv Failed";
			}
			
			readSize += retVal;
		}
		return mBuffer;
	}

}