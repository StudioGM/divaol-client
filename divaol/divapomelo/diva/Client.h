#ifndef _DIVAPOMELO_CLIENT_H_
#define _DIVAPOMELO_CLIENT_H_

#include "PeerBase.h"
#include "ChatPeer.h"
#include "LobbyPeer.h"
#include "StagePeer.h"
#include "GamePeer.h"
#include "Lib/CSHA1/SHA1.h"
#include "SoraAutoUpdate.h"

namespace divapomelo {
	class Client : public Base::Singleton<Client>, public sora::SoraAutoUpdate
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

		void connect() {
			peer.onState(EventCode[ON_CLOSE], Base::Bind(this, &Client::onClose));
			peer.onState(EventCode[ON_CONNECT], Base::Bind(this, &Client::onConnect));
			peer.onState(EventCode[ON_LOGIN], Base::Bind(this, &Client::onLogin));

			peer.clearComp();
			registerComp();
			peer.connect();
		}

		void login(Base::String username, Base::String password) {
			if (!peer.isConnect()) {
				BASE_LOGGER.log_error("not connect to server!");
				return;
			}

			CSHA1 sha1;
			string passwd = password;
			wstring encryptPW;
			sha1.Update((UINT_8*)passwd.c_str(), passwd.size() * sizeof(char));
			sha1.Final();
			sha1.ReportHashStl(encryptPW, CSHA1::REPORT_HEX_SHORT);

			peer.login(username.lower(), Base::String(encryptPW).lower());
		}
		void disconnect() {
			peer.disconnect();
		}

		void attachObserver(Base::Observer ob) {
			peer.attachObserver(ob);
		}

		void onUpdate(float dt) {
			peer.logic();
		}
		void logic() {
			peer.logic();
		}

		bool isLogin() const  {return peer.isLogin();}

	private:
		void onClose(PeerBase*, int status, Json::Value msg) {
			peer.notify(EventCode[ON_CLOSE], ON_CLOSE, &msg, status);
		}

		void onConnect(PeerBase*, int status, Json::Value msg) {
			peer.notify(EventCode[ON_CONNECT], ON_CONNECT, &msg, status);
		}

		void onLogin(PeerBase*, int status, Json::Value msg) {
			peer.notify(EventCode[ON_LOGIN], ON_LOGIN, &msg, status);
		}
	};

#define POMELO_CLIENT divapomelo::Client::instance()
#define POMELO_USER_INFO divapomelo::Client::instance().getBase().getUserInfo()
#define POMELO_CHAT_PEER divapomelo::Client::instance().getChatPeer()
#define POMELO_LOBBY_PEER divapomelo::Client::instance().getLobbyPeer()
#define POMELO_STAGE_PEER divapomelo::Client::instance().getStagePeer()
#define POMELO_GAME_PEER divapomelo::Client::instance().getGamePeer()
}

#endif