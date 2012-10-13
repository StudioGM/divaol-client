/*
 *  DivaChatClient.h
 *
 *  Created by tempbuffer on 9/29/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 */

#ifndef DIVA_CHAT_CLIENT
#define DIVA_CHAT_CLIENT

#include "DivaClient.h"

namespace divanet
{
	class ChatClient : public Client, public Base::Singleton<ChatClient>
	{
	public:
		enum NotifyType{NOTIFY_CHAT_AUTH=0x80,NOTIFY_CHAT_JOIN,NOTIFY_CHAT_LEAVE,NOTIFY_CHAT_CREATE,NOTIFY_CHAT_MSG};

		virtual std::string name() const {return "chat";}

		void login() {
			GNET_RECEIVE_REGISTER(mNetSys,"chat#auth_response",&ChatClient::gnet_login);
			mNetSys->send("chat#login","%S%B",NET_INFO.uid,NET_INFO.token);
		}

		void logout() {
			if(isLogin()) {
				mNetSys->send("chat#logout");
				mNetSys->refresh();
				_setLogin(false);

				GNET_RECEIVE_UNREGISTER(mNetSys,"chat#closehat");
				GNET_RECEIVE_UNREGISTER(mNetSys,"chat#receivemsg");
			}
		}

		void enter(const std::string &room) {
			GNET_RECEIVE_REGISTER(mNetSys,"chat#join_response",&ChatClient::gnet_enter);
			mNetSys->send("chat#enter","%S",room);
		}

		void leave(const std::string &room) {
			GNET_RECEIVE_REGISTER(mNetSys,"chat#leave_ok",&ChatClient::gnet_leave_ok);
			mNetSys->send("chat#leave","%S",room);
		}

		bool check(const std::string room) {
			for(int i = 0; i < mRooms.size(); i++)
				if(mRooms[i]==room)
					return true;
			return false;
		}

		void send(const std::string &room, const Base::String &content) {
			mNetSys->send("chat#sendmsg","%S%W",room,content.asUnicode());
		}

		void sendTo(const std::string &uid, const Base::String &content) {
			if(uid==NET_INFO.uid)
				return;
			std::string room = uid+"_private_room";
			enter(room);
			mNetSys->send("chat#sendmsg","%S%W",room,content.asUnicode());
			leave(room);
		}

	public:
		//gnet callback
		void gnet_login(GPacket *packet) {
			if (packet->getItem(2)->getString() == "ok") {
				_setLogin(true);
				_defaultRoom();
			}

			notify(packet->getItem(2)->getString(), NOTIFY_CHAT_AUTH);

			GNET_RECEIVE_UNREGISTER(mNetSys,"chat#auth_response");

			GNET_RECEIVE_REGISTER(mNetSys,"chat#closehat",&ChatClient::gnet_close);
			GNET_RECEIVE_REGISTER(mNetSys,"chat#receivemsg",&ChatClient::gnet_msg);
		}

		void gnet_create(GPacket *packet) {
			notify(packet->getItem(2)->getString(), NOTIFY_CHAT_CREATE, packet);

			GNET_RECEIVE_UNREGISTER(mNetSys,"chat#create");
		}

		void gnet_enter(GPacket *packet) {
			if (packet->getItem(2)->getString() == "ok") {
				mRooms.push_back(packet->getItem(3)->getString());
			}	
			
			notify(packet->getItem(2)->getString(), NOTIFY_CHAT_JOIN, packet);
			
			GNET_RECEIVE_UNREGISTER(mNetSys,"chat#join_response");
		}

		void gnet_close(GPacket *packet) {
			std::string room = packet->getItem(2)->getString();

			for(std::vector<std::string>::iterator ptr = mRooms.begin(); ptr != mRooms.end(); ptr++)
				if(room==*ptr) {
					mRooms.erase(ptr);
					break;
				}

			notify(room, NOTIFY_CHAT_LEAVE, packet);
		}

		void gnet_leave_ok(GPacket *packet) {
			gnet_close(packet);
		}

		void gnet_msg(GPacket *packet) {
			notify(packet->getItem(4)->getString(), NOTIFY_CHAT_MSG, packet);
		}
	private:
		void _defaultRoom() {
			enter("global");
			enter("region");
			_create(NET_INFO.uid+"_private_room");
			enter(NET_INFO.uid+"_private_room");
		}

		void _create(const std::string &room) {
			GNET_RECEIVE_REGISTER(mNetSys,"chat#create",&ChatClient::gnet_create);
			mNetSys->send("chat#create","%S",room);
		}

	protected:
		friend class Base::Singleton<ChatClient>;

		ChatClient(){}
		~ChatClient() {logout();}

	private:
		std::vector<std::string> mRooms;
	};

#define CHAT_CLIENT (divanet::ChatClient::instance())
}

#endif