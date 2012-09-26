/*
 *  DivaNetworkManager.h
 *
 *  Created by tempbuffer on 9/14/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 */

#define DIVA_GNET_OPEN

#ifndef DIVA_NETWORK_MANAGER
#define DIVA_NETWORK_MANAGER

#include "DivaGNetSystem.h"
#include "Lib/Base/Ptr.h"
#include "Lib/Base/Singleton.h"
#include "SoraCore.h"

namespace divanet
{
	typedef Base::SharedPtr<GNetworkSystem> NetworkPtr;
	
	struct ServiceInfo
	{
		Base::String ip;
		uint32 port;
		Base::String description;
			
		ServiceInfo(const Base::String &ip, uint32 port, Base::String description = "")
			:ip(ip),port(port),description(description) {}
		ServiceInfo() {}
	};
	typedef std::map<std::string,ServiceInfo> ServiceInfos;

	class NetworkManager : public Base::Singleton<NetworkManager>, public sora::SoraAutoUpdate
	{
	public:
		inline NetworkPtr core() {return mCore;}
		inline NetworkPtr auth() {return mAuth;}
		inline NetworkPtr chat() {return mChat;}
		inline NetworkPtr scheduler() {return mScheduler;}
		void setAuth(NetworkPtr ptr) {mAuth = ptr;}
		void setChat(NetworkPtr ptr) {mChat = ptr;}
		void setCore(NetworkPtr ptr) {mCore = ptr;}
		void setScheduler(NetworkPtr ptr) {mScheduler = ptr;}

		void connectAuth() {
			mAuth->setHostInfo(info["auth"].ip,info["auth"].port);
			mAuth->connect();
		}

		void connectChat() {
			mChat->setHostInfo(info["chat"].ip,info["chat"].port);
			mChat->connect();
		}

		void connectCore() {
			mCore->setHostInfo(info["core"].ip,info["core"].port);
			mCore->connect();
		}

		void connectScheduler() {
			mScheduler->setHostInfo(info["scheduler"].ip,info["scheduler"].port);
			mScheduler->connect();
		}

		virtual void init() {
			getServiceInfo();
		}

		virtual void getServiceInfo() {
			info["auth"] = ServiceInfo("58.215.170.145",9699);
			info["chat"] = ServiceInfo("58.215.170.145",9799);
			info["core"] = ServiceInfo("58.215.170.145",9899);
			info["scheduler"] = ServiceInfo("58.215.170.145",9099);
		}

		void onUpdate(float dt) {
			if(mAuth&&mAuth->isConnected()) {
				mAuth->update(dt);
				if(!mAuth->isConnected())
					mIsAuth = false;
			}
			if(mChat&&mChat->isConnected()) {
				mChat->update(dt);
				if(!mChat->isConnected())
					mIsChat = false;
			}
			if(mCore&&mCore->isConnected()) {
				mCore->update(dt);
			}
			if(mScheduler&&mScheduler->isConnected()) {
				mScheduler->update(dt);
			}
		}

	protected:
		friend class Base::Singleton<NetworkManager>;

		NetworkManager():mIsAuth(false) {}
		~NetworkManager() {}

	private:
		uint32 mStatus;
		bool mIsConnect;
		bool mIsAuth;
		bool mIsChat;
		ServiceInfos info;

		NetworkPtr mCore;
		NetworkPtr mAuth;
		NetworkPtr mChat;
		NetworkPtr mScheduler;
	};

#define NET_MANAGER (divanet::NetworkManager::instance())
}

#endif