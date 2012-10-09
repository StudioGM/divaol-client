/*
 *  DivaClient.h
 *
 *  Created by tempbuffer on 9/27/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 */

#ifndef DIVA_CLIENT
#define DIVA_CLIENT

#include "DivaNetInfo.h"
#include "DivaGNetSystem.h"
#include "Lib/Base/Base.h"
#include "Lib/Base/Pattern/Observer.h"
#include "SoraAutoUpdate.h"

namespace divanet
{
	using Base::Observer;
	using Base::Notification;

	class Client : public Base::ObserverHandler, public SoraAutoUpdate
	{
	public:
		enum {DISCONNECT,UNAUTH,AUTH};
		enum {NOTIFY_CONNECT,NOTIFY_DISCONNECT};

		bool isLogin() const {return mIsLogin;}
		bool isConnect() const {return mIsConnect;}
		uint32 state() const {return mState;}

		Client():mIsConnect(false) {}
		virtual ~Client() {
			if(isConnect())
				disconnect();
		}

		virtual std::string name() const = 0;
		virtual void setNetworkSystem(NetworkPtr ptr) {mNetSys = ptr;}
		NetworkPtr getSys() {return mNetSys;}
		void refresh() {mNetSys->refresh();}
		virtual bool connect() {
			mNetSys->setHostInfo(NET_INFO.server(name()).ip, NET_INFO.server(name()).port);
			try{
				mNetSys->connect();
				mIsConnect = true;
				notify("ok",NOTIFY_CONNECT);
			}
			catch(...)
			{
				notify("failed",NOTIFY_CONNECT);
				return false;
			}
			return true;
		}
		void disconnect() {
			mIsConnect = false;
			mNetSys->disconnect();
			notify("ok",NOTIFY_DISCONNECT);
		}

		virtual void onUpdate(float dt) {
			if(isConnect())
				mNetSys->update(dt);
		}

	protected:
		void _setLogin(bool login) {
			mIsLogin = login;
		}

	protected:
		bool mIsConnect;
		bool mIsLogin;
		uint32 mState;
		NetworkPtr mNetSys;
	};
}

#endif