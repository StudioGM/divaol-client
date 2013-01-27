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
#include "divasongmgr/DivaMapManager.h"
#include "divanetwork/DivaChatClient.h"

namespace divanet
{
	struct WaiterInfo {
		enum State{LEAVE,READY,UNREADY};
		std::string uid;
		Base::String nickname;
		uint32 status;
		uint32 color;
		uint32 slot;
	};
	struct SongInfo {
		int songId;
		int level;
		int mode;
		SongInfo():songId(0),level(0),mode(0){}
		SongInfo(int songId, int level, int mode):songId(songId),level(level),mode(mode) {}
	};
	typedef std::vector<SongInfo> SongList;
	typedef std::vector<WaiterInfo> Waiters;
	struct StageInfo {
		enum State{STAGE,GAME};
		uint32 capacity;
		int64 hooks;
		std::string owner;
		std::string mode;
		SongList songId;
		uint32 status;
		Waiters waiters;
	};

	class StageClient : public Client, public Base::Singleton<StageClient>
	{
	public:
		enum StageState{OUTSIDE,GETTING_INFO,STAGE,GAME};
		enum NotifyType{NOTIFY_STAGE_JOIN_RESPONSE = 0x80,
						NOTIFY_STAGE_LEAVE_RESPONSE,
						NOTIFY_STAGE_START,
						NOTIFY_UPDATE_INFO,
						NOTIFY_UPDATE_COLOR,
						NOTIFY_UPDATE_SONG,
						NOTIFY_REFRESH_SONG_UI,
						NOTIFY_UPDATE_MODE,
						NOTIFY_UPDATE_HOOK,
						NOTIFY_UPDATE_READY,
						NOTIFY_STAGE_JOIN,
						NOTIFY_STAGE_LEAVE,
						NOTIFY_STAGE_CLOSED,
						NOTIFY_STAGE_RETURN,
						NOTIFY_GAME_OVER};

		virtual std::string name() const {return "stage";}

		void login() {
			mNetSys->send("auth#setuid","%S",NET_INFO.uid);

			GNET_RECEIVE_REGISTER(mNetSys,"stage#start",&StageClient::gnet_start);
			GNET_RECEIVE_REGISTER(mNetSys,"stage#closed",&StageClient::gnet_closed);
			GNET_RECEIVE_REGISTER(mNetSys,"stage#start_failed",&StageClient::gnet_startfailed);
			GNET_RECEIVE_REGISTER(mNetSys,"stage#start_notify",&StageClient::gnet_startnotify);
			GNET_RECEIVE_REGISTER(mNetSys,"stage#join",&StageClient::gnet_join);
			GNET_RECEIVE_REGISTER(mNetSys,"stage#leave",&StageClient::gnet_leave);
			GNET_RECEIVE_REGISTER(mNetSys,"stage#info",&StageClient::gnet_info);
			GNET_RECEIVE_REGISTER(mNetSys,"stage#draw",&StageClient::gnet_draw);
			GNET_RECEIVE_REGISTER(mNetSys,"stage#setSong",&StageClient::gnet_setSong);
			GNET_RECEIVE_REGISTER(mNetSys,"stage#setMode",&StageClient::gnet_setMode);
			GNET_RECEIVE_REGISTER(mNetSys,"stage#setHooks",&StageClient::gnet_setHooks);
			GNET_RECEIVE_REGISTER(mNetSys,"stage#ready",&StageClient::gnet_ready);
			GNET_RECEIVE_REGISTER(mNetSys,"stage#unready",&StageClient::gnet_unready);
			GNET_RECEIVE_REGISTER(mNetSys,"stage#game_over",&StageClient::gnet_game_over);
		}

