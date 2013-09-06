#ifndef _DIVAPOMELO_PEERBASE_H_
#define _DIVAPOMELO_PEERBASE_H_

#include "PreDefines.h"
#include "PeerComp.h"
#include "GateRouter.h"
#include "../Lib/Base/Pattern/Observer.h"

namespace divapomelo
{
	using namespace PomeloCpp;

	class PeerBase : public Base::ObserverHandler
	{
		friend class PeerComp;
	public:
		typedef Base::Function<void(PeerBase*, int status)> MessageCB;
		typedef std::shared_ptr<PeerComp> PeerCompPtr;
		enum Status {IDLE, GATE, CONNECTING, CONNECTED, AUTH, LOGIN};

	private:
		bool islogin;
		int status;
		UserInfo userInfo;
		ServerInfo serverInfo;

		std::vector<PeerCompPtr> peerComps;
		std::shared_ptr<Client> client;
		GateRouter gateRouter;

		MessageCB onCloseCb;
		MessageCB onConnectCb;
		MessageCB onLoginCb;
		MessageCB onRegisterCb;

	public:
		PeerBase() : islogin(false), status(IDLE) {
			gateRouter.on(Bind(this, &PeerBase::onGate));
		}

		/*
		 * Connect & Login
		 */
		void connect() {
			if (status != IDLE)
			{
				BASE_LOGGER.log_warning("[PeerBase] Client not idle.");
				return;
			}

			status = GATE;
			if (!gateRouter.query(serverInfo.gateIp, serverInfo.gatePort))
			{
				status = IDLE;
			}
		}
		void connect(std::string host, int port) {
			if (status != IDLE && status != GATE)
			{
				BASE_LOGGER.log_warning("[PeerBase] Client not idle.");
				return;
			}

			client.reset(new Client(host, port));

			status = CONNECTING;

			_registerEventCallback();

			client->connect();
		}

		void login(Base::String username, Base::String password) {
			if (status != CONNECTED)
			{
				BASE_LOGGER.log_warning("[PeerBase] now can not auth.");
				return;
			}

			status = AUTH;
			client->request("connector.connectorHandler.login", 
				Json::Object("username", username.asAnsi(), "password", password.asAnsi()), 
				Base::Bind(this, &PeerBase::onLogin));
		}

		void logic() {
			if (status == GATE)
				gateRouter.logic();
			if (status != IDLE && client)
				client->logic();
		}
		void work(double time = 1.0) {
			client->work(time);
		}

		//void getStageList() {
		//	client->request("lobby.lobbyHandler.getStageList", 
		//		Json::Object(), 
		//		[](RequestReq& req, int status, Json::Value resp) {
		//			cout << Json::FastWriter().write(resp) << endl;
		//		});
		//}

		//void createStage() {
		//	client->request("lobby.lobbyHandler.createStage", 
		//		Json::Object(), 
		//		[](RequestReq& req, int status, Json::Value resp) {
		//			cout << "Stage Create" << endl;
		//		});
		//}

		//void leaveStage() {
		//	client->request("lobby.lobbyHandler.leaveStage",
		//		Json::Object(),
		//		[](RequestReq& req, int status, Json::Value resp) {
		//			if (status == 0)
		//				cout << "Stage Leave OK" << endl;
		//			else
		//				cout << "Stage Leave Fail" << endl;
		//		});
		//}

		//void joinStage(std::string id) {
		//	client->request("lobby.lobbyHandler.joinStage",
		//		Json::Object("stageId", id),
		//		[](RequestReq& req, int status, Json::Value resp) {
		//			if (status == 0)
		//				cout << "Join Successful!" << endl;
		//			else
		//				cout << "Join Failed " + ((status==1)?resp.asString():std::string("")) << endl;
		//		});
		//}

		//void kick(int uid) {
		//	client->request("lobby.lobbyHandler.kick",
		//		Json::Object("uid", uid),
		//		[](RequestReq& req, int status, Json::Value resp) {
		//			if (status == 0) {
		//				if (resp["code"].asInt() == 200)
		//					cout << "Kick Successful!" << endl;
		//				else
		//					cout << "Kick Failed " + resp.asString() << endl;
		//			}
		//		});
		//}

		//void getStageAllInfo() {
		//	client->request("lobby.stageHandler.getAllInfo",
		//		Json::Object(),
		//		[](RequestReq& req, int status, Json::Value resp) {
		//			cout << Json::FastWriter().write(resp) << endl;
		//		});
		//}

		//void linkToGame() {
		//	client->request("game.gameHandler.link",
		//		Json::Object(),
		//		[](RequestReq& req, int status, Json::Value resp) {
		//			if (status == 0)
		//				cout << "Link to game OK, info: " << endl << Json::FastWriter().write(resp) << endl;
		//			else
		//				cout << "Link to game FAIL" << endl;
		//		});
		//}

