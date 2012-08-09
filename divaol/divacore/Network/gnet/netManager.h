/*
 *  netManager.h
 *
 *  Created by tempbuffer on 8/8/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 */

#ifndef _GNET_NET_MANAGER_H_
#define _GNET_NET_MANAGER_H_

#include "connection.h"
#include "Base/Thread/Queue.h"
#include "Base/Timer.h"

namespace gnet
{
	using Base::ThreadSafe::Queue;

	class NetManager
	{
	public:
		NetManager() {}
		~NetManager() {}

		void connect(const std::string &host, const std::string &port) {
			mConnector.connect(host,port);

			mRecvThreadHandle = CreateThread(NULL, 0, _RecvThread, (LPVOID)this, 0, 0);
			mSendThreadHandle = CreateThread(NULL, 0, _SendThread, (LPVOID)this, 0, 0);
		}
		void disconnect() {
			CloseHandle(mRecvThreadHandle);
			CloseHandle(mSendThreadHandle);

			clear(true, true);

			mConnector.disconnect();
		}

		void send(ItemBase* item) { mSendQueue.push(item); }
		ItemBase* recv() { return mRecvQueue.take(); }
		void waitNext() { while(mRecvQueue.empty()) Base::TimeUtil::mSleep(1); }
		ItemBase* lookAt() { return mRecvQueue.front(); }
		
		bool empty() const { return mRecvQueue.empty(); }
		size_t size() { return mRecvQueue.size(); }
		void clear(bool bRecv = true, bool bSend = false) { 
			if(bRecv) mRecvQueue.clear(); 
			if(bSend) mSendQueue.clear();
		}
		
		static DWORD WINAPI _RecvThread(LPVOID param);
		static DWORD WINAPI _SendThread(LPVOID param);

	protected:
		TCPConnection mConnector;
		Queue<ItemBase*> mSendQueue;
		Queue<ItemBase*> mRecvQueue;
		HANDLE mSendThreadHandle;
		HANDLE mRecvThreadHandle;
	};
}

#endif