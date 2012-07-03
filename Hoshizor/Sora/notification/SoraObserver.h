//
//  SoraObserver.h
//  Sora
//
//  Created by Robert Bu on 7/11/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraObserver_h
#define Sora_SoraObserver_h

#include "SoraPlatform.h"
#include "SoraNotification.h"

namespace sora {
    
    class SORA_API SoraAbstractObserver {
    public:
        SoraAbstractObserver();
        SoraAbstractObserver(const SoraAbstractObserver& observer);
        virtual ~SoraAbstractObserver();
        
        SoraAbstractObserver& operator = (const SoraAbstractObserver& observer);
        
        virtual void notify(SoraNotification* nf) const = 0;
        virtual bool equals(const SoraAbstractObserver& observer) const = 0;
        virtual bool accepts(SoraNotification* nf) const = 0;
        virtual SoraAbstractObserver* clone() const = 0;
    };
    
    template<class C, class N>
    class SORA_API SoraObserver: public SoraAbstractObserver {
    public:
        typedef void (C::*Callback)(N*);
        
        SoraObserver(C& object, Callback method):
        mObject(&object),
        mMethod(method) {
            
        }
        
        SoraObserver(const SoraObserver& observer):
        SoraAbstractObserver(observer),
        mObject(observer.mObject),
        mMethod(observer.mMethod) {
            
        }
        
        ~SoraObserver() {
            
        }
        
        SoraObserver& operator=(const SoraObserver& observer) {
            if(&observer != this) {
                mObject = observer.mObject;
                mMethod = observer.mMethod;
            }
            return *this;
        }
        
        void notify(SoraNotification* nf) const {
            N* castedNf = dynamic_cast<N*>(nf);
            if(castedNf) {
                castedNf->duplicate();
                (mObject->*mMethod)(castedNf);
            }
        }
        
        bool equals(const SoraAbstractObserver& observer) const {
            const SoraObserver& pobs = dynamic_cast<const SoraObserver*>(&observer);
            return pobs && pobs->mObject == mObject && pobs->mMethod == mMethod;
        }
        
        bool accepts(SoraNotification* nf) {
            return dynamic_cast<N*>(nf) != 0;
        }
        
        SoraAbstractObserver* clone() const {
            return new SoraObserver(*this);
        }
        
    private:
        SoraObserver();
        
        C* mObject;
        Callback mMethod;
    };
    
} // namespace sora



#endif
