/*
 *  DivaStageClient.h
 *
 *  Created by tempbuffer on 10/1/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 */

#ifndef DIVA_STAGE_CLIENT
#define DIVA_STAGE_CLIENT

#include "DivaClient.h"

namespace divanet
{
	struct WaiterInfo {
		enum State{LEAVE,READY,UNREADY};
		std::string uid;
		uint32 status;
		uint32 color;
		uint32 slot;
	};
	typedef std::vector<WaiterInfo> Waiters;
	struct StageInfo {
		enum State{STAGE,GAME};
		uint32 capacity;
		std::string owner;
		std::string mode;
		uint32 songId;
		uint32 status;
		Waiters waiters;
	};

	class StageClient : public Client, public Base::Singleton<StageClient>
	{
		enum StageState{OUTSIDE,GETTING_INFO,STAGE,GAME};
	public:
		enum NotifyType{NOTIFY_STAGE_JOIN = 0x80,NOTIFY_STAGE_LEAVE,NOTIFY_STAGE_START,NOTIFY_UPDATE_INFO};

		virtual std::string name() const {return "stage";}

		void login() {
			mNetSys->send("auth#setuid","%S",NET_INFO.uid);

			GNET_RECEIVE_REGISTER(mNetSys,"stage#start",&StageClient::gnet_start);
			GNET_RECEIVE_REGISTER(mNetSys,"stage#info",&StageClient::gnet_info);
		}

		void logout() {
			GNET_RECEIVE_UNREGISTER(mNetSys,"stage#start");
			GNET_RECEIVE_UNREGISTER(mNetSys,"stage#info");
		}

		void create(int capacity) {
			GNET_RECEIVE_REGISTER(mNetSys,"stage#join",&StageClient::gnet_join);
			mNetSys->send("stage#create","%d",capacity);
		}

		void join(const std::string &roomId) {
			GNET_RECEIVE_REGISTER(mNetSys,"stage#join",&StageClient::gnet_join);
			mNetSys->send("stage#join","%S",roomId);
		}

		void leave() {
			if(mState!=OUTSIDE) {
				GNET_RECEIVE_REGISTER(mNetSys,"stage#leave",&StageClient::gnet_leave);
				mNetSys->send("stage#leave");
			}
		}

		void draw(uint32 color) {

		}

		void changeSlot(uint32 slot) {

		}

		bool start() {
			if(state()==STAGE&&owner()) {
				mNetSys->send("stage#start");
				return true;
			}
			return false;
		}

		uint32 state() const {
			return mState;
		}

		bool owner() const {
			return mIsOwner;
		}

		const StageInfo& info() const {return mInfo;}

	public:
		void gnet_join(GPacket *packet) {
			if(packet->getItem(2)->getString()=="ok") {
				mState = GETTING_INFO;
				if(packet->getItem(3)->getString()==NET_INFO.uid)
					mIsOwner = true;
			}

			notify(packet->getItem(2)->getString(), NOTIFY_STAGE_JOIN, packet);

			GNET_RECEIVE_UNREGISTER(mNetSys,"stage#join");
		}

		void gnet_leave(GPacket *packet) {
			notify(packet->getItem(2)->getString(), NOTIFY_STAGE_LEAVE, packet);

			GNET_RECEIVE_UNREGISTER(mNetSys,"stage#leave");
		}

		void gnet_start(GPacket *packet) {
			notify("start", NOTIFY_STAGE_START, packet);
		}

		void gnet_info(GPacket *packet) {
			GPacket *members = packet->getItem(2)->as<GPacket>();
			gnet::Item<gnet::Tuple> *stageInfo = packet->getItem(3)->as<gnet::Item<gnet::Tuple>>();

			mInfo.owner = stageInfo->getItem(0)->getString();
			mInfo.capacity = stageInfo->getItem(1)->getInt();
			mInfo.songId = stageInfo->getItem(2)->getInt();
			mInfo.mode = stageInfo->getItem(3)->getString();
			mInfo.status = stageInfo->getItem(4)->getString()=="STAGE"?StageInfo::STAGE:StageInfo::GAME;

			mInfo.waiters.clear();
			for(int i = 0; i < members->size(); i++) {
				gnet::Item<gnet::Tuple> *single = members->getItem(i)->as<gnet::Item<gnet::Tuple>>();
				WaiterInfo waiter;
				std::string status = single->getItem(1)->getString();
				if(status=="LEAVE")
					waiter.status = WaiterInfo::LEAVE;
				else if(status=="UNREADY")
					waiter.status = WaiterInfo::UNREADY;
				else if(status=="READY")
					waiter.status = WaiterInfo::READY;
				else
					DIVA_EXCEPTION_MESSAGE("unexpected status");

				waiter.color = single->getItem(2)->getInt();
				waiter.slot = single->getItem(3)->getInt();
				if(waiter.status!=WaiterInfo::LEAVE)
					waiter.uid = single->getItem(0)->getString();
				else
					waiter.uid = "0";

				mInfo.waiters.push_back(waiter);
			}

			if(mState==GETTING_INFO) {
				mState = STAGE;
			}
			else if(mState==STAGE) {

			}

			notify("update", NOTIFY_UPDATE_INFO, packet);
		}

	protected:
		friend class Base::Singleton<StageClient>;

		StageClient():mState(OUTSIDE),mIsOwner(false) {}
		~StageClient() {}

	private:
		bool mIsOwner;
		StageInfo mInfo;
		uint32 mState;
	};

#define STAGE_CLIENT (divanet::StageClient::instance())
}

#endif