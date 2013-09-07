#ifndef _DIVAPOMELO_STAGEPEER_H_
#define _DIVAPOMELO_STAGEPEER_H_

#include "PreDefines.h"
#include "LobbyPeer.h"
#include "divasongmgr/DivaMapManager.h"

namespace divapomelo
{
	using namespace PomeloCpp;

	struct WaiterInfo {
		enum State{LEAVE,READY,UNREADY};
		std::string uid;
		Base::String nickname;
		uint32 status;
		uint32 color;
		uint32 slot;
		WaiterInfo():uid("0") {}
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
		std::string ownerId;
		std::string id;
		std::string mode;
		SongList song;
		uint32 status;
		Waiters waiters;
	};

	class StagePeer : public PeerComp
	{
	public:
		enum StageState{OUTSIDE, STAGE, GAME};

		StagePeer(PeerBase *peer):PeerComp(peer) {
			state = OUTSIDE;
			isStage = isReady = isOwner = isGame = false;
		}

		/*
		 * Requests
		 */ 

		// admin
		void create() {
			if (isInStage()) {
				BASE_LOGGER.log_warning(__FUNCTION__ + std::string(" now in stage!"));
				return;
			}

			request(EventCode[PUSH_LOBBY_CREATESTAGE],
				Json::Object(),
				[&](RequestReq &req, int status, Json::Value resp) {
					if (status == 0) {
						isStage = true;
						isOwner = true;
						isGame = false;
						isReady = true;

						_refreshStageInfo(resp);
					}

					notify(req.route(), PUSH_LOBBY_CREATESTAGE, resp, status);
			});
		}
		void join(std::string stageId) {
			if (isInStage()) {
				BASE_LOGGER.log_warning(__FUNCTION__ + std::string(" now in stage!"));
				return;
			}

			request(EventCode[PUSH_LOBBY_JOINSTAGE],
				Json::Object("stageId", stageId),
				[&](RequestReq &req, int status, Json::Value resp) {
					if (status == 0) {
						isStage = true;
						isOwner = false;
						isGame = false;
						isReady = false;

						_refreshStageInfo(resp);
					}

					notify(req.route(), PUSH_LOBBY_JOINSTAGE, resp, status);
			});
		}
		void leave() {
			if (!isInStage()) {
				BASE_LOGGER.log_warning(__FUNCTION__ + std::string(" not in stage!"));
				return;
			}

			request(EventCode[PUSH_LOBBY_LEAVESTAGE],
				Json::Object(),
				[&](RequestReq &req, int status, Json::Value resp) {
					if (status == 0) {
						isStage = false;
						isReady = isOwner = false;
						state = OUTSIDE;
					}

					notify(req.route(), PUSH_LOBBY_LEAVESTAGE, resp, status);
			});
		}
		// owner request
		void kick(std::string uid) {
			if (!isInStage()) {
				BASE_LOGGER.log_warning(__FUNCTION__ + std::string(" not in stage!"));
				return;
			}
			if (getState() == STAGE && owner()) {
				request(EventCode[PUSH_STAGE_KICK],
					Json::Object(
						"uid", uid
					),
					defaultRequestHandler);
			}
		}
		void setSong(const SongList &songList) {
			if (!isInStage()) {
				BASE_LOGGER.log_warning(__FUNCTION__ + std::string(" not in stage!"));
				return;
			}
			if (getState() == STAGE && owner()) {
				Json::Value list(Json::arrayValue);

				for (int i = 0; i < songList.size(); i++) {
					Json::Value song(Json::objectValue);
					song["id"] = songList[i].songId;
					song["level"] = songList[i].level;
					song["mode"] = songList[i].mode;
					list.append(song);
				}

				request(EventCode[PUSH_STAGE_SETSONG],
					Json::Object(
						"song", list
					),
					defaultRequestHandler);
			}
		}
		void setMode(const std::string &mode) {
			if (!isInStage()) {
				BASE_LOGGER.log_warning(__FUNCTION__ + std::string(" not in stage!"));
				return;
			}
			if (getState() == STAGE && owner()) {
				request(EventCode[PUSH_STAGE_SETMODE],
					Json::Object(
						"mode", mode
					),
					defaultRequestHandler);
			}
		}
		void setHooks(int64 hooks) {
			if (!isInStage()) {
				BASE_LOGGER.log_warning(__FUNCTION__ + std::string(" not in stage!"));
				return;
			}
			if (getState() == STAGE && owner()) {
				request(EventCode[PUSH_STAGE_SETHOOK],
					Json::Object(
					"hook", Base::String::any2string(hooks).asAnsi()
					),
					defaultRequestHandler);
			}
		}
		bool start() {
			if (!isInStage()) {
				BASE_LOGGER.log_warning(__FUNCTION__ + std::string(" not in stage!"));
				return false;
			}
			if (getState() == STAGE && owner()) {
				request(EventCode[PUSH_STAGE_START],
					Json::Object(),
					[&](RequestReq &req, int status, Json::Value resp) {
						notify(req.route(), PUSH_STAGE_START, resp, status);
				});
				return true;
			}
			return false;
		}
		// user request
		void draw(uint32 color) {
			if (!isInStage()) {
				BASE_LOGGER.log_warning(__FUNCTION__ + std::string(" not in stage!"));
				return;
			}
			if (owner() || !getIsReady()) {
				request(EventCode[PUSH_STAGE_DRAW],
					Json::Object(
						"color", color
					),
					defaultRequestHandler);
			}
		}
		void ready() {
			if (!isInStage()) {
				BASE_LOGGER.log_warning(__FUNCTION__ + std::string(" not in stage!"));
				return;
			}
			if (getState() == STAGE && !owner() && myInfo().status == WaiterInfo::UNREADY) {
				request(EventCode[PUSH_STAGE_READY],
					Json::Object(
						"flag", true
					),
					defaultRequestHandler);
			}
		}
		void unready() {
			if (!isInStage()) {
				BASE_LOGGER.log_warning(__FUNCTION__ + std::string(" not in stage!"));
				return;
			}
			if (getState() == STAGE && !owner() && myInfo().status == WaiterInfo::READY) {
				request(EventCode[PUSH_STAGE_READY],
					Json::Object(
						"flag", false
					),
					defaultRequestHandler);
			}
		}
		void back() {
			if (!isPlaying()) {
				BASE_LOGGER.log_warning(__FUNCTION__ + std::string(" not playing!"));
				return;
			}

			request(EventCode[PUSH_GAME_BACK],
				Json::Object(),
				[&](RequestReq &req, int status, Json::Value resp) {
					if (status == 0) {
						isGame = false;
						state = STAGE;
					}
					notify(req.route(), PUSH_GAME_BACK, resp, status);
			});
		}
		void returnToStage(std::string reason) {
			//notify("return", ON_STAGE_RETURN, NULL, 0);
		}

