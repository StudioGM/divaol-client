//
//  SoraPhysicalBody.cpp
//  Sora
//
//  Created by Ruiwei Bu on 10/2/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraPhysicBody.h"
#include "SoraCore.h"

namespace sora {
    
    SoraPhysicBody::SoraPhysicBody(const SoraPhysicBodyDef& def):
    mDef(def),
    mUserData(0) {
        
    }
    
    SoraPhysicBody::~SoraPhysicBody() {
        
    }
    
    void SoraPhysicBody::setLocalAnchor(const SoraVector& anchor) {
        mLocalAnchor = anchor;
    }
    
    SoraVector SoraPhysicBody::getLocalAnchor() const {
        return mLocalAnchor;
    }
    
    void* SoraPhysicBody::getUserData() const {
        return mUserData;
    }
    
    void SoraPhysicBody::setUserData(void* data) {
        mUserData = data;
    }
    
    const SoraPhysicBodyDef& SoraPhysicBody::getDef() const {
        return mDef;
    }
    
    void SoraPhysicBody::render() {
        SoraRect bbox = getBoundingBox();
        sora::SoraCore::Instance()->renderBox(bbox.x1,
                                              bbox.y1,
                                              bbox.x2,
                                              bbox.y2,
											  0xFFFFFFFF,
                                              1.f);
    }
    
} // namespace sora