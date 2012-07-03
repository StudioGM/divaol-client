//
//  SoraPhysicalWorld.h
//  Sora
//
//  Created by Ruiwei Bu on 10/2/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraPhysicalWorld_h
#define Sora_SoraPhysicalWorld_h

#include "SoraPlatform.h"
#include "SoraMath.h"

namespace sora {
    
    class SoraPhysicBody;
    class SoraPhysicJoint;
    class SoraPhysicContactListener;
    struct SoraPhysicBodyDef;
    struct SoraPhysicJointDef;
    struct SoraPhysicRaycastResult;
    
    class SoraPhysicWorld {
    public:
        virtual ~SoraPhysicWorld() {}
        
        virtual SoraPhysicBody* createBody(const SoraPhysicBodyDef& def) = 0;
        virtual void destroyBody(SoraPhysicBody* body) = 0;
        
        virtual SoraPhysicJoint* createJoint(const SoraPhysicJointDef& def) = 0;
        virtual void destroyJoint(SoraPhysicJoint* joint) = 0;
        
        virtual uint32 bodyCount() = 0;
        
        virtual void update(float dt) = 0;
        
        virtual void addContactListener(SoraPhysicContactListener* listener) = 0;
        virtual void delContactListener(SoraPhysicContactListener* listener) = 0;
        
        virtual SoraPhysicRaycastResult rayCast(const SoraVector& start, const SoraVector& dir) = 0;
        
        static SoraPhysicBody*  CreateBody(const SoraPhysicBodyDef& def);
        static void             DestroyBody(SoraPhysicBody* body);
        static SoraPhysicJoint* CreateJoint(const SoraPhysicJointDef& def);
        static void             DestroyJoint(SoraPhysicJoint* joint);
        static void             AddContactListener(SoraPhysicContactListener* listener);
        static void             DelContactListener(SoraPhysicContactListener* listener);
    };
    
} // namespace sora



#endif
