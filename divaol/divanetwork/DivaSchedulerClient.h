/*
 *  DivaSchedulerClient.h
 *
 *  Created by tempbuffer on 9/30/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 */

#ifndef DIVA_SCHEDULER_CLIENT
#define DIVA_SCHEDULER_CLIENT

#include "DivaClient.h"

namespace divanet
{
	struct RoomInfo {
		enum {STAGE,GAME,CHECKOUT};

		std::string ownerId;
<<<<<<< HEAD
=======
		Base::String ownerNickname;
>>>>>>> 8fac03783867a4916e28db1e466348ee4dc2cf87
		uint64 CreationTime;
		uint32 serverId;
		uint32 capacity;
		uint32 playernum;
<<<<<<< HEAD
		uint32 sondId;
=======
		uint32 songId;
		uint32 level;
		uint32 mode;
>>>>>>> 8fac03783867a4916e28db1e466348ee4dc2cf87
		int32 state;
	};
	typedef std::vector<RoomInfo> RoomInfos;

	class SchedulerClient : public Client, public Base::Singleton<SchedulerClient>
	{
	public:
		enum NotifyType{NOTIFY_UPDATE_ROOMLIST=0x80, NOTIFY_REQUEST_ROOMNUM};

		virtual std::string name() const {return "scheduler";}

		void login() {
<<<<<<< HEAD
=======
			isRequireing = false;
>>>>>>> 8fac03783867a4916e28db1e466348ee4dc2cf87
			GNET_RECEIVE_REGISTER(mNetSys,"scheduler#response",&SchedulerClient::gnet_response);
		}

		void logout() {
<<<<<<< HEAD
			GNET_RECEIVE_UNREGISTER(mNetSys,"scheduler#roomlist");
		}

		void updateRoomList() {
			mNetSys->send("scheduler#roomlist");
=======
			GNET_RECEIVE_UNREGISTER(mNetSys,"scheduler#response");
		}

		void updateRoomList() {
			//if (!isRequireing) {
				isRequireing = true;
				mNetSys->send("scheduler#roomlist");
			//}
>>>>>>> 8fac03783867a4916e28db1e466348ee4dc2cf87
		}

		void requestRoomNum() {
			mNetSys->send("scheduler#roomnum");
		}

		const RoomInfos& getRoomList() const {
			return infos;
		}

		void onUpdate(float dt) {

			Client::onUpdate(dt);
			if(state()==STATE_BREAK) {
				BASE_PER_PERIOD_BEGIN(dt, NET_INFO.RECONNECT_TIME);
				reconnect(Task(&SchedulerClient::_reconnect,this));
				notify("reconnect", NOTIFY_CONNECT);
				BASE_PER_PERIOD_END();
			}
		}

	public:
		void gnet_response(GPacket *packet) {
<<<<<<< HEAD
=======
			isRequireing = false;

>>>>>>> 8fac03783867a4916e28db1e466348ee4dc2cf87
			std::string type = packet->getItem(2)->getString();

			if(type=="roomlist")
			{
				infos.clear();
				gnet::Item<gnet::List> *list = packet->getItem(3)->as<gnet::Item<gnet::List>>();
				for(int i = 0; i < list->size(); i++)
				{
					gnet::Item<gnet::Tuple> *item = list->getItem(i)->as<gnet::Item<gnet::Tuple>>();
					gnet::Item<gnet::List> *roomInfo = item->getItem(3)->as<gnet::Item<gnet::List>>();

					RoomInfo info;
					info.ownerId = item->getItem(0)->getString();
					info.CreationTime = item->getItem(1)->getInt();
					info.serverId = item->getItem(2)->getInt();
					info.capacity = roomInfo->getItem(0)->getInt();
<<<<<<< HEAD
					info.state = roomInfo->getItem(1)->getString()=="stage"?RoomInfo::STAGE:("game"?RoomInfo::GAME:RoomInfo::CHECKOUT);
					info.sondId = roomInfo->getItem(2)->getInt();
=======
					info.ownerNickname = Base::String::unEscape(roomInfo->getItem(4)->getString());
					if (info.ownerNickname.size() == 0)
						continue;
					info.state = roomInfo->getItem(1)->getString()=="stage"?RoomInfo::STAGE:("game"?RoomInfo::GAME:RoomInfo::CHECKOUT);
					gnet::Item<gnet::List> *songList = roomInfo->getItem(2)->as<gnet::Item<gnet::List>>();
					if(songList == 0 || songList->size() == 0)
						info.songId = 0;
					else
					{
						gnet::Item<gnet::Tuple> *firstItem = songList->getItem(0)->as<gnet::Item<gnet::Tuple>>();
						info.songId = firstItem->getItem(0)->getInt();
						info.level = firstItem->getItem(1)->getInt();
						info.mode = firstItem->getItem(2)->getInt();
					}
>>>>>>> 8fac03783867a4916e28db1e466348ee4dc2cf87
					info.playernum = roomInfo->getItem(3)->getInt();

					infos.push_back(info);
				}

				notify("update", NOTIFY_UPDATE_ROOMLIST, packet);
			}
			else if(type=="roomnum") {
				int num = packet->getItem(3)->getInt();

				notify("reqeust", NOTIFY_REQUEST_ROOMNUM, packet, num);
			}
		} 

	private:
		void _reconnect() {
			connect_thread();
		}
	protected:
<<<<<<< HEAD
		SchedulerClient(){}
		virtual ~SchedulerClient() throw() {}
=======
		SchedulerClient(){isRequireing = false;}
		~SchedulerClient() {}
>>>>>>> 8fac03783867a4916e28db1e466348ee4dc2cf87
		friend class Base::Singleton<SchedulerClient>;

		

	private:
<<<<<<< HEAD
=======
		bool isRequireing;
>>>>>>> 8fac03783867a4916e28db1e466348ee4dc2cf87
		RoomInfos infos;
	};

#define SCHEDULER_CLIENT (divanet::SchedulerClient::instance())
}

#endif