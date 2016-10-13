#ifndef _DIVAPOMELO_GAMEPEER_H_
#define _DIVAPOMELO_GAMEPEER_H_

#include "PreDefines.h"
#include "StagePeer.h"

namespace divapomelo
{
	using namespace PomeloCpp;

	class GamePeer : public PeerComp
	{
	public:
		GamePeer(PeerBase *peer, std::shared_ptr<StagePeer> stagePeer):PeerComp(peer),stagePeer(stagePeer) {
		}

		void on(std::string route, PomeloCpp::Client::message_cb cb) {
			PeerComp::on(route, cb);
		}
		void remove(std::string route) {
			PeerComp::remove(route);
		}
		void push(string route, Json::Value msg = Json::Object(), PomeloCpp::Client::request_cb cb = defaultRequestHandler) {
			PeerComp::request(route, msg, cb);
		}

		bool isPlaying() {stagePeer->isPlaying();}
		bool isStageExist() {stagePeer->isInStage();}

	protected:
		virtual void registerEventCallback() {
		}

	private:
		std::shared_ptr<StagePeer> stagePeer;
	};
}

#endif