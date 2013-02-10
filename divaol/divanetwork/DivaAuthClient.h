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
<<<<<<< HEAD
		enum NotifyType{NOTIFY_AUTH_REPLAY=0x80,NOTIFY_AUTH_REQUEST};
=======
		enum NotifyType{NOTIFY_AUTH_REPLAY=0x80,NOTIFY_AUTH_REQUEST,NOTIFY_AUTH_KICK};
>>>>>>> 8fac03783867a4916e28db1e466348ee4dc2cf87

		virtual std::string name() const {return "auth";}

		void login(const std::string &username, const std::string &password) {
			GNET_RECEIVE_REGISTER(mNetSys,"auth#login",&AuthClient::gnet_login);
<<<<<<< HEAD
			mNetSys->send("auth#login","%S%S",username.c_str(),password.c_str());
=======
			GNET_RECEIVE_REGISTER(mNetSys,"auth#kick",&AuthClient::gnet_kick);
			mNetSys->send("auth#login","%S%S",username,password);
>>>>>>> 8fac03783867a4916e28db1e466348ee4dc2cf87
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
<<<<<<< HEAD
			mNetSys->send("auth#request","%S",uid.c_str());
=======
			mNetSys->send("auth#request","%S",uid);
>>>>>>> 8fac03783867a4916e28db1e466348ee4dc2cf87
		}

		void onUpdate(float dt) {
			Client::onUpdate(dt);
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
				NET_INFO.nickname = Base::String::unEscape(Base::String(packet->getItem(6)->getString()));
			}
			   
			notify(packet->getItem(2)->getString(), NOTIFY_AUTH_REPLAY, packet);

			GNET_RECEIVE_UNREGISTER(mNetSys,"auth#login");
<<<<<<< HEAD
=======
			GNET_RECEIVE_UNREGISTER(mNetSys,"auth#kick");
>>>>>>> 8fac03783867a4916e28db1e466348ee4dc2cf87
		}

		void gnet_response(GPacket *packet) {
			notify(packet->getItem(2)->getString(), NOTIFY_AUTH_REQUEST, packet);
			GNET_RECEIVE_UNREGISTER(mNetSys,"auth#response");
		}

<<<<<<< HEAD
=======
		void gnet_kick(GPacket *packet) {
			notify("system", NOTIFY_AUTH_KICK, packet);
		}

>>>>>>> 8fac03783867a4916e28db1e466348ee4dc2cf87
	protected:
		friend class Base::Singleton<AuthClient>;

		AuthClient() {}
<<<<<<< HEAD
		virtual ~AuthClient() throw() {logout();}
=======
		~AuthClient() {logout();}
>>>>>>> 8fac03783867a4916e28db1e466348ee4dc2cf87

	private:
	};

#define AUTH_CLIENT (divanet::AuthClient::instance())
}

#endif