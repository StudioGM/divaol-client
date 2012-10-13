/*
 *  DivaAuthClient.h
 *
 *  Created by tempbuffer on 9/27/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 */

#ifndef DIVA_AUTH_CLIENT
#define DIVA_AUTH_CLIENT

#include "DivaClient.h"

namespace divanet
{
	class AuthClient : public Client, public Base::Singleton<AuthClient>
	{
	public:
		enum NotifyType{NOTIFY_AUTH_REPLAY=0x80,NOTIFY_AUTH_REQUEST};

		virtual std::string name() const {return "auth";}

		void login(const std::string &username, const std::string &password) {
			GNET_RECEIVE_REGISTER(mNetSys,"auth#login",&AuthClient::gnet_login);
			mNetSys->send("auth#login","%S%S",username,password);
		}

		void logout() {
			if(isLogin()) {
				mNetSys->send("auth#logout");
				mNetSys->refresh();
				_setLogin(false);
			}
		}

		void request(const std::string &uid) {
			GNET_RECEIVE_REGISTER(mNetSys,"auth#response",&AuthClient::gnet_response);
			mNetSys->send("auth#request","%S",uid);
		}

	public:
		//gnet callback
		void gnet_login(GPacket *packet) {
			if (packet->getItem(2)->getString() == "ok")
			{
				_setLogin(true);
				NET_INFO.login = true;
				NET_INFO.uid = packet->getItem(4)->getString();
				NET_INFO.token = packet->getItem(5)->as<gnet::Item<gnet::Binary>>()->getRaw();
				NET_INFO.username =  Base::String(packet->getItem(3)->getString());
				NET_INFO.nickname = NET_INFO.username;
			}

			notify(packet->getItem(2)->getString(), NOTIFY_AUTH_REPLAY, packet);

			GNET_RECEIVE_UNREGISTER(mNetSys,"auth#login");
		}

		void gnet_response(GPacket *packet) {
			notify(packet->getItem(2)->getString(), NOTIFY_AUTH_REQUEST, packet);
			GNET_RECEIVE_UNREGISTER(mNetSys,"auth#response");
		}

	protected:
		friend class Base::Singleton<AuthClient>;

		AuthClient() {}
		~AuthClient() {logout();}

	private:
	};

#define AUTH_CLIENT (divanet::AuthClient::instance())
}

#endif