	private:
				/*
		 * Message CBs
		 */
		void onSetSong(MessageReq& req) {
			if (getState() != STAGE)
				return;

			Json::Value& msg = req.msg();
			SongList newSong = _parseSongList(msg["song"]);

			if (!_compareSongList(newSong, info.song)) {
				info.song = newSong;
				notify(req.route(), ON_STAGE_SETSONG, msg);
			}
		}
		void onSetMode(MessageReq& req) {
			if (getState() != STAGE)
				return;

			Json::Value& msg = req.msg();
			info.mode = msg["mode"].asString();

			notify(req.route(), ON_STAGE_SETMODE, msg);
		}
		void onSetHook(MessageReq& req) {
			if (getState() != STAGE)
				return;

			Json::Value& msg = req.msg();
			info.hooks = Base::String::string2any<int64>(msg["hook"].asString());

			notify(req.route(), ON_STAGE_SETHOOK, msg);
		}
		void onDraw(MessageReq& req) {
			if(getState()!=STAGE)
				return;

			Json::Value& msg = req.msg();
			int index = _findPlayer(msg["player"]["uid"].asString());
			if (index < 0) return;
			info.waiters[index].color = msg["color"].asInt();

			notify(req.route(), ON_STAGE_DRAW, msg);
		}
		void onReady(MessageReq& req) {
			if(getState()!=STAGE)
				return;

			Json::Value& msg = req.msg();
			int index = _findPlayer(msg["player"]["uid"].asString());
			if (index < 0) return;
			info.waiters[index].status = WaiterInfo::READY;

			if(isMe(index))
				isReady = true;

			notify(req.route(), ON_STAGE_READY, msg);
		}
		void onUnready(MessageReq& req) {
			if(getState()!=STAGE)
				return;

			Json::Value& msg = req.msg();
			int index = _findPlayer(msg["player"]["uid"].asString());
			if (index < 0) return;
			info.waiters[index].status = WaiterInfo::UNREADY;

			if(isMe(index))
				isReady = false;

			notify(req.route(), ON_STAGE_UNREADY, msg);
		}
		void onJoin(MessageReq& req) {
			if(getState()!=STAGE)
				return;

			Json::Value& msg = req.msg();
			int index = msg["slot"].asInt();
			info.waiters[index].uid = msg["player"]["uid"].asString();
			info.waiters[index].color = 0;
			info.waiters[index].nickname = Base::String::unEscape(msg["player"]["nickname"].asString());
			info.waiters[index].status = WaiterInfo::UNREADY;

			notify(req.route(), ON_STAGE_JOIN, msg);
		}
		void onLeave(MessageReq& req) {
			if(getState()!=STAGE)
				return;

			Json::Value& msg = req.msg();
			int index = msg["slot"].asInt();
			info.waiters[index].uid = "0";
			info.waiters[index].color = 0;
			info.waiters[index].nickname = "";
			info.waiters[index].status = WaiterInfo::LEAVE;

			notify(req.route(), ON_STAGE_LEAVE, msg);
		}
		void onClose(MessageReq& req) {
			isStage = false;
			isReady = isOwner = false;
			state = OUTSIDE;
			notify(req.route(), ON_STAGE_CLOSE, req.msg());
		}
		void onAllInfo(MessageReq& req) {
			Json::Value &msg = req.msg();

			_refreshStageInfo(msg);
			notify(EventCode[ON_STAGE_ALLINFO], ON_STAGE_ALLINFO, msg);
		}
		void onStart(MessageReq& req) {
			Json::Value &msg = req.msg();
			if (msg["flag"].asBool()) {
				state = GAME;	
				isGame = true;
			}
			notify(req.route(), ON_STAGE_START, msg);
		}
		void onReturn(MessageReq& req) {
			if (getState() == GAME)
				state = STAGE;
			if (isGame)
				isGame = false;

			Json::Value &msg = req.msg();
			if (getState() == STAGE) {
				_refreshStageInfo(msg["info"]);

				if (msg["phase"] == "over") {
					if (owner())
					{
						if(info.song.size()>0)
						{
							// remove the first song(just play)
							info.song.erase(info.song.begin());
							// update map manager
							MAPMGR.SelectedMap_Clear();
							for(int i = 0; i < info.song.size(); i++)
								MAPMGR.SelectedMap_Add(info.song[i].songId, static_cast<divamap::DivaMap::LevelType>(info.song[i].level), static_cast<divamap::DivaMap::ModeType>(info.song[i].mode));
							// notify ui
							notify(divapomelo::EventCode[divapomelo::LOCAL_UPDATE_SONG_UI], divapomelo::LOCAL_UPDATE_SONG_UI, Json::Value());
							// send msg to server
							setSong(info.song);
						}
					}
				}
			
				notify(req.route(), ON_STAGE_RETURN, msg);
			}
		}
		void onKick(MessageReq& req) {
			if(getState()!=STAGE)
				return;

			notify(req.route(), ON_STAGE_KICK, req.msg());
		}

