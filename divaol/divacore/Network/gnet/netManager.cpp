/*
 *  netManager.cpp
 *
 *  Created by tempbuffer on 8/8/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 */

#include "netManager.h"
#include "Base/Timer.h"

namespace gnet
{
	DWORD WINAPI NetManager::_RecvThread(LPVOID param)
	{
		NetManager *netManager = static_cast<NetManager*>(param);
		
		while(true)
			netManager->mRecvQueue.push(netManager->mConnector.recv());
	}
	
	DWORD WINAPI NetManager::_SendThread(LPVOID param)
	{
		NetManager *netManager = static_cast<NetManager*>(param);
		
		while(true)
		{
			if(!netManager->mSendQueue.empty())
			{
				ItemBase *item = netManager->mSendQueue.take();
				netManager->mConnector.send(item);
				delete item;
			}
			else
				Base::TimeUtil::mSleep(1);
		}
	}
}