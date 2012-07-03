#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>

#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_PORT "20248"

/*
SockException
SockSystem的异常类
*/

class SockException
{
	std::string content;
public:        
	SockException(std::string content):content(content) {}
	std::string getContent() {return content;}
};

/*
SockSystem
简单封装winsock, 用C/S模式在两台机器间建立链接并传输数据
*/

class SockSystem
{
	friend class GameSock;
	static const int MAXSIZE = 1024;
	enum{WAIT,SERVER,CLIENT};

	std::string port;
	int SystemMode;
	SOCKET Socket;
	SOCKET ListenSocket;

public:
	SockSystem():port(DEFAULT_PORT),SystemMode(WAIT),Socket(INVALID_SOCKET),ListenSocket(INVALID_SOCKET) {}
	~SockSystem() {release();}
	static SockSystem& Instance() {static SockSystem instance; return instance;}
	void setPort(std::string port) {this->port = port;}
	void init()
	{
		WSADATA wsaData;
		if(WSAStartup(0x101, &wsaData))
			throw SockException("error at WSAStartup!");
	}
	void release()
	{
		releaseSocket();
		WSACleanup();
	}
	void releaseSocket()
	{
		SystemMode = WAIT;
		if(Socket!=INVALID_SOCKET)
			closesocket(Socket), Socket = INVALID_SOCKET;
		if(ListenSocket!=INVALID_SOCKET)
			closesocket(ListenSocket), ListenSocket = INVALID_SOCKET;
	}
	void sendBuffer(const char *buffer, int size)
	{
		if(SystemMode==WAIT)
			return;
			//throw SockException("no connection!");
		send(Socket,buffer,size,0);
	}
	void sendString(std::string buffer)
	{
		if(SystemMode==WAIT)
			return;
			//throw SockException("no connection!");
		send(Socket,buffer.data(),buffer.size(),0);
	}
	int recvBuffer(char *buffer, int maxSize = MAXSIZE)
	{
		if(SystemMode==WAIT)
			throw ("no connection!");
		memset(buffer,0,maxSize*sizeof(char));
		int size = recv(Socket,buffer,maxSize,0);
		if(size==SOCKET_ERROR)
		{
			releaseSocket();
			throw SockException("connection lost");
		}
		return size;
	}
	std::string recvString()
	{
		static char buffer[MAXSIZE];
		memset(buffer,0,sizeof(buffer));
		int size = recvBuffer(buffer);
		if(size==SOCKET_ERROR)
		{
			releaseSocket();
			throw SockException("connection lost!");
		}
		buffer[size] = 0;
		return buffer;
	}
	void serverInit()
	{
		releaseSocket();

		addrinfo *result = NULL, *ptr = NULL, init;
		memset(&init,0,sizeof(init));
		init.ai_family = AF_INET;
		init.ai_socktype = SOCK_STREAM;
		init.ai_protocol = IPPROTO_TCP;
		init.ai_flags = AI_PASSIVE;

		if(getaddrinfo(NULL,port.data(),&init,&result))
			throw SockException("getaddrinfo failed");

		ListenSocket = INVALID_SOCKET;

		ListenSocket = socket(result->ai_family,result->ai_socktype,result->ai_protocol);

		if(ListenSocket == INVALID_SOCKET)
			throw SockException("Error at socket()");

		if(bind(ListenSocket,result->ai_addr, (int)result->ai_addrlen))
		{
			printf("bind failed\n");
			freeaddrinfo(result);
			closesocket(ListenSocket);
			throw SockException("Error at bind()");
		}

		freeaddrinfo(result);

		if(listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
		{
			closesocket(ListenSocket);
			throw SockException("Listen failed with error");
		}

		Socket = INVALID_SOCKET;
		
		sockaddr_in addr;

		int addrlen = sizeof(addr);

		Socket = accept(ListenSocket, (sockaddr*)&addr, &addrlen);

		SystemMode = SERVER;

		closesocket(ListenSocket);
	}

	void clientInit(std::string serverIP = "127.0.0.1")
	{
		addrinfo *result = NULL, *ptr = NULL, addr;

		memset(&addr,0,sizeof(addr));
		addr.ai_family = AF_UNSPEC;
		addr.ai_socktype = SOCK_STREAM;
		addr.ai_protocol = IPPROTO_TCP;

		if(getaddrinfo(serverIP.data(),port.data(),&addr,&result))
			throw SockException("getaddrinfo failed");
		
		Socket = INVALID_SOCKET;

		Socket = socket(result->ai_family,result->ai_socktype,result->ai_protocol);

		if(Socket == INVALID_SOCKET)
			throw SockException("Error at socket()");
			
		if(connect(Socket,result->ai_addr,(int)result->ai_addrlen))
			throw SockException("Error at connect()");

		freeaddrinfo(result);

		if(Socket==INVALID_SOCKET)
			throw SockException("Unable to connect to server!");

		SystemMode = CLIENT;
	}
};
