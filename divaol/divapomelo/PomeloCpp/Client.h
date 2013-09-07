#ifndef _POMELOCPP_CLIENT_H_
#define _POMELOCPP_CLIENT_H_

#ifdef _WIN32
#include <winsock2.h>
#else
#include <unistd.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <map>
#include <memory>
#include "PreDeclare.h"
#include "Event.h"
#include "Json.h"
#include "ReqWrapper.h"

using namespace std;

namespace PomeloCpp
{
	class Address
	{
	public:
		string ip;
		int port;
	};

	class Client
	{
	public:
		typedef Base::Function<void(NotifyReq&, int)> notify_cb;
		typedef Base::Function<void(RequestReq&, int, Json::Value)> request_cb;
		typedef Base::Function<void(MessageReq&)> message_cb;
		typedef Base::Function<void(ConnectReq&, int)> connected_cb;
		typedef Base::Publisher<void(MessageReq&)> message_pub;
		typedef Base::Publisher<void(ConnectReq&, int)> connected_pub;
		
	private:
		typedef unsigned long LONG;

		pc_client_t* client;
		Base::Thread<void()> taskThread;
		Base::Mutex mutex;
		
		int eventThreshold;
		bool connected;
		volatile bool taskRunning;

		Event lastEvent;
		Address address;
		Base::ThreadSafe::Queue<Event> events;
		map<LONG, request_cb> requestCbs;
		map<LONG, notify_cb> notifyCbs;
		map<string, message_pub> messagePubs;
		connected_pub _connected_pub;

		pc_msg_parse_done_cb default_parse_done;

	public:

		Client(string ip = "127.0.0.1", int port = 3010, int eventThreshold = -1)
		{
			this->address.ip = ip;
			this->address.port = port;
			connected = false;
			taskRunning = false;
			this->eventThreshold = eventThreshold;

			reset();
		}
		~Client() {
			disConnect();
		}

		/*
		 * interface
		 */
		bool connect(bool async = true, bool reqAsync = false)
		{
			if (connected)
				disConnect();

			client->data = (void*)this;

			struct sockaddr_in address;

			memset(&address, 0, sizeof(struct sockaddr_in));
			address.sin_family = AF_INET;
			address.sin_port = htons(this->address.port);
			address.sin_addr.s_addr = inet_addr(this->address.ip.c_str());

			default_parse_done = client->parse_msg_done;
			client->parse_msg_done = pc_msg_parse_done_cb;

			if (async) {
				pc_connect_t *conn_req = pc_connect_req_new(&address);
				//~ bring some private date on
				conn_req->data = (void*)this;

				if(pc_client_connect2(client, conn_req, on_connected_cb)) {
					BASE_LOGGER.log_error("fail to connect server.\n");
					pc_connect_req_destroy(conn_req);
					pc_client_destroy(client);
					return false;
				}
			}
			else {
				if(pc_client_connect(client, &address)) {
					BASE_LOGGER.log_error("fail to connect server.\n");
					pc_client_destroy(client);
					return false;
				}
				connected = true;
			}

			if (reqAsync) {
				taskThread.set(Base::Bind(this, &Client::asyncTask));
				taskThread.start();
			}

			return true;
		}
		void disConnect()
		{
			if (!connected)
				return;

			pc_client_destroy(client);
			connected = false;
			taskRunning = false;
			if (taskThread.active())
				taskThread.wait();
		}
		void reset() {
			client = pc_client_new();
			pc_add_listener(client, PC_EVENT_DISCONNECT, on_close_cb);
		}
		void join() {
			if (!isConnected())
				return;

			pc_client_join(client);
		}
		void work(double time = 1.0) {
			Base::Timestamp &timer = Base::GlobalTimeStamp::instance();
			double to = timer.elapsedInSecond() + time;
			while (timer.elapsedInSecond() < to) {
				logic();
				Base::TimeUtil::mSleep(1);
			}
		}

		void logic()
		{
			BASE_MUTEX_GUARD(mutex);

			Event ev;
			int count = 0;

			while (events.take(ev) && (eventThreshold < 0 || count++ < eventThreshold))
			{
				lastEvent = ev;
				switch (ev.type)
				{
				case Event::Type::OnClose:
					on_close(ev.event);
					break;
				case Event::Type::OnConnect:
					on_connected(ev.req.conn_req, ev.status);
					break;
				case Event::Type::OnRequest:
					on_request(ev.req.request_req, ev.status, ev.resp);
					break;
				case Event::Type::OnNotified:
					on_notified(ev.req.notify_req, ev.status);
					break;
				case Event::Type::OnMessage:
					on_message(ev.event, ev.resp);
					break;
				case Event::Type::OnClear:
					on_clear(ev.req.msg);
					break;
				default:
					BASE_LOGGER.log_warning("[Libpomelo.Client] Unknown Event.");
					break;
				}
			}
		}

		void request(std::string route, Json::Value msg, request_cb cb) {
			if (!isConnected())
				return;

			pc_request_t *request = pc_request_new();
			request->data = (void*)this;
			if (!cb.empty())
				requestCbs[(LONG)request] =  cb;

			pc_request(client, request, route.c_str(), JsonHelper::translate(msg), on_request_cb);
		}
		void notify(std::string route, Json::Value msg, notify_cb cb) {
			if (!isConnected())
				return;

			pc_notify_t *notify = pc_notify_new();
			notify->data = (void*)this;
			if (!cb.empty())
				notifyCbs[(LONG)notify] =  cb;

			pc_notify(client, notify, route.c_str(), JsonHelper::translate(msg), on_notified_cb);
		}

