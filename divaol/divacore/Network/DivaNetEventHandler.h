/*
 *  DivaNetEvent.h
 *
 *  Created by Hyf042 on 3/4/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_NET_EVENT
#define DIVA_NET_EVENT

#include "DivaNetPacket.h"

namespace divacore
{
	using namespace sora;
	/*
     *	Event Func, support class member func or global func
	 */
	namespace network
	{
		class EventFuncImpl
		{
		public:
			virtual void operator()(Packet&) = 0;
		};
		template<typename T>
		class MemberEventFuncImpl : public EventFuncImpl
		{
		public:
			typedef void (T::*MemberFunc)(Packet& arg);
			MemberEventFuncImpl(MemberFunc func, T* obj):func(func),obj(obj) {}

			void operator()(Packet& arg) {
				(obj->*func)(arg);
			}
		private:
			MemberFunc func;
			T* obj;
		};
		class CEventFuncImpl : public EventFuncImpl
		{
		public:
			typedef void (*CEventFunc)(Packet& arg);
			CEventFuncImpl(CEventFunc func): func(func) {}

			void operator()(Packet& arg) {
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
			EventFunc(void (T::*memberFunc)(Packet& arg),T* obj) {setAsMemberFunc(memberFunc,obj);}
			EventFunc(void (*cFunc)(Packet& arg)) {setAsCFunc(cFunc);}

			template<typename T>
			void setAsMemberFunc(void (T::*memberFunc)(Packet& arg),T* obj)
			{
				func = new MemberEventFuncImpl<T>(memberFunc, obj);
				if(!func)
					DIVA_EXCEPTION_MODULE( "Error creating event member func","DivaEvent");
			}
			void setAsCFunc(void (*cFunc)(Packet& arg))
			{
				func = new CEventFuncImpl(cFunc);
				if(!func)
					DIVA_EXCEPTION_MODULE( "Error creating event c func","DivaEvent");
			}

			void run(Packet& arg)
			{
				if(isValid())
					(*func)(arg);
			}
			void operator()(Packet& arg) {run(arg);}

			bool isValid() {return func!=0;}
		};
		typedef EventFunc* EventFuncPtr;

		/*
		* EventHandler, a interface to derive a event handle class
		*/
		class NetEventHandler
		{
			typedef std::map<uint32,EventFuncPtr> NET_EVENT_MAP;
			NET_EVENT_MAP netEventMap;
		public:
			NetEventHandler() {}
			~NetEventHandler() {clearEventMap();}

			inline bool isExisted(uint32 id) {return netEventMap.find(id)!=netEventMap.end();}

			void receive(uint32 id, EventFuncPtr eventFunc)
			{
				if(!isExisted(id))
					netEventMap[id] = eventFunc;
				else
					DIVA_EXCEPTION_MODULE("Event "+iToS(id)+" is existed!","EventManager");
			}
			void unreceive(uint32 id)
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
			void dispatch(uint32 id, Packet &packet) 
			{
				if(isExisted(id))
					(*netEventMap[id])(packet);
			}
		};
	}
	#define RECEIVE_PACKET(id,func) (NETWORK_SYSTEM_PTR->receive(id,new network::EventFunc((func),this)))
	#define UNRECEIVE_PACKET(id) (NETWORK_SYSTEM_PTR->unreceive(id))
}

#endif