	public:
		/*
		 * Gets & infos
		 */
		bool isInStage() const {return isStage;}
		bool isPlaying() const {return isGame;}
		bool owner() const {return isOwner;}
		int getState() const {return state;}
		bool getIsReady() const {return isReady;}
		bool isMe(int index) {return index==myIndex;}
		int getPlayerNum() const {
			int count = 0;
			for(Waiters::const_iterator ptr = info.waiters.begin(); ptr != info.waiters.end(); ptr++)
				if(ptr->status != WaiterInfo::LEAVE)
					count++;
			return count;
		}
		const WaiterInfo& myInfo() const {return info.waiters[myIndex];}
		const WaiterInfo& waiterInfo(std::string uid) const {
			for(Waiters::const_iterator ptr = info.waiters.begin(); ptr != info.waiters.end(); ptr++)
				if(ptr->uid == uid)
					return *ptr;
			static WaiterInfo null;
			return null;
		}
		const StageInfo& getInfo() const {return info;}
		const std::string &getRoomID() const {return info.id;}

		void refreshMusic() {
			if(state==STAGE) {
				if(owner()) {
					SongList songList;
					for(int i = 0; i < MAPMGR.GetSelectedMaps().size(); i++)
						songList.push_back(SongInfo(MAPMGR.GetSelectedMaps()[i].id, MAPMGR.GetSelectedMaps()[i].level, MAPMGR.GetSelectedMaps()[i].mode));
					setSong(songList);
				}
				else {
					MAPMGR.SelectedMap_Clear();
					for(int i = 0; i < info.song.size(); i++)
						MAPMGR.SelectedMap_Add(info.song[i].songId, static_cast<divamap::DivaMap::LevelType>(info.song[i].level), static_cast<divamap::DivaMap::ModeType>(info.song[i].mode));
				}
			}
		}

