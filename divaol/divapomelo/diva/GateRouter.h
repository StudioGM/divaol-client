#ifndef _DIVAPOMELO_GATEROUTER_H_
#define _DIVAPOMELO_GATEROUTER_H_

#include "..\PomeloCpp\PomeloCpp.h"
#include "NetInfo.h"

namespace divapomelo
{
	using namespace PomeloCpp;

	class GateRouter
	{
		typedef Base::Function<void(GateRouter&, int)> ResponseCb;

		std::shared_ptr<Client> client;
		bool querying;
		bool succeed;
		ResponseCb cb;

		int port;
		std::string host;

	public:
		GateRouter():querying(false),succeed(false) {}

		bool query(std::string ip, int port) {
			if (querying)
				return false;

			succeed = false;
			querying = true;
			// host ip and gate ip are same
			host = ip;
			client.reset(new Client(ip, port));
			client->onclose(Base::Bind(this, &GateRouter::onClose));
			client->on(Base::Bind(this, &GateRouter::onConnect));
			client->connect(true);
			return true;
		}
		void logic() {
			if (querying)
				client->logic();
		}
		void error() {
			clear();
			if (cb)
				cb(*this, -1);
		}
		void clear() {
			if (!querying)
				return;
			querying = false;
			client->disConnect();
		}
		void on(ResponseCb cb) {
			this->cb = cb;
		}

		bool isQuerying() const {return querying;}
		bool isSucceed() const {return succeed;}
		int getPort() const {return port;}
		std::string getHost() const {return host;}

	private:
		void onConnect(ConnectReq& req, int status)
		{
			if (status == 0) {
				client->request("gate.gateHandler.queryEntry", Json::Object("uid", Base::Random::RandomInt()), Base::Bind(this, &GateRouter::onQuery));
			}
			else {
				error();
			}
		}
		void onClose(MessageReq& req) {
			if (querying)
				error();
		}
		void onQuery(RequestReq& req, int status, Json::Value resp) {
			if (status == 0) {
				//host = resp["host"].asString();
				port = resp["port"].asInt();
				succeed = true;

				clear();
				if (cb)
					cb(*this, 0);
			}
			else
				error();
		}
	};
}

#endif