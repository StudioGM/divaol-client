#ifndef SORA_EVENT_H
#define SORA_EVENT_H

#include "SoraPlatform.h"
#include "SoraException.h"
#include "SoraTypeInfo.h"
#include "SoraAutoPtr.h"
#include "SoraStringId.h"

#include <map>

namespace sora {

    class SoraEventHandler;
	class SoraMessageEvent;
    
#define SORA_EVENT_IDENTIFIER(ident) \
    sora::SoraStringId getEventIdentifier() const { \
        return ident; \
    } \
    static sora::SoraStringId GetEventIdentifier() { \
		return ident; \
	}
    
	class SORA_API SoraEvent {
	protected:
        friend class SoraEventFactory;
		virtual ~SoraEvent() {};
        
    public:
        SoraEvent(): 
        mConsumed(false),
        mName(StringType()),
        mSender(NULL),
        mReceiver(NULL) {
		}

        void setSource(SoraEventHandler* source) { 
            mSender = source; 
        }
        SoraEventHandler* getSource() const { 
            return mSender;
        }
		
		void setName(StringType _name) { 
            mName = _name;
        }
		StringType getName() const { 
            return mName;
        }
		
		// consume a event
		void consume() { mConsumed = true; }
		bool isConsumed() const { return mConsumed; }
		
        virtual SoraStringId getEventIdentifier() const = 0;
       
        inline void setReceiver(SoraEventHandler* receiver) {
            mReceiver = receiver;
        }
        inline SoraEventHandler* getReceiver() const {
            return mReceiver;
        }
		
    protected:
        inline void init(const SoraEvent& evt);
    
		bool mConsumed;
		
		StringType mName;
        SoraEventHandler* mSender;
        SoraEventHandler* mReceiver;
    };
 
	class SORA_API SoraHandlerFunctionBase {
	public:
		virtual ~SoraHandlerFunctionBase() {};
		inline void exec(SoraEvent* event) { call(event);}

	private:
		virtual void call(SoraEvent*) = 0;
	};

	template <class T, class EventT>
	class SoraMemberFunctionHandler : public SoraHandlerFunctionBase {
	public:
		typedef void (T::*MemberFunc)(EventT*);
		SoraMemberFunctionHandler(T* instance, MemberFunc memFn) : _instance(instance), _function(memFn) {};

		inline void call(SoraEvent* event) {
			(_instance->*_function)(static_cast<EventT*>(event));
		}

	private:
		T* _instance;
		MemberFunc _function;
	};
	
	template<typename EventT>
	class SoraFuncFunctionHandler: public SoraHandlerFunctionBase  {
	public:
		typedef void (*EventFunc)(EventT* event);
		 
		SoraFuncFunctionHandler(EventFunc func): _func(func) {}
		
		inline void call(SoraEvent* ev) {
			_func(static_cast<EventT*>(ev));
		}
		
	private:
		EventFunc _func;
	};
    
    class SoraEventWorld;
    class SoraSystemEvent;

	class SORA_API SoraEventHandler: public SoraSharedClass<SoraEventHandler> {
        friend class SoraEventWorldImpl;
        friend class SoraEventWorld;
        
	public:
        SoraEventHandler();
		virtual ~SoraEventHandler();
        
        /** 
         * This function would ignore whether the handler is enabled in EventWorld
         *  or not
         * For compability with older versions of Sora
         * And see SORA_EVENT_GENERIC for event handling function
         **/
		virtual void handleEvent(SoraEvent*);

		template <class T, class EventT>
		void registerEventFunc(T*, void (T::*memFn)(EventT*));
		
		template <class EventT>
		void registerEventFunc(void (*evFn)(EventT*));
        
        bool isInWorld(SoraEventWorld* world) const;
        bool isInWorld() const;
        
        void enableUpdate(bool receiveEvent = false);
        void disableUpdate();
        bool isEnableUpdate() const;
        
        SoraEventWorld* getWorld(void);
        bool isEnabled();
        
        virtual	void onUpdate(float dt);
        // on debug render
        // must turnon SoraCore::enableDebugDraw to enable
        // and must belongs to defaultEventWorld
        virtual void onDebugRender();
        
        virtual void onEnable();
        virtual void onDisable();
        virtual void onMessage(SoraMessageEvent* message);
        
#ifdef SORA_EVENT_GENERIC
        virtual void onEvent(SoraEvent* event);
#endif
        
	private:
        /**
         * Directly handle system event without RTTI check, for performance
         **/
        void handleSystemEvent(SoraSystemEvent* evt);
        /**
         * Event through SoraEventWorld
         * Would check whether the handler is enabled or not
         * Default: No
         **/
        void handleWorldEvent(SoraEvent* evt);
        
        void enable(SoraEventWorld* world);
        void disable();
        
    private:
        bool mEnabled;
        SoraEventWorld* mWorld;
        
        bool mEnableUpdate;
        bool mUpdateReceiveEvent;
        
		typedef std::map<SoraTypeInfo, SoraHandlerFunctionBase*> Handlers;
		Handlers _handlers;
	};


	template <class T, class EventT>
	void SoraEventHandler::registerEventFunc(T* obj, void (T::*memFn)(EventT*)) {	
#ifdef SORA_USE_RTTI
		_handlers[SoraTypeInfo(typeid(EventT))]= new SoraMemberFunctionHandler<T, EventT>(obj, memFn);
#else
		EventT tmp;
		_handlers[SoraTypeInfo(tmp.getEventIdentifier())] = new SoraMemberFunctionHandler<T, EventT>(obj, memFn);
#endif
	}
	
	template <class EventT>
	void SoraEventHandler::registerEventFunc(void (*evFn)(EventT*)) {
#ifdef SORA_USE_RTTI
		_handlers[SoraTypeInfo(typeid(EventT))] = new SoraFuncFunctionHandler<EventT>(evFn);
#else
		EventT tmp;
		_handlers[SoraTypeInfo(tmp.getEventIdentifier())] = new SoraFuncFunctionHandler<EventT>(evFn);
#endif
	}
    
    /**
     * Helper macro that defines a event handler class [func]##EventHandler
     * With a global static access func Instance()
     * And when it handles [evtType] event
     * It will call [func]
     **/
#define SORA_DEF_FUNC_AS_EVENT_HANDLER(func, evtType) \
namespace { \
    struct func##EventHandler: public sora::SoraEventHandler { \
        func##EventHandler() { \
            registerEventFunc(this, &func##EventHandler::onEvent); \
        } \
        void onEvent(evtType* evt) { \
            func(evt); \
        } \
        static func##EventHandler* Instance() { \
            static func##EventHandler instance; \
            return &instance; \
        } \
    }; \
}

} // namespace sora

#endif