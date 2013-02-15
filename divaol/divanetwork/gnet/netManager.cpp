/*
 *  netManager.cpp
 *
 *  Created by tempbuffer on 8/8/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 */

#include "netManager.h"
#include "Lib/Base/Timer.h"
#include "core/DivaCore.h"

namespace gnet
{
	DWORD WINAPI NetManager::_RecvThread(LPVOID param)
	{
		NetManager *netManager = static_cast<NetManager*>(param);
		
		while(netManager->isRecv)
		{
			ItemBase *item = 0;
			try{
				item = netManager->mConnector.recv();
			}
			catch(...)
			{
				return 0;
			}
			if(item)
				netManager->mRecvQueue.push(item);
		}
	}
	
	DWORD WINAPI NetManager::_SendThread(LPVOID param)
	{
		NetManager *netManager = static_cast<NetManager*>(param);
		
		while(netManager->isSend)
		{
			if(netManager->mSendQueue.task_on())
			{
				ItemBase *item = netManager->mSendQueue.task_take();
				try
				{
					netManager->mConnector.sendEncodedWithRFA(item);
				}
				catch (...)
				{
					delete item;
					netManager->mSendQueue.task_done();
					return 0;
				}
				delete item;
				netManager->mSendQueue.task_done();
			}
			else
				Base::TimeUtil::mSleep(1);
		}
	}
}