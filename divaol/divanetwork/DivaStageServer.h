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
	class StageClient : public Client, public Base::Singleton<StageClient>
	{
		enum StageState{OUTSIDE,STAGE,GAME};
	public:
		enum NotifyType{NOTIFY_STAGE_JOIN = 0x80,NOTIFY_STAGE_LEAVE,NOTIFY_STAGE_START};

		virtual std::string name() const {return "stage";}

		void login() {
			mNetSys->send("auth#setuid","%S",NET_INFO.uid);

			GNET_RECEIVE_REGISTER(mNetSys,"stage#start",&StageClient::gnet_start);
		}

		void logout() {
			GNET_RECEIVE_UNREGISTER(mNetSys,"stage#start");
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

	public:
		void gnet_join(GPacket *packet) {
			if(packet->getItem(2)->getString()=="ok") {
				mState = true;
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

	protected:
		friend class Base::Singleton<StageClient>;

		StageClient():mState(OUTSIDE),mIsOwner(false) {}
		~StageClient() {}

	private:
		bool mIsOwner;
		uint32 mState;
	};

#define STAGE_CLIENT (divanet::StageClient::instance())
}

#endif