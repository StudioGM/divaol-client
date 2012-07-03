/*
 *  DivaEventManager.h
 *
 *  Created by Hyf042 on 1/21/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_EVENT_MANAGER
#define DIVA_EVENT_MANAGER

#include "DivaCommon.h"
#include "DivaMapInfo.h"

namespace divacore
{
	using namespace sora;
	/*
     *	Event Func, support class member func or global func
	 */
	class EventFuncImpl
	{
	public:
		virtual void operator()(ARGUMENTS &arg) = 0;
	};
	template<typename T>
	class MemberEventFuncImpl : public EventFuncImpl
	{
	public:
		typedef void (T::*MemberFunc)(ARGUMENTS &arg);
		MemberEventFuncImpl(MemberFunc func, T* obj):func(func),obj(obj) {}

		 void operator()(ARGUMENTS &arg) {
            (obj->*func)(arg);
        }
	private:
		MemberFunc func;
        T* obj;
	};
	class CEventFuncImpl : public EventFuncImpl
	{
	public:
        typedef void (*CEventFunc)(ARGUMENTS &arg);
        CEventFuncImpl(CEventFunc func): func(func) {}
        
        void operator()(ARGUMENTS &arg) {
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
		EventFunc(void (T::*memberFunc)(ARGUMENTS &arg),T* obj) {setAsMemberFunc(memberFunc,obj);}
		EventFunc(void (*cFunc)(ARGUMENTS &arg)) {setAsCFunc(cFunc);}

		template<typename T>
		void setAsMemberFunc(void (T::*memberFunc)(ARGUMENTS &arg),T* obj)
		{
			func = new MemberEventFuncImpl<T>(memberFunc, obj);
			if(!func)
				DIVA_EXCEPTION_MODULE( "Error creating event member func","DivaEvent");
		}
		void setAsCFunc(void (*cFunc)(ARGUMENTS &arg))
		{
			func = new CEventFuncImpl(cFunc);
			if(!func)
				DIVA_EXCEPTION_MODULE( "Error creating event c func","DivaEvent");
		}

		void run(ARGUMENTS &arg)
		{
			if(isValid())
				(*func)(arg);
		}
		void operator()(ARGUMENTS &arg) {run(arg);}
        
		bool isValid() {return func!=0;}
	};
	typedef EventFunc* EventFuncPtr;

	/*
	 * EventManager , register or call any function
	 */
	class EventManager
	{
		typedef std::map<std::string,EventFuncPtr> EVENTMAP;
		EVENTMAP eventMap;
	public:
		EventManager() {}
		~EventManager() {for(EVENTMAP::iterator ptr = eventMap.begin(); ptr != eventMap.end(); ptr++)SAFE_DELETE((ptr->second)); eventMap.clear();}

		inline bool isExisted(const std::string &name) {return eventMap.find(name)!=eventMap.end();}
		void Register(const std::string &name, EventFuncPtr eventFunc)
		{
			if(!isExisted(name))
				eventMap[name] = eventFunc;
			else
				DIVA_EXCEPTION_MODULE("Event "+name+" is existed!","EventManager");
		}
		void unRegister(const std::string &name)
		{
			if(isExisted(name))
			{
				SAFE_DELETE(eventMap[name]);
				eventMap.erase(eventMap.find(name));
			}
		}
		void call(const std::string &name, ARGUMENTS &arg)
		{
			if(isExisted(name))
				eventMap[name]->run(arg);
			//else
			//	DIVA_EXCEPTION_MODULE("Event "+name+" is not existed!","EventManager");
		}
		void call(MapEvent &event) {call(event.eventType,event.arg);}
	};
	typedef EventManager* EventManagerPtr;

	/*
	 * EventHandler, a interface to derive a event handle class
	 */
	class EventHandler
	{
		EventManagerPtr manager;
	protected:
		bool isValid() {return manager!=0;}

		void registerEvent(const std::string &name, EventFuncPtr eventFunc) {assert(isValid());manager->Register(name,eventFunc);}
		void unRegisterEvent(const std::string &name) {assert(isValid());manager->unRegister(name);}
		
		template<typename T>
		void registerEvent(const std::string &name, void (T::*memberFunc)(ARGUMENTS &arg)) {assert(isValid());manager->Register(name,new EventFunc(memberFunc,(T*)this));}
		void registerEvent(const std::string &name, void (*cFunc)(ARGUMENTS &arg)) {assert(isValid());manager->Register(name,new EventFunc(cFunc));}

		void call(const std::string name, ARGUMENTS &arg){assert(isValid());manager->call(name,arg);}
		void call(MapEvent &event) {assert(isValid());manager->call(event);}

	public:
		void setEventManager(EventManagerPtr manager) {this->manager = manager;}
		virtual void registerEvents() {assert(isValid());}
		virtual void gameReset() {}
		virtual void gameStart() {}
		virtual void gameStop() {}
		virtual void gameOver() {}
		virtual void gameReady() {}
		virtual void gameLoad(const std::string &configFile) {}
		virtual void init() {}
		virtual void destroy() {}
		virtual void update(float dt) {}
	};
}

#endif