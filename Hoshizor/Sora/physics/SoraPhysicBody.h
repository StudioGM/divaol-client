//
//  SoraPhysicalBody.h
//  Sora
//
//  Created by Ruiwei Bu on 10/2/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraPhysicalBody_h
#define Sora_SoraPhysicalBody_h

#include "SoraPlatform.h"
#include "SoraMovable.h"
#include "SoraPhysicDef.h"
#include "SoraMath.h"

namespace sora {

    class SoraPhysicBody {
    public:
        SoraPhysicBody(const SoraPhysicBodyDef& def);
        virtual ~SoraPhysicBody();
        
        const SoraPhysicBodyDef& getDef() const;
    
        void        setLocalAnchor(const SoraVector& anchor);
        SoraVector  getLocalAnchor() const;
        
        void        setUserData(void* data);
        void*       getUserData() const;
        
        virtual void render();
        
        virtual void createFixture(const SoraPhysicFixtureDef& def) = 0;
        
        virtual void setMass(float mass, float centerX, float centerY) = 0;
        virtual float getMass() const = 0;
        
        virtual void setSpeed(float x, float y) = 0;
        virtual SoraVector getSpeed() const = 0;
        
        virtual void setAngularSpeed(float r) = 0;
        virtual float getAngularSpeed() const = 0;
        
        virtual void setTransform(float x, float y, float rot) = 0;
        virtual float getRotation() const = 0;
        virtual SoraVector getPosition() const = 0;        
        
        virtual void setActive(bool flag) = 0;
        virtual bool isActive() const = 0;
        
        virtual void setSleep(bool flag) = 0;
        virtual bool isSleep() const = 0;
        
        virtual void setBullet(bool flag) = 0;
        virtual bool isBullet() const = 0;
        
        virtual SoraVector getCenter() const = 0;
        virtual SoraVector getLocalCenter() const = 0;
        virtual SoraVector worldToLocal(const SoraVector& vec) const = 0;
        
        virtual void applyForce(float fx, float fy) = 0;
        virtual void applyForce(float fx, float fy, float px, float py) = 0;
        
        virtual void applyTorque(float t) = 0;
        virtual void applyLinearImpulse(float fx, float fy, float px, float py) =0;
        virtual void applyAngularImpulse(float p) = 0;
        
        virtual SoraRect getBoundingBox() = 0;
        
    protected:
        SoraPhysicBodyDef mDef;
        SoraVector mLocalAnchor;
        
        void* mUserData;
    };
    
    
} // namespace sora



#endif
