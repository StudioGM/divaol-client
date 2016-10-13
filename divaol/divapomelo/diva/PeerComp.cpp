#include "PeerComp.h"
#include "PeerBase.h"

namespace divapomelo {
	void PeerComp::request(string route, Json::Value msg, Client::request_cb cb) {
		peer->request(route, msg, cb);
	}

	void PeerComp::on(std::string route, Client::message_cb cb) {
		peer->on(route, cb);
	}

	void PeerComp::remove(std::string route) {
		peer->remove(route);
	}

	void PeerComp::notify(std::string route, uint32 code, Json::Value &msg, int status) {
		peer->notify(route, code, &msg, status);
	}

	bool PeerComp::isLogin() { 
		return peer->isLogin();
	}

	UserInfo& PeerComp::getUserInfo() {
		return peer->getUserInfo();
	}
	ServerInfo& PeerComp::getServerInfo() {
		return peer->getServerInfo();
	}
}