	private:
		/*
		 * Utilities
		 */
		void _refreshStageInfo(Json::Value msg) {
			cout << Json::FastWriter().write(msg) << endl;
			info.id = msg["id"].asString();
			info.ownerId = msg["owner"]["uid"].asString();
			info.capacity = msg["capacity"].asInt();
			info.song = _parseSongList(msg["data"]["song"]);
			info.mode = msg["data"]["mode"].asString();
			info.status = (msg["stat"].asInt() == STAGE_STAT_IDLE)?StageInfo::STAGE:StageInfo::GAME;
			info.hooks = Base::String::string2any<int64>(msg["data"]["hook"].asString());

			info.waiters.clear();
			for (int i = 0; i < msg["slots"].size(); i++) {
				WaiterInfo waiter;

				Json::Value slot = msg["slots"][i];
				if (slot.isNull() || !slot.isObject()) {
					waiter.status = WaiterInfo::LEAVE;
				}
				else {
					waiter.status = (slot["stat"].asInt() == STAGE_USER_STAT_READY)
						?WaiterInfo::READY:WaiterInfo::UNREADY;
					waiter.nickname = Base::String::unEscape(slot["player"]["nickname"].asString());
					waiter.uid = slot["player"]["uid"].asString();
					waiter.color = slot["color"].asInt();
					waiter.slot = slot["slot"].asInt();
					
					if (waiter.uid == getUserInfo().uid) {
						myIndex = i;
						isReady = waiter.status == WaiterInfo::READY;
					}
				}

				info.waiters.push_back(waiter);
			}

			state = info.status == StageInfo::STAGE?STAGE:GAME;
		}
		SongList _parseSongList(Json::Value song) {
			SongList newSong;
			for (int i = 0; i < song.size(); i++) {
				newSong.push_back(SongInfo(song[i]["id"].asInt(), song[i]["level"].asInt(), song[i]["mode"].asInt()));
			}
			return newSong;
		}
		bool _compareSongList(const SongList &a, const SongList &b)
		{
			if(a.size()!=b.size())
				return false;
			for(int i = 0; i < a.size(); i++)
				if(a[i].level!=b[i].level||a[i].songId!=b[i].songId||a[i].mode!=b[i].mode)
					return false;
			return true;
		}
		int32 _findPlayer(std::string uid) {
			for(int i = 0; i < info.waiters.size(); i++)
				if(info.waiters[i].uid == uid) {
					return i;
				}
			BASE_LOGGER.log_error("player "+Base::String::any2string(uid)+" not found");
			return -1;
		}

	protected:
		virtual void registerEventCallback() {
			on(EventCode[ON_STAGE_SETSONG], Base::Bind(this, &StagePeer::onSetSong));
			on(EventCode[ON_STAGE_SETMODE], Base::Bind(this, &StagePeer::onSetMode));
			on(EventCode[ON_STAGE_SETHOOK], Base::Bind(this, &StagePeer::onSetHook));
			on(EventCode[ON_STAGE_DRAW], Base::Bind(this, &StagePeer::onDraw));
			on(EventCode[ON_STAGE_READY], Base::Bind(this, &StagePeer::onReady));
			on(EventCode[ON_STAGE_UNREADY], Base::Bind(this, &StagePeer::onUnready));
			on(EventCode[ON_STAGE_JOIN], Base::Bind(this, &StagePeer::onJoin));
			on(EventCode[ON_STAGE_LEAVE], Base::Bind(this, &StagePeer::onLeave));
			on(EventCode[ON_STAGE_CLOSE], Base::Bind(this, &StagePeer::onClose));
			on(EventCode[ON_STAGE_ALLINFO], Base::Bind(this, &StagePeer::onAllInfo));
			on(EventCode[ON_STAGE_START], Base::Bind(this, &StagePeer::onStart));
			on(EventCode[ON_STAGE_RETURN], Base::Bind(this, &StagePeer::onReturn));
			on(EventCode[ON_STAGE_KICK], Base::Bind(this, &StagePeer::onKick));
		}

	private:
		int myIndex;
		int state;
		bool isStage;
		bool isGame;
		bool isOwner;
		bool isReady;

		StageInfo info;
		uint32 stage;
		String roomId;
	};
}

#endif