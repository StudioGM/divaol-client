#ifndef _POMELOCPP_REQWRAPPER_H_
#define _POMELOCPP_REQWRAPPER_H_

#include <string>
#include "PreDeclare.h"
#include "Json.h"

namespace PomeloCpp
{
	template<typename REQ>
	class ReqWrapper
	{
	protected:
		Json::Value _msg;
		REQ *_data;
		Client* _client;
	public:
		ReqWrapper(REQ *data, Client*client):_data(data), _client(client) {}

		pc_req_type type() const {return _data->type;}
		std::string route() const {return _data->route;}
		Json::Value msg() {
			if (_msg.empty())
				_msg = JsonHelper::translate(_data->msg);
			return _msg;
		}
		Client* client() const {return client;}
	};

	typedef ReqWrapper<pc_notify_t> NotifyReq;

	class RequestReq : public ReqWrapper<pc_request_t>
	{
	public:
		RequestReq(pc_request_t *t, Client *client):ReqWrapper<pc_request_t>(t,client) {}
		Base::uint32 id() {return _data->id;}
	};

	class ConnectReq
	{
	protected:
		pc_connect_t *_data;
		Client *_client;
	public:
		ConnectReq(pc_connect_t *data, Client *client):_data(data),_client(client) {}

		pc_req_type type() const {return _data->type;}
		std::string ip() const {return inet_ntoa(_data->address->sin_addr);}
		int port() const {return _data->address->sin_port;}
		void* data() const {return _data->data;}
		uv_tcp_t* socket() const {return _data->socket;}
		Client* client() const {return _client;}
	};

	class MessageReq
	{
	protected:
		Client *_client;
		std::string _event, _route;
		Json::Value _msg;

	public:
		MessageReq(std::string route, std::string event, Json::Value msg, Client *client)
			:_client(client),_event(event),_route(route),_msg(msg) {}

		const std::string& event() const {return _event;}
		Json::Value& msg() {return _msg;}
		Client* client() const {return _client;}
		const std::string& route() const {return _route;}
	};
}

#endif