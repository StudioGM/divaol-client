//
//  SoraPhysicJoint.h
//  Sora
//
//  Created by Ruiwei Bu on 10/2/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraPhysicJoint_h
#define Sora_SoraPhysicJoint_h

#include "SoraPlatform.h"
#include "SoraMath.h"

namespace sora {
    
    class SoraPhysicBody;
    
    class SoraPhysicJoint {
	public:
        virtual SoraVector getAnchorA() = 0;
        virtual SoraVector getAnchorB() = 0;
        
        SoraPhysicBody* mBodyA;
        SoraPhysicBody* mBodyB;
        
        void* mUserData;
    };
    
} // namespace sora

#endif
