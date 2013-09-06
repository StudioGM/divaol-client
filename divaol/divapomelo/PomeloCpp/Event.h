#ifndef _POMELOCPP_EVENT_H_
#define _POMELOCPP_EVENT_H_

#include "PreDeclare.h"

namespace PomeloCpp
{
	class Event
	{
	public:
		enum Type{None, OnConnect, OnClose, OnRequest, OnNotified, OnClear, OnMessage};

		Type type;
		union {
			pc_connect_t *conn_req;
			pc_request_t *request_req;
			pc_notify_t *notify_req;
			pc_msg_t *msg;
		}req;
		json_t *resp;
		std::string event;
		int status;
		double timeStamp;

		Event():type(None),status(0)  {
			timeStamp = 0;
		}
		Event(Type type):type(type),status(0) {
			timeStamp = Base::GlobalTimeStamp::instance().elapsedInSecond();
		}

		static Event CreateOnCloseEvent(std::string event) {
			Event ev(OnClose);
			ev.event = event;
			return ev;
		}
		static Event CreateOnMessageEvent(std::string event, json_t *msg) {
			Event ev(OnMessage);
			ev.event = event;
			ev.resp = msg;
			return ev;
		}
		static Event CreateOnConnectEvent(pc_connect_t *conn_req, int status) {
			Event ev(OnConnect);
			ev.req.conn_req = conn_req;
			ev.status = status;
			return ev;
		}
		static Event CreateOnRequestEvent(pc_request_t *req, int status, json_t *resp) {
			Event ev(OnRequest);
			ev.req.request_req = req;
			ev.status = status;
			ev.resp = resp;
			return ev;
		}
		static Event CreateOnNotifiedEvent(pc_notify_t *req, int status) {
			Event ev(OnNotified);
			ev.req.notify_req = req;
			ev.status = status;
			return ev;
		}
		static Event CreateOnClearEvent(pc_msg_t *msg) {
			Event ev(OnClear);
			ev.req.msg = msg;
			return ev;
		}
	};
}

#endif