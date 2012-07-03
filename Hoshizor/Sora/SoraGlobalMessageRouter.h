//
//  SoraGlobalMessageRouter.h
//  Sora
//
//  Created by Robert Bu on 8/24/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraGlobalMessageRouter_h
#define Sora_SoraGlobalMessageRouter_h

#include "message/SoraMessageRouter.h"

namespace sora {
    
    class SORA_API SoraGlobalMessageRouter {
    public:
        static SoraGlobalMessageRouter* Instance();
        
        /**
         * Rules for message routing
         * * Acceptors with specified message id have the highest priority
         * * Global Acceptors next
         * * When Specified Acceptors consumed the message event, global acceptors won't receive the message
         **/
        void sendMessage(SoraMessageEvent* evt);
        
        template<typename T>
        void sendMessage(const MessageIdType& mssg, const T& data);
                
        template<typename T>
        SoraConnection acceptMessage(const T& fn);
        
        template<typename T>
        SoraConnection acceptMessage(const MessageIdType& mssg, const T& fn);
        
    private:
        void handleEvent(SoraEvent* evt);
        
        SoraMessageRouter mRouter;
        
        typedef std::map<MessageIdType, SoraMessageRouter> MessageRouterMap;
        MessageRouterMap mMessageRouters;
    };
    
    template<typename T>
    inline void SoraGlobalMessageRouter::sendMessage(const MessageIdType& message, const T& data) {
        SoraMessageEvent evt(message, data);
        sendMessage(&evt);
    }
    
    template<typename T>
    inline SoraConnection SoraGlobalMessageRouter::acceptMessage(const MessageIdType& message, const T& fn) {
        MessageRouterMap::iterator it = mMessageRouters.find(message);
        if(it != mMessageRouters.end()) {
            return it->second.connect(fn);
        } else {
            it = mMessageRouters.insert(std::make_pair(message, SoraMessageRouter())).first;
            return it->second.connect(fn);
        }
    }
    
    template<typename T>
    inline SoraConnection SoraGlobalMessageRouter::acceptMessage(const T& fn) {
        return mRouter.connect(fn);
    }
    
    template<typename T>
    inline SoraConnection AcceptMessage(const MessageIdType& mssg, const T& fn) {
        return SoraGlobalMessageRouter::Instance()->acceptMessage(mssg, fn);
    }
    
    template<typename T>
    inline SoraConnection AcceptMessage(const T& fn) {
        return SoraGlobalMessageRouter::Instance()->acceptMessage(fn);
    }
    
    template<typename T>
    inline void SendMessage(const MessageIdType& mssg, const T& data) {
        return SoraGlobalMessageRouter::Instance()->sendMessage<T>(mssg, data);
    }
    
    template<typename T>
    inline void SendMessage(SoraMessageEvent* evt) {
        return SoraGlobalMessageRouter::Instance()->sendMessage(evt);
    }
    
} // namespace sora


#endif
