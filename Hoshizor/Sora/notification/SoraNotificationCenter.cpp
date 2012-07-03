//
//  SoraNotificationCenter.cpp
//  Sora
//
//  Created by Robert Bu on 7/11/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraNotificationCenter.h"
#include "SoraSingletonHolder.h"


namespace sora {
    
    SoraNotificationCenter::SoraNotificationCenter() {
        
    }
    
    SoraNotificationCenter::~SoraNotificationCenter() {
        
    }
    
    void SoraNotificationCenter::addObserver(const SoraAbstractObserver& observer) {
        SoraAutoPtr<SoraAbstractObserver> ap(observer.clone());
        mObservers.push_back(ap);
    }
    
    void SoraNotificationCenter::removeObserver(const SoraAbstractObserver& observer) {
        for(ObserverList::iterator it = mObservers.begin();
            it != mObservers.end();
            ++it) {
            if(observer.equals(*(*it).get())) {
                mObservers.erase(it);
                return;
            }
        }
    }
    
    void SoraNotificationCenter::postNotification(SoraNotification* notification) {
        ObserverList observersToNotify(mObservers);
        for(ObserverList::iterator it = observersToNotify.begin();
            it != observersToNotify.end();
            ++it) {
            (*it)->notify(notification);
        }
    }
    
    bool SoraNotificationCenter::hasObservers() const {
        return !mObservers.empty();
    }
    
    std::size_t SoraNotificationCenter::countObservers() const {
        return mObservers.size();
    }
    
    namespace {
        static SoraSingletonHolder<SoraNotificationCenter, void> sh;
    }
    
    SoraNotificationCenter& SoraNotificationCenter::defaultCenter() {
        return *sh.get();
    }
    
} // namespace sora