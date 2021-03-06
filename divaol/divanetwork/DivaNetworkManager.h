/*
 *  DivaNetworkManager.h
 *
 *  Created by tempbuffer on 9/14/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 */

//#define DIVA_USE_GNET
#define DIVA_USE_POMELO
#if defined(DIVA_USE_GNET) && defined(DIVA_USE_POMELO)
#error "Can not use multiple network peer"
#endif

#ifndef DIVA_NETWORK_MANAGER
#define DIVA_NETWORK_MANAGER

#include "DivaNetInfo.h"
#include "DivaGNetSystem.h"
#include "Lib/Base/Ptr.h"
#include "Lib/Base/Pattern/Singleton.h"
#include "SoraAutoUpdate.h"
#include "DivaAuthClient.h"
#include "DivaChatClient.h"
#include "DivaSchedulerClient.h"
#include "DivaStageServer.h"

namespace divanet
{
#if defined(DIVA_USE_GNET)
	class NetworkManager : public Base::Singleton<NetworkManager>, public sora::SoraAutoUpdate
	{
	public:
		inline NetworkPtr core() {return mCore;}
		void setCore(NetworkPtr ptr) {mCore = ptr;}

		void connectCore() {
			mCore->setHostInfo(info["core"].ip,info["core"].port);
			mCore->connect();
		}

		virtual void init() {
			getServiceInfo();
		}

		virtual void getServiceInfo() {
			info["core"] = ServiceInfo("divaolserver00.gmgate.net",9899);
		}

		void onUpdate(float dt) {
			if(mCore&&mCore->isConnected()) {
				mCore->update(dt);
			}
		}

	protected:
		friend class Base::Singleton<NetworkManager>;

		NetworkManager() {}
		~NetworkManager() {}

	private:
		uint32 mStatus;
		bool mIsConnect;
		ServiceInfos info;

		NetworkPtr mCore;
		NetworkPtr mScheduler;
	};

#define NET_MANAGER (divanet::NetworkManager::instance())
#endif
}

#endif