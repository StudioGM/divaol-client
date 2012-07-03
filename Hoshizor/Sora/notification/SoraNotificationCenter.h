//
//  SoraNotificationCenter.h
//  Sora
//
//  Created by Robert Bu on 7/11/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraNotificationCenter_h
#define Sora_SoraNotificationCenter_h

/// The NotificationCenter class is basically a C++ implementation of the NSNotificationCenter class
/// found in Apple's Cocoa (or OpenStep).
///
/// While handling a notification, an observer can unregister itself from the notification center,
/// or it can register or unregister other observers. Observers added during a dispatch cycle
/// will not receive the current notification.
///
/// The method receiving the notification must be implemented as
///     void handleNotification(MyNotification* pNf);
/// The handler method gets co-ownership of the Notification object
/// and must release it when done. This is best done with an AutoPtr:
///     void MyClass::handleNotification(MyNotification* pNf)
///     {
///         AutoPtr<MyNotification> nf(pNf);
///         ...
///     }

#include "SoraPlatform.h"
#include "SoraNotification.h"
#include "SoraObserver.h"
#include "SoraAutoPtr.h"

namespace sora {
    
    class SORA_API SoraNotificationCenter {
    public:
        SoraNotificationCenter();
        ~SoraNotificationCenter();
        
        void    addObserver(const SoraAbstractObserver& observer);
        void    removeObserver(const SoraAbstractObserver& observer);
        void    postNotification(SoraNotification* Notification);
        bool    hasObservers() const;
        
        std::size_t countObservers() const;
        static SoraNotificationCenter& defaultCenter();
        
    private:
        typedef SoraAutoPtr<SoraAbstractObserver> AbstractObserverPtr;
        typedef std::vector<AbstractObserverPtr> ObserverList;
        
        ObserverList mObservers;
    };
    
    
} // namespace sora

#endif
