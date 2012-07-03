//
//  SoraNotification.h
//  Sora
//
//  Created by Robert Bu on 7/11/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraNotification_h
#define Sora_SoraNotification_h

#include "SoraPlatform.h"
#include "SoraRefCounted.h"
#include "SoraAutoPtr.h"

namespace sora {
    
    class SORA_API SoraNotification: public SoraRefCounted {
    public:
        typedef SoraAutoPtr<SoraNotification> Ptr;
        
        SoraNotification();
        virtual std::string name() const;
        
    protected:
        virtual ~SoraNotification();
    };
    
    
} // namespace sora



#endif
