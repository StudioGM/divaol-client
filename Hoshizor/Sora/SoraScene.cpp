//
//  Sora2DScene.cpp
//  Sora
//
//  Created by Robert Bu on 7/17/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraScene.h"
#include "SoraCore.h"
#include "SoraLogger.h"
#include "Modifiers/SoraObjectModifiers.h"

namespace sora {

    typedef SoraTransitionModifier<Sora2DScene> ScenePositionModifier;

    Sora2DScene::Sora2DScene(int32 width, int32 height):
    mRealWidth(0),
    mRealHeight(0),
    mRotation(0.f),
    mHScale(1.f),
    mVScale(1.f),
    mParentScene(NULL),
    mCamera(NULL),
    mCanvas(NULL) {
        adjustSize(width, height);
    }
    
    Sora2DScene::~Sora2DScene() {
        LayerMap::iterator itLayer = mLayers.begin();
        while(itLayer != mLayers.end()) {
            delete itLayer->second;
            ++itLayer;
        }
        mLayers.clear();
    }
    
    int32 Sora2DScene::getWidth() const {
        return mWidth;
    }
    
    int32 Sora2DScene::getHeight() const {
        return mHeight;
    }

    void Sora2DScene::adjustSize(int32 width, int32 height) {
        if(width == 0 || height == 0)
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Attempt to create a empty scene with width or height = 0");
        
        mWidth = mRealWidth = width;
        mHeight = mRealHeight = height;
        _adjustSize();
    }
     
    void Sora2DScene::add(SoraObject* obj) {
        Sora2DScene* subScene = dynamic_cast<Sora2DScene*>(obj);
        if(subScene == NULL)
            add(obj, 0);
        else {
            SoraObject::add(subScene);
            if(mCamera)
                subScene->setCamera(mCamera);
        }
    }
    
    void Sora2DScene::del(SoraObject* obj, int32 layer) {
        LayerMap::iterator itLayer = mLayers.find(layer);
        if(itLayer != mLayers.end()) {
            SoraLayer* layer = itLayer->second;
            layer->del(obj);
        }
    }

    
    void Sora2DScene::onParentChange(SoraObject* obj) {
        if(obj == NULL) {
            mParentScene = NULL;
            adjustSize(mRealWidth, mRealHeight);
        }
        
        Sora2DScene* parentScene = dynamic_cast<Sora2DScene*>(obj);
        if(parentScene != NULL) {
            mParentScene = parentScene;
            _adjustSize();
        }
    }
    
    void Sora2DScene::_adjustSize() {
        if(mParentScene) {
            int32 maxRight = (int32)mParent->getPositionX() + mParentScene->getWidth();
            int32 maxBottom = (int32)mParent->getPositionY() + mParentScene->getHeight();
            if(getPositionX() + mWidth > maxRight)
                mWidth = mWidth - ((int32)(getPositionX() + mWidth) - maxRight);
            if(getPositionY() + mHeight > maxBottom)
                mHeight = mHeight - ((int32)(getPositionY() + mHeight) - maxBottom);
        }
    }
    
    void Sora2DScene::add(SoraObject* obj, int32 layerDepth) {
        LayerMap::iterator itLayer = mLayers.find(layerDepth);
        if(itLayer != mLayers.end()) {
            SoraLayer* layer = itLayer->second;
            layer->add(obj);
        } else {
            SoraLayer* layer = addLayer(layerDepth);
            if(layer)
                layer->add(obj);
        }
    }
    
    SoraLayer* Sora2DScene::addLayer(int32 layerDepth) {
        SoraLayer* layer = new SoraLayer(layerDepth);
        if(!layer)
            THROW_SORA_EXCEPTION(RuntimeException, "Error creating new layer");
        else {
            mLayers.insert(std::make_pair(layerDepth, layer));
        }
        return layer;
    }
    
    void Sora2DScene::removeLayer(int32 layerDepth) {
        LayerMap::iterator itLayer = mLayers.find(layerDepth);
        if(itLayer != mLayers.end()) {
            SoraLayer* layer = itLayer->second;
            delete layer;
            mLayers.erase(itLayer);
        }
    }
    
    void Sora2DScene::setLayerDepth(int32 layerDepth, int32 newDepth) {
        LayerMap::iterator itLayer = mLayers.find(layerDepth);
        if(itLayer != mLayers.end()) {
            mLayers.insert(std::make_pair(newDepth, itLayer->second));
            itLayer->second->setLayerDepth(newDepth);
            mLayers.erase(itLayer);
        }
    }
    
    void Sora2DScene::combineLayer(int32 layer1, int32 layer2) {
        LayerMap::iterator itLayerA = mLayers.find(layer1);
        LayerMap::iterator itLayerB = mLayers.find(layer2);
        if(itLayerA != mLayers.end() &&
           itLayerB != mLayers.end()) {
            SoraLayer* layerA = itLayerA->second;
            SoraLayer* layerB = itLayerB->second;
            
            mLayers.erase(itLayerA);
            mLayers.erase(itLayerB);
            
            mLayers.insert(std::make_pair(layerA->getLayerDepth(), layerA));
            mLayers.insert(std::make_pair(layerB->getLayerDepth(), layerB));
        } else {
            log_warning(vamssg("Sora2DScene: Error combining layer %d and %d", layer1, layer2));
        }
    }
    
