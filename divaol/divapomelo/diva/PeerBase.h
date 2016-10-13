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
		typedef Base::Function<void(PeerBase*, int status, Json::Value msg)> MessageCB;
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
		void disconnect() {
			_close();
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

		// Utility
		void onState(std::string route, MessageCB cb) {
			if (route == EventCode[ON_CLOSE])
				onCloseCb = cb;
			else if (route == EventCode[ON_LOGIN])
				onLoginCb = cb;
			else if (route == EventCode[ON_CONNECT])
				onConnectCb = cb;
		}
		void on(std::string route, Client::message_cb cb) {
			client->on(route, cb);
		}
		void remove(std::string route) {
			client->remove(route);
		}
		void remove(std::string route, Client::message_cb cb) {
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
		bool isConnecting() const {
			return status == CONNECTED || status == GATE;
		}
		bool isIdle() const {return status == IDLE;}
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
					onConnectCb(this, -1, NULL);
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
				onConnectCb(this, status, NULL);
		}

		void onClose(MessageReq& req) {
			_close();
		}

		void onLogin(RequestReq& req, int status, Json::Value resp) {
			if (status == 0) {
				BASE_LOGGER.log("[PeerBase] login successful");
				this->status = LOGIN;

				userInfo.username = resp["username"].asString();
				userInfo.nickname = Base::String::unEscape(resp["nickname"].asString());
				userInfo.uid = resp["uid"].asString();

				islogin = true;

				if (onLoginCb)
					onLoginCb(this, 0, resp);
			}
			else {
				BASE_LOGGER.log("[PeerBase] login unknown error");
				_close();

				if (onLoginCb)
					onLoginCb(this, status, resp);
			}
		}

		void _close() {
			if (islogin || client->isConnected()) {
				BASE_LOGGER.log("[PeerBase] close");
				islogin = false;
				status = IDLE;
				client->disConnect();

				peerComps.clear();

				if (onCloseCb)
					onCloseCb(this, 0, NULL);
			}
		}
	};
}

#endif