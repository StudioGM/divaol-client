//
//  SoraMessageRouter.h
//  Sora
//
//  Created by Robert Bu on 8/24/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraMessageRouter_h
#define Sora_SoraMessageRouter_h

#include "SoraMessageEvent.h"
#include "event/SoraEventRouter.h"
#include "SoraPreDeclare.h"

namespace sora {
    
    template<>
    class SoraEventRouter<SoraMessageEvent> {
    public:
        typedef SoraMessageEvent EventType;
        typedef SoraSignal<void(EventType*)> SignalType;
        
        SoraEventRouter() {}
        virtual ~SoraEventRouter() {}
        
        template<typename T>
        inline void sendMessage(const MessageIdType& mssg, const T& data) {
            SoraMessageEvent evt(mssg, data);
            sendMessage(&evt);
        }
        
        inline void sendMessage(SoraMessageEvent* evt) {
            mConnections.sig(evt);
        }
        
        /**
         * Redirect events to signal connections
         * Inherited from SoraEventHandler
         **/
        void handleEvent(SoraEvent* evt) {
            if(evt->getReceiver() != NULL)
                evt->getReceiver()->handleEvent(evt);
            else {
#ifdef SORA_USE_RTTI
                if(isClassClass<SoraEvent, SoraMessageEvent>(evt))
#else
                if(evt->getEventIdentifier() == SoraMessageEvent::GetEventIdentifier())
#endif
                    sendMessage(static_cast<SoraMessageEvent*>(evt));
            }
        }
        
        /**
         * Connect to the router using direct function
         * Class can be non SoraEventHandler
         * But function prototype should be void(SoraEvent*)
         * And have to determine what event it is
         * EventHandlers can use like SoraBind(this, &MY_CLASS::handleEvent)
         * and let it handle the event itself
         **/
        template<typename FN>
        SoraConnection connect(const FN& func) {
            return mConnections.connect(SignalType::slot_type(func));
        }
        
    protected:
        SignalType mConnections;
    };
    
    typedef SoraEventRouter<SoraMessageEvent> SoraMessageRouter;
    
} // namespace sora

#endif
