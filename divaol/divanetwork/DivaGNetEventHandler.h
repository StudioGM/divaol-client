/*
 *  DivaGNetEventHandler.h
 *
 *  Created by tempbuffer on 8/9/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 */

#ifndef DIVA_GNET_EVENT
#define DIVA_GNET_EVENT

#include "gnet/gnet.h"

namespace divanet
{
	typedef gnet::Item<gnet::Tuple> GPacket;

	/*
     *	Event Func, support class member func or global func
	 */
	namespace gnetwork
	{
		using namespace gnet;

		class EventFuncImpl
		{
		public:
			virtual void operator()(GPacket*) = 0;
		};
		template<typename T>
		class MemberEventFuncImpl : public EventFuncImpl
		{
		public:
			typedef void (T::*MemberFunc)(GPacket* arg);
			MemberEventFuncImpl(MemberFunc func, T* obj):func(func),obj(obj) {}

			void operator()(GPacket* arg) {
				(obj->*func)(arg);
			}
		private:
			MemberFunc func;
			T* obj;
		};
		class CEventFuncImpl : public EventFuncImpl
		{
		public:
			typedef void (*CEventFunc)(GPacket* arg);
			CEventFuncImpl(CEventFunc func): func(func) {}

			void operator()(GPacket* arg) {
				if(func)
					func(arg);
			}

		private:
			CEventFunc func;
		};
		class EventFunc
		{
			EventFuncImpl *func;
		public:
			EventFunc(): func(NULL) {}
			template<typename T>
			EventFunc(void (T::*memberFunc)(GPacket* arg),T* obj) {setAsMemberFunc(memberFunc,obj);}
			EventFunc(void (*cFunc)(GPacket* arg)) {setAsCFunc(cFunc);}

			template<typename T>
			void setAsMemberFunc(void (T::*memberFunc)(GPacket* arg),T* obj)
			{
				func = new MemberEventFuncImpl<T>(memberFunc, obj);
				if(!func)
					DIVA_EXCEPTION_MODULE( "Error creating event member func","DivaEvent");
			}
			void setAsCFunc(void (*cFunc)(GPacket* arg))
			{
				func = new CEventFuncImpl(cFunc);
				if(!func)
					DIVA_EXCEPTION_MODULE( "Error creating event c func","DivaEvent");
			}

			void run(GPacket* arg)
			{
				if(isValid())
					(*func)(arg);
			}
			void operator()(GPacket* arg) {run(arg);}

			bool isValid() {return func!=0;}
		};
		typedef EventFunc* EventFuncPtr;

		/*
		* EventHandler, a interface to derive a event handle class
		*/
		class GNetEventHandler
		{
			typedef std::map<std::string,EventFuncPtr> NET_EVENT_MAP;
			NET_EVENT_MAP netEventMap;
		public:
			GNetEventHandler() {}
			~GNetEventHandler() {clearEventMap();}

			inline bool isExisted(const std::string &id) {return netEventMap.find(id)!=netEventMap.end();}

			void receive(const std::string &id, EventFuncPtr eventFunc)
			{
				if(!isExisted(id))
					netEventMap[id] = eventFunc;
				else
					DIVA_EXCEPTION_MODULE("Event "+id+" is existed!","EventManager");
			}
			void unreceive(const std::string &id)
			{
				if(isExisted(id))
				{
					SAFE_DELETE(netEventMap[id]);
					netEventMap.erase(netEventMap.find(id));
				}
			}
			void clearEventMap() 
			{
				for(NET_EVENT_MAP::iterator ptr = netEventMap.begin(); ptr != netEventMap.end(); ptr++)
					SAFE_DELETE(ptr->second);
				netEventMap.clear();
			}
			void dispatch(const std::string &id,  GPacket *packet) 
			{
				if(isExisted(id))
					(*netEventMap[id])(packet);
			}
		};
	}
	#define GNET_RECEIVE_PACKET(id,func) (NETWORK_SYSTEM_PTR->receive(id,new gnetwork::EventFunc((func),this)))
	#define GNET_UNRECEIVE_PACKET(id) (NETWORK_SYSTEM_PTR->unreceive(id))
}

#endif