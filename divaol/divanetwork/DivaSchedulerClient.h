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
		Base::String ownerNickname;
		uint64 CreationTime;
		uint32 serverId;
		uint32 capacity;
		uint32 playernum;
		uint32 songId;
		uint32 level;
		uint32 mode;
		int32 state;
	};
	typedef std::vector<RoomInfo> RoomInfos;

#if defined(DIVA_GNET_OPEN) && !defined(DIVA_USE_POMELO)
	class SchedulerClient : public Client, public Base::Singleton<SchedulerClient>
	{
	public:
		enum NotifyType{NOTIFY_UPDATE_ROOMLIST=0x80, NOTIFY_REQUEST_ROOMNUM};

		virtual std::string name() const {return "scheduler";}

		void login() {
			isRequireing = false;
			GNET_RECEIVE_REGISTER(mNetSys,"scheduler#response",&SchedulerClient::gnet_response);
		}

		void logout() {
			GNET_RECEIVE_UNREGISTER(mNetSys,"scheduler#response");
		}

		void updateRoomList() {
			//if (!isRequireing) {
				isRequireing = true;
				mNetSys->send("scheduler#roomlist");
			//}
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
			isRequireing = false;

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
		SchedulerClient(){isRequireing = false;}
		~SchedulerClient() {}
		friend class Base::Singleton<SchedulerClient>;

		

	private:
		bool isRequireing;
		RoomInfos infos;
	};

#define SCHEDULER_CLIENT (divanet::SchedulerClient::instance())
#endif
}

#endif