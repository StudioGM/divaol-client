#ifndef _DIVAPOMELO_PEERCOMP_H_
#define _DIVAPOMELO_PEERCOMP_H_

#include "PreDefines.h"

namespace divapomelo
{
	using namespace PomeloCpp;

	class PeerComp
	{
		friend class PeerBase;
	protected:
		PeerBase* peer;
	public:
		PeerComp(PeerBase* peer):peer(peer) {
		}

	protected:
		virtual void registerEventCallback() {
		}

		static void defaultRequestHandler(RequestReq& req, int status, Json::Value resp) {
			if (status == 0)
				BASE_LOGGER.log("[PeerBase] " + req.route() + " send OK");
			else
				BASE_LOGGER.log("[PeerBase] " + req.route() + " send FAIL");
		}

		void request(string route, Json::Value msg, Client::request_cb cb);
		void on(std::string route, Client::message_cb cb);
		void notify(std::string route, uint32 code, Json::Value &msg, int status = 0);

		bool isLogin();

		UserInfo& getUserInfo();
		ServerInfo& getServerInfo();
	};
}

#endif