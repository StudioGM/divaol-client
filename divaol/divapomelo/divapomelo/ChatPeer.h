#ifndef _DIVAPOMELO_CHATPEER_H_
#define _DIVAPOMELO_CHATPEER_H_

#include "PreDefines.h"

namespace divapomelo
{
	using namespace PomeloCpp;

	class ChatPeer : public PeerComp
	{
	public:
		ChatPeer(PeerBase *peer):PeerComp(peer) {
		}

		void sendTo(int uid, string content) {
			if (!isLogin()) {
				BASE_LOGGER.log_warning(__FUNCTION__ + std::string(" not login"));
				return;
			}

			request(EventCode[PUSH_CHAT_SEND],
				Json::Object(
					"content", content, 
					"scope", "private",
					"target", uid
				),
				PeerComp::defaultRequestHandler);
		}

		void sendInStage(string content) {
			if (!isLogin()) {
				BASE_LOGGER.log_warning(__FUNCTION__ + std::string(" not login"));
				return;
			}

			request(EventCode[PUSH_CHAT_SEND],
				Json::Object(
					"content", content,
					"scope", "stage"
				),
				PeerComp::defaultRequestHandler);
		}

		void sendGlobal(string content) {
			if (!isLogin()) {
				BASE_LOGGER.log_warning(__FUNCTION__ + std::string(" not login"));
				return;
			}

			request(RequestCode[PUSH_CHAT_SEND],
				Json::Object(
					"content", content,
					"scope", "global"
				),
				PeerComp::defaultRequestHandler);
		}

	protected:
		virtual void registerEventCallback() {
			on(EventCode[ON_CHAT], Base::Bind(this, &ChatPeer::onChat));
		}

	private:
		void onChat(MessageReq& req) {
			notify(req.route(), ON_CHAT, req.msg());
		}
	};
}

#endif