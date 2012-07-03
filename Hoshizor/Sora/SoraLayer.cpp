//
//  SoraLayer.cpp
//  Sora
//
//  Created by Robert Bu on 7/18/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraLayer.h"

namespace sora {
    
    SoraLayer::SoraLayer(int32 layerDepth):
    mLayerDepth(layerDepth) {
        
    }
    
    SoraLayer::~SoraLayer() {
     //   mSubObjects = NULL;
    }
    
    SoraLayer& SoraLayer::combine(const SoraLayer& layer) {
        SoraObject* obj = layer.getObjList();
        while(obj != NULL) {
            SoraObject::add(obj);
            obj = obj->getNext();
        }
		return *this;
    }
    
    SoraLayer& SoraLayer::removeDuplicate(const SoraLayer& layer) {
        SoraObject* obj = layer.getObjList();
        while(obj != NULL) {
            SoraObject::del(obj);
            obj = obj->getNext();
        }
		return *this;
    }
    
    void SoraLayer::setLayerDepth(int32 depth) {
        mLayerDepth = depth;
    }
    
    void SoraLayer::swap(SoraLayer& layer) {
        SoraObject* objA = getObjList();
        SoraObject* objB = layer.getObjList();
        
        delAll();
        layer.delAll();
        
        while(objA != NULL) {
            layer.add(objA);
            objA = objA->getNext();
        }
        
        while(objB != NULL) {
            SoraObject::add(objB);
            objB = objB->getNext();
        }
    }
    
    SoraLayer* SoraLayer::clone() {
        SoraLayer* layer = new SoraLayer(mLayerDepth);
        SoraObject* obj = getObjList();
        while(obj != NULL) {
            layer->add(obj);
            obj = obj->getNext();
        }
        return layer;
    }
    
} // namespace sora
