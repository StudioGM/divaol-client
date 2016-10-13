#ifndef _DIVAPOMELO_LOBBYPEER_H_
#define _DIVAPOMELO_LOBBYPEER_H_

#include "PreDefines.h"

namespace divapomelo
{
	using namespace PomeloCpp;

	struct RoomInfo {
		enum {STAGE,GAME};

		std::string ownerId;
		std::string stageId;
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

	class LobbyPeer : public PeerComp
	{
	public:
		LobbyPeer(PeerBase *peer):PeerComp(peer) {
		}

		void getStageList() {
			if (!isLogin()) {
				BASE_LOGGER.log_warning(__FUNCTION__ + std::string(" not login!"));
				return;
			}

			request(EventCode[PUSH_LOBBY_GETSTAGELIST],
				Json::Object(),
				[&](RequestReq& req, int status, Json::Value resp) {
					_refreshStageList(resp);

					peer->notify(req.route(), PUSH_LOBBY_GETSTAGELIST, &resp, status);
			});
		}

		RoomInfos& getRoomList() {return infos;}

	protected:
		virtual void registerEventCallback() {
			on(EventCode[ON_ENTER], Base::Bind(this, &LobbyPeer::onEnter));
			on(EventCode[ON_LEAVE], Base::Bind(this, &LobbyPeer::onLeave));
		}

		void _refreshStageList(Json::Value &msg) {
			infos.clear();

			for (int i = 0; i < msg.size(); i++) {
				RoomInfo info;
				Json::Value &item = msg[i];

				info.stageId = item["id"].asString();
				info.ownerId = item["owner"]["uid"].asString();
				info.ownerNickname = Base::String::unEscape(item["owner"]["nickname"].asString());
				info.capacity = item["capacity"].asInt();
				info.CreationTime = item["createTime"].asInt();
				info.serverId = 0;
				
				info.state = (item["stat"].asInt() == STAGE_STAT_IDLE)?RoomInfo::STAGE:RoomInfo::GAME;
				if (item["data"]["song"].isArray() && item["data"]["song"].size() > 0) {
					Json::Value &firstItem = item["data"]["song"][Json::Value::UInt(0)];
					info.songId = firstItem["id"].asInt();
					info.level = firstItem["level"].asInt();
					info.mode = firstItem["mode"].asInt();
				}
				else
					info.songId = 0;
				info.playernum = item["count"].asInt();

				infos.push_back(info);
			}
		}

	private:
		void onEnter(MessageReq& req) {
			notify(req.route(), ON_ENTER, req.msg());
		}

		void onLeave(MessageReq& req) {
			notify(req.route(), ON_LEAVE, req.msg());
		}

	private:
		bool isRequireing;
		RoomInfos infos;
	};
}

#endif