		void logout() {
			leave();
			GNET_RECEIVE_UNREGISTER(mNetSys,"stage#start");
			GNET_RECEIVE_UNREGISTER(mNetSys,"stage#closed");
			GNET_RECEIVE_UNREGISTER(mNetSys,"stage#start_failed");
			GNET_RECEIVE_UNREGISTER(mNetSys,"stage#start_notify");
			GNET_RECEIVE_UNREGISTER(mNetSys,"stage#join");
			GNET_RECEIVE_UNREGISTER(mNetSys,"stage#leave");
			GNET_RECEIVE_UNREGISTER(mNetSys,"stage#info");
			GNET_RECEIVE_UNREGISTER(mNetSys,"stage#draw");
			GNET_RECEIVE_UNREGISTER(mNetSys,"stage#setSong");
			GNET_RECEIVE_UNREGISTER(mNetSys,"stage#setMode");
			GNET_RECEIVE_UNREGISTER(mNetSys,"stage#ready");
			GNET_RECEIVE_UNREGISTER(mNetSys,"stage#unready");
			GNET_RECEIVE_UNREGISTER(mNetSys,"stage#game_over");
		}

		void create(int capacity) {
			GNET_RECEIVE_REGISTER(mNetSys,"stage#join_response",&StageClient::gnet_join_response);
			mNetSys->send("stage#create","%d",capacity);
			
			// create room chatroom
			CHAT_CLIENT._create(NET_INFO.uid+"_stage_room");
			CHAT_CLIENT.enter(NET_INFO.uid+"_stage_room");

			mRoomID = NET_INFO.uid;
		}

		void join(const std::string &roomId) {
			GNET_RECEIVE_REGISTER(mNetSys,"stage#join_response",&StageClient::gnet_join_response);
			mNetSys->send("stage#join","%S",roomId);

			CHAT_CLIENT.enter(roomId+"_stage_room");

			mRoomID = roomId;
		}

		void leave() {
			if(mState!=OUTSIDE) {
				GNET_RECEIVE_REGISTER(mNetSys,"stage#leave_response",&StageClient::gnet_leave_response);
				mNetSys->send("stage#leave");
			}
		}

		void draw(uint32 color) {
			if(mState==STAGE &&  (owner() || myInfo().status!=WaiterInfo::READY))
				mNetSys->send("stage#draw","%d",color);
		}

		void setSong(const SongList &songList) {
			if(mState==STAGE&&owner()) {
				divanet::GPacket *packet = new divanet::GPacket;
				divanet::ItemList *list = new divanet::ItemList;
				packet->appendAhead<gnet::Atom>(gnet::Atom("setSong"));
				packet->appendAhead<gnet::Atom>(gnet::Atom("stage"));
				packet->appendItem(list);
				for(int i = 0; i < songList.size(); i++)
					list->appendItem(gnet::ItemUtility::formatTuple("%d%d%d",songList[i].songId,songList[i].level,songList[i].mode));
				mNetSys->send(packet);
			}
		}

		void setMode(std::string mode) {
			if(mState==STAGE&&owner())
				mNetSys->send("stage#setMode","%S",mode);
		}

		void setHooks(int64 hooks) {
			if(mState==STAGE&&owner())
				mNetSys->send("stage#setHooks","%d",hooks);
		}

		void changeSlot(uint32 slot) {

		}

		void ready() {
			if(mState==STAGE&&!owner()) {
				int index = _findPlayer(NET_INFO.uid);
				if(mInfo.waiters[index-1].status==WaiterInfo::UNREADY)
					mNetSys->send("stage#ready");
			}
		}

		void unready() {
			if(mState==STAGE&&!owner()) {
				int index = _findPlayer(NET_INFO.uid);
				if(mInfo.waiters[index-1].status==WaiterInfo::READY)
					mNetSys->send("stage#unready");
			}
		}