		/*
		 * event register/unregister
		 */
		void on(std::string route, message_cb cb) {
			if (!cb.empty()) {
				if (messagePubs.find(route) == messagePubs.end())
					pc_add_listener(client, route.c_str(), on_message_cb);
				messagePubs[route] += cb;
			}
		}
		void on(connected_cb cb) {
			_connected_pub += cb;
		}
		void once(std::string route, message_cb cb) {
			if (!cb.empty()) {
				messagePubs[route].once(cb);
			}
		}
		void once(connected_cb cb) {
			_connected_pub.once(cb);
		}
		void onclose(message_cb cb) {
			on("disconnect", cb);
		}
		void remove(std::string route, message_cb cb) {
			messagePubs[route] -= cb;
		}
		void remove(std::string route) {
			messagePubs[route].clear();
		}
		void remove(connected_cb cb) {
			_connected_pub -= cb;
		}
		
		/*
		 * Get&Set Functions
		 */
		bool isConnected() const  {
			return connected;
		}
		bool isReqAsync() const {return taskRunning;}
		Event getLastEvent() const {return lastEvent;}
		void setEventThreshold(int value) {eventThreshold = value;}
		int getEventThreshold() const {return eventThreshold;}

	private:
		/*
		 * Callback Functions
		 */
		void on_close(std::string event)
		{
			BASE_LOGGER.log("[PomeloCpp]Client closed.");

			disConnect();

			_callMessageCb("disconnect", event);
		}
		void on_connected(pc_connect_t *conn_req, int status)
		{
			BASE_LOGGER.log(Base::String::format("[PomeloCpp]Client Connected"));
			if (status == 0)
				connected = true;

			_connected_pub(ConnectReq(conn_req, this), status);

			pc_connect_req_destroy(conn_req);
		}
		void on_message(std::string event, json_t *msg)
		{
			if (!isConnected())
				return;

			BASE_LOGGER.log("[PomeloCpp]Client Message: " + event);

			try {
				Json::Value json = JsonHelper::translate(msg);
				if (json.isObject()) {
					std::string route = json["route"].asString();
					Json::Value _msg = json["msg"];
					_callMessageCb(route, event, _msg);
				}
				else
					_callMessageCb(event, event, json);
			}
			catch(...) {
				BASE_LOGGER.log_error("[PomeloCpp]Message Parse Failed: " + event);
			}
		}
		void on_request(pc_request_t *req, int status, json_t *resp) {
			if (!isConnected())
				return;

			BASE_LOGGER.log(Base::String::format("[PomeloCpp]Client Request Response: %s", req->route));

			Json::Value json = JsonHelper::translate(resp);
			if(requestCbs.find((LONG)req) != requestCbs.end()) {
				// check return code
				if (status == 0) {
					if (!json.isMember("code") || !json["code"].isInt() || !(json["code"].asInt() == 200))
						status = 1;
					if (json.isMember("msg"))
						json = json["msg"];
				}
				requestCbs[(LONG)req](RequestReq(req, this), status, json);
			}

			// release relative resource with pc_request_t
			json_t *msg = req->msg;
			json_decref(msg);
			pc_request_destroy(req);
		}
		void on_notified(pc_notify_t *req, int status) {
			if (!isConnected())
				return;

			BASE_LOGGER.log(Base::String::format("[PomeloCpp]Client Notify Response: %s", req->route));

			if(notifyCbs.find((LONG)req) != notifyCbs.end()) {
				notifyCbs[(LONG)req](NotifyReq(req, this), status);
				notifyCbs.erase((LONG)req);
			}

			// release resources
			json_t *msg = req->msg;
			json_decref(msg);
			pc_notify_destroy(req);
		}
		void on_clear(pc_msg_t *msg) {
			default_parse_done(client, msg);
		}

		/*
		 * Callback Static Functions
		 */
		static void on_close_cb(pc_client_t *client, const char *event, void *data)
		{
			static_cast<Client*>(client->data)->events.push(Event::CreateOnCloseEvent(event));
		}
		static void on_message_cb(pc_client_t *client, const char *event, void *data)
		{
			static_cast<Client*>(client->data)->events.push(Event::CreateOnMessageEvent(event, (json_t*)data));
		}
		static void on_connected_cb(pc_connect_t *conn_req, int status){
			static_cast<Client*>(conn_req->data)->events.push(Event::CreateOnConnectEvent(conn_req, status));
		}
		static void on_request_cb(pc_request_t *req, int status, json_t *resp){
			static_cast<Client*>(req->data)->events.push(Event::CreateOnRequestEvent(req, status, resp));
		}
		static void on_notified_cb(pc_notify_t *req, int status)
		{
			static_cast<Client*>(req->data)->events.push(Event::CreateOnNotifiedEvent(req, status));
		}

		static void pc_msg_parse_done_cb(pc_client_t *client, pc_msg_t *msg) {
			static_cast<Client*>(client->data)->events.push(Event::CreateOnClearEvent(msg));
		}
	
		void asyncTask() {
			taskRunning = true;
			while (taskRunning) {
				try {
					logic();
				}
				catch(...)
				{
					BASE_LOGGER.log_error(Base::String::format("[PomeloCpp]An error occured in AsyncTask."));
				}
				Base::TimeUtil::mSleep(1);
			}
			BASE_LOGGER.log(Base::String::format("[PomeloCpp]Async Task Thread Finished."));
		}

	private:
		bool _callMessageCb(std::string route, std::string event = "", Json::Value msg = NULL) {
			MessageReq req(route, event, msg, this); 
			if (messagePubs.find(route) == messagePubs.end())
				return false;
			messagePubs[route](req);
			return true;
		}
	};
}

#endif