//
//  SoraListener.h
//  Sora
//
//  Created by Robert Bu on 8/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraListener_h
#define Sora_SoraListener_h

#include "SoraPlatform.h"
#include "SoraDelegate.h"
#include "SoraLogger.h"

#include "SoraMessage.h"

namespace sora {
    
    namespace message {
        template<typename TSource, typename TTarget>
        struct ListenerBase {
        private:
            ListenerBase(const ListenerBase& rhs);
            ListenerBase& operator=(const ListenerBase& rhs);
            
        public:
            typedef MessageBase<TSource, TTarget> MessageType;
            
            ListenerBase(const std::string& msgTypeName):
            mTypeName(msgTypeName) {
                
            }
            
            virtual ~ListenerBase() {
            }
            
            void execute(const MessageType& msg) {
                doExecute(msg);
            }
            
            const std::string& msgTypeName() const {
                return mTypeName;
            }
            
            void destroy() {
                sora_assert(this);
                delete this;
            }
            
        protected:
            virtual void doExecute(const MessageType&) = 0;
            
        private:
            std::string mTypeName;
        };
    }
    
    template<typename TSource, typename TTarget, typename TMsg>
    struct SoraListener: public message::ListenerBase<TSource, TTarget> {
    private:
        SoraListener(const SoraListener&);
        SoraListener& operator=(const SoraListener&);
        
    public:
        typedef SoraAbstractDelegate<TMsg, void> HandleFuncType;
        typedef message::MessageBase<TSource, TTarget> MessageType;
        
        virtual ~SoraListener() {
            if(mHandleFunc)
                delete mHandleFunc;
        }
        
        SoraListener(const HandleFuncType& fn):
        message::ListenerBase<TSource, TTarget>(GetTypeName<TMsg>()),
        mHandleFunc(fn.clone()) {
            
        }
        
    protected:
        virtual void doExecute(const MessageType& msg) {
            try {
                TMsg val = AnyCast<TMsg>(msg.value());
                mHandleFunc->notify(this, val);
            }
            catch(BadCastException& e) {
                log_error(std::string("msg.exception: ")+e.what());
            }
            catch(...) {
                log_error("msg.exception: unhandled exception");
            }
        }
        
    private:
        HandleFuncType* mHandleFunc;
    };
    
    template<typename TSource, typename TTarget, typename T>
    SoraListener<TSource, TTarget, T>* MakeListener(const SoraAbstractDelegate<T, void>& fn) {
        return new SoraListener<TSource, TTarget, T>(fn);
    }
    
} // namespace sora

#endif
