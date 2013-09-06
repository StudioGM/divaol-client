#ifndef _DIVAPOMELO_CLIENT_H_
#define _DIVAPOMELO_CLIENT_H_

#include "PeerBase.h"
#include "ChatPeer.h"
#include "LobbyPeer.h"
#include "StagePeer.h"
#include "GamePeer.h"
#include "Lib/CSHA1/SHA1.h"

namespace divapomelo {
	class Client : public Base::Singleton<Client>
	{
		PeerBase peer;
		std::shared_ptr<ChatPeer> chatPeer;
		std::shared_ptr<LobbyPeer> lobbyPeer;
		std::shared_ptr<StagePeer> stagePeer;
		std::shared_ptr<GamePeer> gamePeer;
	private:
		friend class Base::Singleton<Client>;

		void registerComp() {
			chatPeer.reset(new ChatPeer(&peer));
			lobbyPeer.reset(new LobbyPeer(&peer));
			stagePeer.reset(new StagePeer(&peer));
			gamePeer.reset(new GamePeer(&peer, stagePeer));

			peer.registerComp(chatPeer);
			peer.registerComp(lobbyPeer);
			peer.registerComp(stagePeer);
			peer.registerComp(gamePeer);
		}
	public:
		PeerBase& getBase() {return peer;}
		std::shared_ptr<ChatPeer> getChatPeer() {return chatPeer;}
		std::shared_ptr<LobbyPeer> getLobbyPeer() {return lobbyPeer;}
		std::shared_ptr<StagePeer> getStagePeer() {return stagePeer;}
		std::shared_ptr<GamePeer> getGamePeer() {return gamePeer;}

		void login(std::string username, std::string password) {
			peer.clearComp();
			registerComp();

			peer.connect();

			while (!peer.isConnect()) {
				peer.logic();
				Base::TimeUtil::mSleep(1);
			}

			CSHA1 sha1;
			string passwd = Base::String(password);
			wstring encryptPW;
			sha1.Update((UINT_8*)passwd.c_str(), passwd.size() * sizeof(char));
			sha1.Final();
			sha1.ReportHashStl(encryptPW, CSHA1::REPORT_HEX_SHORT);

			peer.login(username, Base::String(encryptPW).lower());
		}
	};

#define CLIENT divapomelo::Client::instance()
#define PEER_BASE divapomelo::Client::instance().getBase()
#define CHAT_PEER divapomelo::Client::instance().getChatPeer()
#define LOBBY_PEER divapomelo::Client::instance().getLobbyPeer()
#define STAGE_PEER divapomelo::Client::instance().getStagePeer()
#define GAME_PEER divapomelo::Client::instance().getGamePeer();
}

#endif