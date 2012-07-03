//
//  SoraTaskNotification.h
//  Sora
//
//  Created by Ruiwei Bu on 9/30/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraTaskNotification_h
#define Sora_SoraTaskNotification_h

#include "SoraEvent.h"
#include "SoraStringId.h"

namespace sora {
    
    class SoraAbstractTask;
    
    class SoraTaskNotification: public SoraEvent {
    public:
        enum NotificationType {
            TaskStarted,
            TaskFinished,
            TaskCanceled,
            TaskProgress,
        };
        
        SoraTaskNotification(NotificationType type, SoraAbstractTask* task):
        mType(type),
        mTask(task) {}
        
        NotificationType notificationType() const {
            return mType;
        }
        
        SoraAbstractTask* task() const {
            return mTask;
        }
        
        SORA_EVENT_IDENTIFIER(GetUniqueStringId("SoraTaskNotification"));
        
    private:
        NotificationType mType;
        SoraAbstractTask* mTask;
    };
    
} // namespace sora


#endif
