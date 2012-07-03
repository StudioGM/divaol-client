/*
 *  Sora2DCamera.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/20/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraCamera.h"
#include "SoraCore.h"
#include "SoraRenderSystem.h"
#include "Modifiers/SoraCameraModifiers.h"

namespace sora {

    Sora2DCamera::Sora2DCamera(float x, float y, float viewWidth, float viewHeight):
    mHZoom(1.f),
    mVZoom(1.f),
    mRotation(0.f),
    mPropChanged(true){
        setPosition(x, y);
        mViewRect.set(x, y, viewWidth, viewHeight);
    }
    
    Sora2DCamera::Sora2DCamera():
    mHZoom(1.f),
    mVZoom(1.f),
    mRotation(0.f),
    mPropChanged(true) {
        mViewRect.set(0.f, 0.f, (float)SoraCore::Ptr->getScreenWidth(), (float)SoraCore::Ptr->getScreenHeight());
    }
    
    Sora2DCamera::~Sora2DCamera() {
        
    }
    
    void Sora2DCamera::setViewRect(float width, float height) {
        mViewRect.set(getPositionX(),
                      getPositionY(),
                      width,
                      height);
        mPropChanged = true;
    }
    float Sora2DCamera::getViewWidth() const {
        return mViewRect.x2;
    }
    
    float Sora2DCamera::getViewHeight() const {
        return mViewRect.y2;
    }
    
    void Sora2DCamera::setZoom(float h, float v) {
        if(mHZoom != h || mVZoom != v) {
            mHZoom = h;
            mVZoom = v;
            mPropChanged = true;
        }
    }
    
    void Sora2DCamera::setRotation(float rot) {
        if(mRotation != rot) {
            mRotation = rot;
            mPropChanged = true;
        }
    }
    
    void Sora2DCamera::getZoom(float* h, float* v) {
        *h = mHZoom;
        *v = mVZoom;
    }
    
    void Sora2DCamera::setPosition(float x, float y) {
        if(getPositionX() != x || getPositionY() != y) {
            mPropChanged = true;
            SoraObject::setPosition(x, y);
        }
    }
    
    float Sora2DCamera::getRotation() const {
        return mRotation;
    }
    
    float Sora2DCamera::getHZoom() const {
        return mHZoom;
    }
    
    float Sora2DCamera::getVZoom() const {
        return mVZoom;
    }

    int32 Sora2DCamera::update(float dt) {
        if(mPropChanged) {
            sora::SoraCore::Ptr->setTransform(getPositionX(), getPositionY(), 0.f, 0.f, mRotation, mHZoom, mVZoom);
            sora::SoraCore::Ptr->setClipping((int32)getPositionX(), (int32)getPositionY(), (int32)mViewRect.x2, (int32)mViewRect.y2);
            mPropChanged = false;
        }
		return 0;
    }
    
    void Sora2DCamera::render() {
        if(getObjSize() != 0) {
            sora::SoraCore::Ptr->pushTransformMatrix();
            sora::SoraCore::Ptr->pushClippingMatrix();
            apply();
            SoraObject::render();
            sora::SoraCore::Ptr->popTransformMatrix();
            sora::SoraCore::Ptr->popClippingMatrix();
        }
    }
    
    void Sora2DCamera::apply() {
        sora::SoraCore::Ptr->setTransform(getPositionX(), getPositionY(), 0.f, 0.f, mRotation, mHZoom, mVZoom);
        sora::SoraCore::Ptr->setClipping((int32)getPositionX(), (int32)getPositionY(), (int32)mViewRect.x2, (int32)mViewRect.y2);
    }
    
    void Sora2DCamera::moveTo(float x, float y, float inTime) {
        CreateModifierAdapter(this, 
                              new SoraCameraPositionModifier(getPositionX(),
                                                             getPositionY(),
                                                             x, 
                                                             y,
                                                             inTime));

    }
    
    void Sora2DCamera::rotateTo(float newRot, float inTime) {
        CreateModifierAdapter(this,
                              new SoraCameraRotationModifier(mRotation,
                                                             newRot,
                                                             inTime));
    }

    void Sora2DCamera::zoomTo(float hz, float vz, float inTime) {
        CreateModifierAdapter(this,
                              new SoraCameraZoomModifier(mHZoom,
                                                         mVZoom,
                                                         hz,
                                                         vz,
                                                         inTime));
                                              
    }
    
    void Sora2DCamera::transformTo(Sora2DCamera* r, float inTime) {
        assert(r);
        moveTo(r->getPositionX(), r->getPositionY(), inTime);
        rotateTo(r->getRotation(), inTime);
        zoomTo(r->mHZoom, r->mVZoom, inTime);
    }
    
} // namespace sora