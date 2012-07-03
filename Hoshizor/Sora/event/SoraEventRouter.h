//
//  Header.h
//  Sora
//
//  Created by Robert Bu on 8/23/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraEventRouter_h
#define Sora_SoraEventRouter_h

#include "signal/SoraSignal.h"
#include "SoraEvent.h"
#include "SoraEventWorld.h"

namespace sora {
    
    /**
     * A EventRouter can redirect events to others
     * Event it's not a EventHandler
     * Useful for message handling
     **/
    
    template<typename T = sora::SoraEvent>
    class SoraEventRouter: public SoraEventHandler {
    public:
        typedef T EventType;
        typedef SoraSignal<void(EventType*)> SignalType;
        
        SoraEventRouter();
        virtual ~SoraEventRouter() {}
        
        /**
         * Redirect events to signal connections
         * Inherited from SoraEventHandler
         **/
        void handleEvent(SoraEvent*);
        
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
    
    
    template<typename T>
    SoraEventRouter<T>::SoraEventRouter() {
        SoraEventWorld::defaultWorld().enter(this);
        SoraEventHandler::enableUpdate();
    }
    
    template<typename T>
    inline void SoraEventRouter<T>::handleEvent(SoraEvent* evt) {
        mConnections.sig(evt);
    }
    
} // namespace sora


#endif
