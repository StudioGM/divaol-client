/*
 *  DivaNetworkManager.h
 *
 *  Created by tempbuffer on 9/14/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 */

#ifndef DIVA_NETWORK_MANAGER
#define DIVA_NETWORK_MANAGER

#include "DivaGNetSystem.h"
#include "Lib/Base/Ptr.h"
#include "Lib/Base/Singleton.h"

namespace divanet
{
	typedef Base::SharedPtr<GNetworkSystem> NetworkPtr;

	class NetworkManager : public Base::Singleton<NetworkManager>
	{
	public:
		enum{AUTH,LOBBY,STAGE,GAME};

		void setCoreNet(NetworkPtr networkSystem) {
			mCoreNet = networkSystem;
		}

		inline NetworkPtr getCoreNet() {return mCoreNet;}

	protected:
		friend class Base::Singleton<NetworkManager>;

		NetworkManager():mStatus(AUTH),mIsConnect(false) {}
		~NetworkManager() {}

	private:
		uint32 mStatus;
		bool mIsConnect;
		NetworkPtr mCoreNet;
	};

#define NET_MANAGER (divanet::NetworkManager::instance())
}

#endif