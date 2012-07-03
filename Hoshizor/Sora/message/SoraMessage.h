//
//  SoraMessage.h
//  Sora
//
//  Created by Robert Bu on 8/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraMessage_h
#define Sora_SoraMessage_h

#include "SoraPlatform.h"
#include "SoraAny.h"

namespace sora {
    
    namespace message {
        
        template<typename TSource, typename TTarget>
        struct traits;
        
        template<>
        struct traits<int, int> {
            typedef int Source;
            typedef int Target;
            
            static const Source kNoSource = 0;
            static const Source kAnySource = -2;
            
            static const Target kNoTarget = 0;
            static const Target kAnyTarget = -2;
        };
        
        template<typename TSource, typename TTarget>
        struct MessageBase {
        private:
            MessageBase(const MessageBase&);
            MessageBase& operator=(const MessageBase&);
            
        public:
            typedef traits<TSource, TTarget> TraitsType;
            
            template<typename T>
            MessageBase(const T& val,
                        TSource src = TraitsType::kNoSource,
                        TTarget tgt = TraitsType::kNoTarget,
                        const std::string& msgTypeName = std::string()):
            mValue(val),
            mSource(src),
            mTarget(tgt),
            mTypeName(msgTypeName.empty() ?
                      GetTypeName<T>(): 
                      msgTypeName) {
            
            }
            
            const SoraAny& value() const {
                return mValue;
            }
            
            template<typename TargetT>
            bool valueAs(TargetT& o) {
                try {
                    o = AnyCast<TargetT>(o);
                    return true;
                } catch(BadCastException&) {
                    return false;
                }
            }
            
            const std::string& msgTypeName() const {
                return mTypeName;
            }
            
            TSource source() const {
                return mSource;
            }
            
            TTarget target() const {
                return mTarget;
            }
            
            void setTarget(const TTarget tgt) {
                mTarget = tgt;
            }
            
        protected:
            SoraAny mValue;
            
        private:
            std::string mTypeName;
            TSource mSource;
            TTarget mTarget;
        };
    } // namespace message
    
    template<typename TSource, typename TTarget, typename T>
    struct SoraMessage: public message::MessageBase<TSource, TTarget> {
        typedef message::traits<TSource, TTarget> TraitsType;
        
        SoraMessage(const T& v,
                    TSource src = TraitsType::kNoSource,
                    TTarget tgt = TraitsType::kNoTarget,
                    const std::string& msgTypeName = std::string()):
        message::MessageBase<TSource, TTarget>(v, src, tgt, msgTypeName) {
            
        }
        
        SoraMessage():
        message::MessageBase<TSource, TTarget>(T()) {
            
        }
        
        void setValue(const T& v) {
            this->mValue = v;
        }
    };
    
    
} // namespace sora 



#endif