		//void back() {
		//	client->request("game.gameHandler.back",
		//		Json::Object(),
		//		[](RequestReq& req, int status, Json::Value resp) {
		//			if (status == 0)
		//				cout << "Back to stage OK" << endl;
		//			else
		//				cout << "Back to stage FAIL" << endl;
		//		});
		//}

		//void draw(int color) {
		//	client->request("lobby.stageHandler.draw",
		//		Json::Object("color", color),
		//		[](RequestReq& req, int status, Json::Value resp) {
		//			if (status == 0)
		//				cout << Json::FastWriter().write(resp) << endl;
		//		});
		//}

		//void ready(bool flag) {
		//	client->request("lobby.stageHandler.ready",
		//		Json::Object("flag", flag),
		//		[](RequestReq& req, int status, Json::Value resp) {
		//			if (status == 0)
		//				cout << "Ready OK" << endl;
		//	});
		//}

		//void start() {
		//	client->request("lobby.stageHandler.start",
		//		Json::Object(),
		//		[](RequestReq& req, int status, Json::Value resp) {
		//			if (status == 0)
		//				cout << "Start OK" << endl;
		//	});
		//}

		//void getGameInfos() {
		//	client->request("game.gameHandler.currentInfo",
		//		Json::Object(),
		//		[](RequestReq& req, int status, Json::Value resp) {
		//			if (status == 0)
		//				cout << Json::FastWriter().write(resp) << endl;
		//	});
		//}

		// Utility
		void on(std::string route, MessageCB cb) {
			if (route == "close")
				onCloseCb = cb;
			else if (route == "login")
				onLoginCb = cb;
			else if (route == "connect")
				onConnectCb = cb;
		}
		void on(std::string route, Client::message_cb cb) {
			client->on(route, cb);
		}
		void removeMessage(std::string route, Client::message_cb cb) {
			client->remove(route, cb);
		}
		void request(string route, Json::Value msg, Client::request_cb cb) {
			client->request(route, msg, cb);
		}
		
		/*
		 * Get & Set
		 */
		int getStatus() const {return status;}
		std::shared_ptr<Client> getClient() { return client; }
		ServerInfo& getServerInfo() {return serverInfo;}
		UserInfo& getUserInfo() {return userInfo;}
		bool isConnect() const {
			return status > CONNECTING;
		}
		bool isLogin() const {return islogin;}

		void registerComp(std::shared_ptr<PeerComp> peerComp) {
			peerComps.push_back(peerComp);
		}
		void clearComp() {
			peerComps.clear();
		}

	protected:
		void _registerEventCallback() {
			client->onclose(Base::Bind(this, &PeerBase::onClose));
			client->on(Base::Bind(this, &PeerBase::onConnect));

			for (PeerCompPtr comp : peerComps) {
				comp->registerEventCallback();
			}
			//client->on("onChat", Base::Bind(this, &PeerBase::onChat));
			//client->on("stage.start", Base::Bind(this, &PeerBase::onStart));
			//client->on("game.heartbeat", Base::Bind(this, &PeerBase::onHeartbeat));
		}

	private:
		/*
		 * common cb
		 */
		void onGate(GateRouter &router, int status) {
			if (status == 0) {
				serverInfo.connectorIp = router.getHost();
				serverInfo.connectorPort = router.getPort();
				
				connect(router.getHost(), router.getPort());
			}
			else {
				BASE_LOGGER.log("[PeerBase] connect error");
				this->status = IDLE;

				if (onConnectCb)
					onConnectCb(this, -1);
			}
		}

		void onConnect(ConnectReq& req, int status) {
			if (status == 0) {
				this->status = CONNECTED;
			}
			else {
				BASE_LOGGER.log("[PeerBase] connect error");
				this->status = IDLE;
			}

			if (onConnectCb)
				onConnectCb(this, status);
		}

		void onClose(MessageReq& req) {
			_close();
		}

		void onLogin(RequestReq& req, int status, Json::Value resp) {
			if (status == 0) {
				BASE_LOGGER.log("[PeerBase] login successful");
				this->status = LOGIN;

				userInfo.username = resp["username"].asString();
				userInfo.nickname = resp["nickname"].asString();
				userInfo.uid = resp["uid"].asInt();

				islogin = true;

				if (onLoginCb)
					onLoginCb(this, 0);
			}
			else {
				BASE_LOGGER.log("[PeerBase] login unknown error");
				_close();

				if (onLoginCb)
					onLoginCb(this, status);
			}
		}

		void _close() {
			if (islogin) {
				BASE_LOGGER.log("[PeerBase] close");
				islogin = false;
				status = IDLE;
				client->disConnect();

				peerComps.clear();

				if (onCloseCb)
					onCloseCb(this, 0);
			}
		}
	};
}

#endif