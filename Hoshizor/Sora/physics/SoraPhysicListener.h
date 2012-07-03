//
//  SoraPhysicListener.h
//  Sora
//
//  Created by Ruiwei Bu on 10/3/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraPhysicListener_h
#define Sora_SoraPhysicListener_h

#include "SoraPlatform.h"

namespace sora {
    
    class SoraPhysicBody;
    
    struct SoraPhysicContactInfo {
        SoraPhysicBody* mBodyA;
        SoraPhysicBody* mBodyB;
        
        // optional part
        // may not be filled depends on physical engine
        SoraVector mNormal;
        SoraVector mPosition;
        
        bool mIsTouching;
    };
    
    class SoraPhysicContactListener {
    public:
        virtual void onContactBegin(const SoraPhysicContactInfo& info) = 0;
        virtual void onContactEnd(const SoraPhysicContactInfo& info) = 0;
    };
    
    
} // namespace sora


#endif
