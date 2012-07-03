//
//  SoraPhysicBodyDef.h
//  Sora
//
//  Created by Ruiwei Bu on 10/3/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraPhysicBodyDef_h
#define Sora_SoraPhysicBodyDef_h

#include "SoraPhysicShape.h"
#include "SoraMath.h"

namespace sora {
    
    class SoraPhysicBody;
    
    struct SoraPhysicFixtureDef {
        float mFriction;
        float mDensity;
        
        SoraPhysicShape mShape;
        
        SoraPhysicFixtureDef():
        mFriction(0.f),
        mDensity(1.f) { }
        
        SoraPhysicFixtureDef(const SoraPhysicShape& shape, float friction=0.f, float density=1.0f):
        mFriction(friction),
        mDensity(density),
        mShape(shape) { }
        
        SoraPhysicFixtureDef& friction(float friction) {
            this->mFriction = friction;
            return *this;
        }
        
        SoraPhysicFixtureDef& density(float density) {
            this->mDensity = density;
            return *this;
        }
    };
    
    struct SoraPhysicBodyDef {
        enum BodyType {
            DynamicBody,
            StaticBody,
        };
        
        BodyType mType;
        SoraVector mPosition;
        
        SoraPhysicBodyDef(BodyType type=DynamicBody):
        mType(type) { }
        
        SoraPhysicBodyDef(BodyType type, const SoraVector& pos):
        mType(type),
        mPosition(pos) { }
        
        SoraPhysicBodyDef& friction(BodyType type) {
            this->mType = type;
            return *this;
        }
        
        SoraPhysicBodyDef& density(const SoraVector& pos) {
            this->mPosition = pos;
            return *this;
        }
    };
    
    struct SoraPhysicJointDef {
        enum JointType {
            None = -1,
            RevoluteJoint,
            PrismaticJoint,
            DistanceJoint,
            LineJoint,
        };
        
        JointType mType;
        SoraPhysicBody* mBodyA;
        SoraPhysicBody* mBodyB;
        
        SoraPhysicJointDef():
        mType(None),
        mBodyA(0),
        mBodyB(0) { }
        
        SoraPhysicJointDef(JointType type, SoraPhysicBody* b1, SoraPhysicBody* b2):
        mType(type),
        mBodyA(b1),
        mBodyB(b2) { }
        
        SoraPhysicJointDef& type(JointType type) {
            this->mType = type;
            return *this;
        }
        
        SoraPhysicJointDef& bodyA(SoraPhysicBody* b1) {
            this->mBodyA = b1;
            return *this;
        }
        
        SoraPhysicJointDef& bodyB(SoraPhysicBody* b2) {
            this->mBodyB = b2;
            return *this;
        }
    };
    
    struct SoraPhysicRaycastResult {
        SoraVector mPoint;
        SoraVector mNormal;
        float mFraction;
        
        SoraPhysicBody* mBody;
    };
    
} // namespace sora



#endif