    SoraLayer* Sora2DScene::getLayer(int32 layerDepth) const {
        LayerMap::const_iterator itLayer = mLayers.find(layerDepth);
        if(itLayer != mLayers.end()) {
            return itLayer->second;
        }
        return NULL;
    }
    
    SoraLayer* Sora2DScene::operator[](int32 layerDepth) const {
        LayerMap::const_iterator itLayer = mLayers.find(layerDepth);
        if(itLayer != mLayers.end()) {
            return itLayer->second;
        }
        return NULL;
    }
    
    void Sora2DScene::render() {
        if(mRenderToCanvas && mCanvas) 
            mCanvas->beginRender();
        
        sora::SoraCore::Ptr->pushTransformMatrix();
        if(!mCamera) {
            sora::SoraCore::Ptr->setTransform(0.f, 
                                              0.f, 
                                              getPositionX(), 
                                              getPositionY(),
                                              mRotation,
                                              mHScale,
                                              mVScale);
        } else {
            sora::SoraCore::Ptr->setTransform(mCamera->getPositionX(), 
                                              mCamera->getPositionY(), 
                                              getPositionX(), 
                                              getPositionY(),
                                              mRotation+mCamera->getRotation(),
                                              mHScale+mCamera->getHZoom()-1.f,
                                              mVScale+mCamera->getVZoom()-1.f);
        }
        
        sora::SoraCore::Ptr->pushClippingMatrix();
        if(!mCamera) {
            sora::SoraCore::Ptr->setClipping((int32)(getPositionX()), 
                                    (int32)(getPositionY()), 
                                    mWidth,
                                    mHeight);
        } else {
            int32 rx = getPositionX() < mCamera->getPositionX() ? (int32)mCamera->getPositionX(): (int32)getPositionX();
            int32 ry = getPositionY() < mCamera->getPositionY() ? (int32)mCamera->getPositionY(): (int32)getPositionY();
            int32 rWidth = (int32)mCamera->getPositionX()+mCamera->getViewWidth()-rx;
            int32 rHeight = (int32)mCamera->getPositionY()+mCamera->getViewHeight()-ry;
            
            sora::SoraCore::Ptr->setClipping(rx,
                                             ry,
                                             rWidth>mWidth?mWidth:rWidth,
                                             rHeight>mHeight?mHeight:rHeight);
        }
        
        LayerMap::iterator itLayer = mLayers.begin();
        while(itLayer != mLayers.end()) {
            itLayer->second->render();
            ++itLayer;
        }
        
        /**
         *  Render SubScenes(If Available)
         **/
        SoraObject::render();
        
        sora::SoraCore::Ptr->popTransformMatrix();
        sora::SoraCore::Ptr->popClippingMatrix();
        
        if(mRenderToCanvas && mCanvas)
            mCanvas->finishRender();
    }

    void Sora2DScene::moveTo(float x, float y, float inTime) {
        CreateModifierAdapter(this, 
                              new ScenePositionModifier(getPositionX(),
                                                        getPositionY(),
                                                        x,
                                                        y,
                                                        inTime));
    }
    
    int32 Sora2DScene::update(float dt) {        
        LayerMap::iterator itLayer = mLayers.begin();
        while(itLayer != mLayers.end()) {
            itLayer->second->update(dt);
            ++itLayer;
        }
        
        /**
         *  Update SubScenes(If Available)
         **/
        SoraObject::update(dt);
        return 0;
    }

    
    void Sora2DScene::setRotation(float rot) {
        mRotation = rot;
    }
    
    float Sora2DScene::getRotation() const {
        return mRotation;
    }
    
    void Sora2DScene::setScale(float scaleh, float scalev) {
        mHScale = scaleh;
        mVScale = scalev;
    }
    
    void Sora2DScene::getScale(float* scaleh, float* scalev) {
        *scaleh = mHScale;
        *scalev = mVScale;
    }
    
    void Sora2DScene::onPositionChange(float x, float y) {
        _adjustSize();
    }
    
    void Sora2DScene::enableRenderToCanvas(bool flag) {
        mRenderToCanvas = flag;
        if(flag) {
            if(!mCanvas) {
                mCanvas = new SoraBaseCanvas(mRealWidth, mRealHeight);
                if(!mCanvas)
                    THROW_SORA_EXCEPTION(RuntimeException, "Error creating canvas");
            }
        }
    }
    
    bool Sora2DScene::isRenderToCanvasEnabled() const {
        return mRenderToCanvas;
    }
    
    SoraBaseCanvas* Sora2DScene::getCanvas() const {
        return mCanvas;
    }
    
    void Sora2DScene::setCamera(Sora2DCamera* camera) {
        mCamera = camera;
        SoraObject* objects = getObjList();
        while(objects != NULL) {
            Sora2DScene* scene = dynamic_cast<Sora2DScene*>(objects);
            if(scene != NULL)
                scene->setCamera(mCamera);
            objects = objects->getNext();
        }
    }
    
    Sora2DCamera* Sora2DScene::getCamera() const {
        return mCamera;
    }
} // namespace sora