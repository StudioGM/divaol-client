//
//  SoraMessageEvent.h
//  Sora
//
//  Created by Robert Bu on 8/22/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraMessageEvent_h
#define Sora_SoraMessageEvent_h

#include "SoraEvent.h"
#include "SoraAny.h"

namespace sora {
    
    /**
     * Template class for a event that contains a message 
     * and AnyData
     **/

    typedef SoraString MessageIdType;
    
    class SoraMessageEvent: public SoraEvent {
    public:        
        SoraMessageEvent() { }
        
        SoraMessageEvent(MessageIdType message):
        mMessageType(message) { }
        
        template<typename T>
        SoraMessageEvent(MessageIdType message, const T& data):
        mMessageType(message),
        mData(data) { }
        
        virtual ~SoraMessageEvent() { } 
        
        MessageIdType getMessage() const {
            return mMessageType;
        }
        
        template<typename T>
        inline void setData(const T& data) {
            mData = data;
        }
        
        /**
         * May throw a exception
         **/
        template<typename T>
        inline T getData() const {
            return AnyCast<T>(mData);
        }
        
        SORA_EVENT_IDENTIFIER(GetUniqueStringId("SoraMessageEvent"));
        
    private:
        MessageIdType mMessageType;
        SoraAny mData;
    };
    
    template<typename T>
    inline SoraMessageEvent* MakeMessage(const std::string& name, const T& data) {
        return new SoraMessageEvent(name, data);
    }
    
    template<typename T>
    inline void DestroyMessage(SoraMessageEvent* message) {
        sora_assert(message);
        
        delete message;
    }
    
} // namespace sora



#endif