		bool start() {
			if(mState==STAGE&&owner()) {
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

		void back() {
			mNetSys->send("game#back");
		}

		bool isReady() const {return mIsReady;}
		bool isMe(int index) const {return index==myIndex;}
		int getPlayerNum() const {
			int count = 0;
			for(Waiters::const_iterator ptr = mInfo.waiters.begin(); ptr != mInfo.waiters.end(); ptr++)
				if(ptr->status != WaiterInfo::LEAVE)
					count++;
			return count;
		}
		const WaiterInfo& myInfo() const {return mInfo.waiters[myIndex-1];}
		const WaiterInfo& waiterInfo(const std::string &uid) const {
			for(Waiters::const_iterator ptr = mInfo.waiters.begin(); ptr != mInfo.waiters.end(); ptr++)
				if(ptr->uid == uid)
					return *ptr;
			return WaiterInfo();
		}

		void refreshMusic() {
			if(mState==STAGE) {
				if(owner()) {
					SongList songList;
					for(int i = 0; i < MAPMGR.GetSelectedMaps().size(); i++)
						songList.push_back(SongInfo(MAPMGR.GetSelectedMaps()[i].id, MAPMGR.GetSelectedMaps()[i].level, MAPMGR.GetSelectedMaps()[i].mode));
					setSong(songList);
				}
				else {
					MAPMGR.SelectedMap_Clear();
					for(int i = 0; i < mInfo.songId.size(); i++)
						MAPMGR.SelectedMap_Add(mInfo.songId[i].songId, static_cast<divamap::DivaMap::LevelType>(mInfo.songId[i].level), static_cast<divamap::DivaMap::ModeType>(mInfo.songId[i].mode));
				}
			}
		}

		void returnToStage(const std::string &info) {
			notify(info, NOTIFY_STAGE_RETURN);
		}

		const StageInfo& info() const {return mInfo;}
		const Base::String &getRoomID() const {return mRoomID;}
		void onUpdate(float dt) {
			Client::onUpdate(dt);
			if(Client::state()==STATE_BREAK) {
				BASE_PER_PERIOD_BEGIN(dt, NET_INFO.RECONNECT_TIME);
				reconnect(Task(&StageClient::_reconnect,this));
				notify("reconnect", NOTIFY_CONNECT);
				BASE_PER_PERIOD_END();
			}
		}

	private:
		void gnet_closed(GPacket *packet) {
			_leaveStage();
			notify("closed",NOTIFY_STAGE_CLOSED,packet);
		}

		void gnet_join_response(GPacket *packet) {
			if(packet->getItem(2)->getString()=="ok") {
				mState = GETTING_INFO;
				if(packet->getItem(3)->getString()==NET_INFO.uid)
				{
					mIsOwner = true;
					mIsReady = true;
				}
				else
				{
					mIsOwner = false;
					mIsReady = false;
				}
			}

			notify(packet->getItem(2)->getString(), NOTIFY_STAGE_JOIN_RESPONSE, packet);

			GNET_RECEIVE_UNREGISTER(mNetSys,"stage#join_response");
		}

		void gnet_leave_response(GPacket *packet) {
			_leaveStage();
			notify(packet->getItem(2)->getString(), NOTIFY_STAGE_LEAVE_RESPONSE, packet);

			GNET_RECEIVE_UNREGISTER(mNetSys,"stage#leave_response");
		}

		void gnet_startfailed(GPacket *packet) {
			notify("failed", NOTIFY_STAGE_START, packet);
			mState = STAGE;
		}

		void gnet_startnotify(GPacket *packet) {
			notify("notify", NOTIFY_STAGE_START, packet);
			if(owner()) {
				Base::String info;
				bool rt = _checkStart(info);
				mNetSys->send("stage#start_checkout","%b%S",rt,info.asAnsi());
			}
		}

		void gnet_start(GPacket *packet) {
			notify("start", NOTIFY_STAGE_START, packet);
			mState = GAME;
		}

		void gnet_info(GPacket *packet) {
			GPacket *members = packet->getItem(2)->as<GPacket>();
			gnet::Item<gnet::Tuple> *stageInfo = packet->getItem(3)->as<gnet::Item<gnet::Tuple>>();

			mInfo.owner = stageInfo->getItem(0)->getString();
			mInfo.capacity = stageInfo->getItem(1)->getInt();
			mInfo.songId = _gnet_parse_songList(stageInfo->getItem(2)->as<divanet::ItemList>());
			mInfo.mode = stageInfo->getItem(3)->getString();
			mInfo.status = stageInfo->getItem(4)->getString()=="stage"?StageInfo::STAGE:StageInfo::GAME;
			mInfo.hooks = stageInfo->getItem(5)->getInt();

			mInfo.waiters.clear();
			for(int i = 0; i < members->size(); i++) {
				gnet::Item<gnet::Tuple> *single = members->getItem(i)->as<gnet::Item<gnet::Tuple>>();
				WaiterInfo waiter;
				std::string status = single->getItem(1)->getString();
				waiter.nickname = Base::String::unEscape(single->getItem(4)->getString());
 				if(status=="leave")
					waiter.status = WaiterInfo::LEAVE;
				else if(status=="unready")
					waiter.status = WaiterInfo::UNREADY;
				else if(status=="ready")
					waiter.status = WaiterInfo::READY;
				else
					DIVA_EXCEPTION_MODULE("unexpected status","StageServer");

				waiter.color = single->getItem(2)->getInt();
				waiter.slot = single->getItem(3)->getInt();
				if(waiter.status!=WaiterInfo::LEAVE)
					waiter.uid = single->getItem(0)->getString();
				else
					waiter.uid = "0";

				if(waiter.uid==NET_INFO.uid) {
					myIndex = i+1;
					mIsReady = waiter.status==WaiterInfo::READY;
				}

				mInfo.waiters.push_back(waiter);
			}

			if(mState==GETTING_INFO) {
				mState = STAGE;
			}
			else if(mState==GAME) {
				mState = STAGE;
			}

			notify("update", NOTIFY_UPDATE_INFO, packet);
		}

		void gnet_draw(GPacket *packet) {
			if(state()!=STAGE)
				return;

			int index = _findPlayer(packet->getItem(2)->getString());
			mInfo.waiters[index-1].color = packet->getItem(3)->getInt();

			notify("color", NOTIFY_UPDATE_COLOR, packet, index);
		}

		void gnet_setSong(GPacket *packet) {
			if(state()!=STAGE)
				return;

			divanet::SongList newSong = _gnet_parse_songList(packet->getItem(2)->as<divanet::ItemList>());

			if(!compare(newSong, mInfo.songId))
			{
				mInfo.songId = newSong;
				notify("song", NOTIFY_UPDATE_SONG, packet);
			}
		}

		void gnet_setMode(GPacket *packet) {
			if(state()!=STAGE)
				return;

			mInfo.mode = packet->getItem(2)->getString();

			notify(mInfo.mode, NOTIFY_UPDATE_MODE, packet);
		}

		void gnet_setHooks(GPacket *packet) {
			if(state()!=STAGE)
				return;

			mInfo.hooks = packet->getItem(2)->getInt();

			notify("updateHooks", NOTIFY_UPDATE_HOOK, packet);
		}

		void gnet_ready(GPacket *packet) {
			if(state()!=STAGE)
				return;

			int index = _findPlayer(packet->getItem(2)->getString());
			mInfo.waiters[index-1].status = WaiterInfo::READY;

			if(isMe(index))
				mIsReady = true;

			notify("ready", NOTIFY_UPDATE_READY, packet, index);
		}

		void gnet_unready(GPacket *packet) {
			if(state()!=STAGE)
				return;

			int index = _findPlayer(packet->getItem(2)->getString());
			mInfo.waiters[index-1].status = WaiterInfo::UNREADY;

			if(isMe(index))
				mIsReady = false;

			notify("unready", NOTIFY_UPDATE_READY, packet, index);
		}

		void gnet_join(GPacket *packet) {
			if(state()!=STAGE)
				return;

			int index = packet->getItem(4)->getInt();
			mInfo.waiters[index-1].uid = packet->getItem(2)->getString();
			mInfo.waiters[index-1].color = 0;
			mInfo.waiters[index-1].nickname = Base::String::unEscape(packet->getItem(3)->getString());

			mInfo.waiters[index-1].status = WaiterInfo::UNREADY;

			notify("join", NOTIFY_STAGE_JOIN, packet, index);
		}

		void gnet_leave(GPacket *packet) {
			if(state()!=STAGE)
			{
				int index = _findPlayer(packet->getItem(2)->getString());
				mInfo.waiters[index-1].status = WaiterInfo::LEAVE;
				return;
			}

			int index = _findPlayer(packet->getItem(2)->getString());
			mInfo.waiters[index-1].uid = "0";
			mInfo.waiters[index-1].color = 0;

			mInfo.waiters[index-1].status = WaiterInfo::LEAVE;

			notify("leave", NOTIFY_STAGE_LEAVE, packet, index);
		}

		void gnet_game_over(GPacket *packet)
		{
			if(mState==GAME) {
				mState = STAGE;
			}

			if (owner())
			{
				if(mInfo.songId.size()>0)
				{
					// remove the first song(just play)
					mInfo.songId.erase(mInfo.songId.begin());
					// update map manager
					MAPMGR.SelectedMap_Clear();
					for(int i = 0; i < mInfo.songId.size(); i++)
						MAPMGR.SelectedMap_Add(mInfo.songId[i].songId, static_cast<divamap::DivaMap::LevelType>(mInfo.songId[i].level), static_cast<divamap::DivaMap::ModeType>(mInfo.songId[i].mode));
					// notify ui
					notify("song", NOTIFY_REFRESH_SONG_UI, NULL);
					// send msg to server
					setSong(mInfo.songId);
				}
			}
			notify("song", NOTIFY_GAME_OVER, packet);
		}

	private:
		void _leaveStage() {
			mState = OUTSIDE;

			CHAT_CLIENT.leave(mRoomID+"_stage_room");

			if(owner())
				CHAT_CLIENT._close(mRoomID+"_stage_room");
		}
		divanet::SongList _gnet_parse_songList(divanet::ItemList *list) {
			divanet::SongList newSong;
			for(int i = 0; i < list->size(); i++)
			{
				divanet::GPacket *songItem = list->getItem(i)->as<divanet::GPacket>();
				newSong.push_back(SongInfo(songItem->getItem(0)->getInt(), songItem->getItem(1)->getInt(), songItem->getItem(2)->getInt()));
			}

			return newSong;
		}
		int32 _findPlayer(const std::string &uid) {
			for(int i = 0; i < mInfo.waiters.size(); i++)
				if(mInfo.waiters[i].uid == uid) {
					return i+1;
				}
			DIVA_EXCEPTION_MODULE("player "+uid+" not found","StageServer");
		}

		bool _checkStart(Base::String &info) {
			if(!owner() || mInfo.waiters.size()==0)
				return false;
			if(mInfo.songId.size()==0) {
				info = "noselect";
				return false;
			}
			for(int i = 0; i < mInfo.waiters.size(); i++)
				if(mInfo.waiters[i].status==WaiterInfo::UNREADY) {
					info = "unready";
					return false;
				}
			std::map<int,int> colorCount;
			for(int i = 0; i < mInfo.waiters.size(); i++)
				if(mInfo.waiters[i].status==WaiterInfo::READY)
				{
					int color = mInfo.waiters[i].color;
					if(colorCount.find(color)==colorCount.end())
						colorCount[color] = 1;
					else
						colorCount[color]++;
				}	
			std::map<int,int>::iterator ptr = colorCount.begin();
			int tmp = (*ptr).second;
			while((++ptr) != colorCount.end())
				if(tmp != (*ptr).second)
				{
					info = "not match";
					return false;
				}

			if(mInfo.songId[0].mode == divamap::DivaMap::RelayMode) {
				for(int i = 0; i < colorCount.size(); i++)
					if(colorCount[i]<=1)
					{
						info = "not match";
						return false;
					}
			}
			else if(mInfo.songId[0].mode == divamap::DivaMap::PairMode) {
				for(int i = 0; i < colorCount.size(); i++)
					if(colorCount[i]!=2)
					{
						info = "not match";
						return false;
					}
			}

			info = "ok";
			return true;
		}

		void _reconnect() {
			if(connect_thread())
				login();
		}

		bool compare(const SongList &a, const SongList &b)
		{
			if(a.size()!=b.size())
				return false;
			for(int i = 0; i < a.size(); i++)
				if(a[i].level!=b[i].level||a[i].songId!=b[i].songId||a[i].mode!=b[i].mode)
					return false;
			return true;
		}

	protected:
		friend class Base::Singleton<StageClient>;

		StageClient():mState(OUTSIDE),mIsOwner(false) {}
		~StageClient() {logout();}

	private:
		int myIndex;
		bool mIsOwner;
		bool mIsReady;
		StageInfo mInfo;
		uint32 mState;
		Base::String mRoomID;
	};

#define STAGE_CLIENT (divanet::StageClient::instance())
}

#endif