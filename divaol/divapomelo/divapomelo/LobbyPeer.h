#ifndef _DIVAPOMELO_LOBBYPEER_H_
#define _DIVAPOMELO_LOBBYPEER_H_

#include "PreDefines.h"

namespace divapomelo
{
	using namespace PomeloCpp;

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

			request(RequestCode[PUSH_LOBBY_GETSTAGELIST],
				Json::Object(),
				[&](RequestReq& req, int status, Json::Value resp) {
					peer->notify(req.route(), PUSH_LOBBY_GETSTAGELIST, &resp, status);
			});
		}

	protected:
		virtual void registerEventCallback() {
			on(EventCode[ON_ENTER], Base::Bind(this, &LobbyPeer::onEnter));
			on(EventCode[ON_LEAVE], Base::Bind(this, &LobbyPeer::onLeave));
		}

	private:
		void onEnter(MessageReq& req) {
			notify(req.route(), ON_ENTER, req.msg());
		}

		void onLeave(MessageReq& req) {
			notify(req.route(), ON_LEAVE, req.msg());
		}
	};
}

#endif