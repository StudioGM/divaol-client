//
//  SoraScene.h
//  Sora
//
//  Created by Robert Bu on 7/17/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraScene_h
#define Sora_SoraScene_h

/**
 *  A simple scene implemention
 *  All objects in the scene would be updated and rendered when the scene
 *  is updated or rendered
 *  All subclasses of SoraObject in the scene would calculate their position depend on the position
 *  of the scene
 **/

#include "SoraPlatform.h"
#include "SoraObject.h"
#include "SoraCanvas.h"
#include "SoraLayer.h"
#include "SoraCamera.h"

#include <map>

namespace sora {
    
    class SORA_API Sora2DScene: public SoraObject {
    public:
        Sora2DScene(int32 width, int32 height);
        ~Sora2DScene();
    
        /**
         *  add function inherited from SoraObject
         *  would use obj->getName() as the name of the object
         *  add layerDepth = 0(bottom layer) for layer 
         *  if the object being added is a scene
         *  would treat as subscene
         **/
        void add(SoraObject* obj);
        /**
         *  Add a object to the scene
         *  @param obj, object to add
         *  @param layerDepth, the layer depth of a layer, if the layer does not exist, then 
                a new layer with layerDepth would be added to the scene
         **/
        void add(SoraObject* obj, int32 layerDepth);
        
        
        void del(SoraObject* obj, int32 layer);
        
        /**
         *  Add a layer to the scene
         *  @param layer, the layer to be added
         *  @retval, the new layer
         **/
        SoraLayer* addLayer(int32 layerDepth);
        
        /**
         *  Remove a layer from the scene
         *  Would remove all objects attached to the layer
         *  @param layerDepth, the depth of the layer to be removed
         **/
        void removeLayer(int32 layerDepth);
        
        /**
         *  Set the depth of the layer
         *  Would adjust render sequence
         *  Notice that use SoraLayer::setLayerDepth wouldn't have any effect on the scene
         *  @param layerDepth, the depth of the layer to be set
         *  @param newDepth, the new depth for the layer
         **/
        void setLayerDepth(int32 layerDepth, int32 newDepth);
        
        /**
         *  Combine two layers together
         *  The depth would be the depth of the first layer
         *  @param layer1, the first layer to combine
         *  @param layer2, the second layer to combine
         **/
        void combineLayer(int32 layer1, int32 layer2);
        
        SoraLayer* getLayer(int32 layerDepth) const;
        SoraLayer* operator[](int32 layerDepth) const;
        
		virtual void    render();
		virtual int32   update(float dt);
        
        int32	getWidth() const;
        int32	getHeight() const;
        void    adjustSize(int32 width, int32 height);
        
        void    onParentChange(SoraObject* obj);
        
        void    setRotation(float rot);
        float   getRotation() const;
        
        void    setScale(float scaleh, float scalev);
        void    getScale(float* scaleh, float* scalev);
        
        void    moveTo(float x, float y, float inTime);
        
        void    onPositionChange(float x, float y);
        
        void    enableRenderToCanvas(bool flag);
        bool    isRenderToCanvasEnabled() const;
        SoraBaseCanvas* getCanvas() const;
        
        void          setCamera(Sora2DCamera* camera);
        Sora2DCamera* getCamera() const;
        
    private:
        void _adjustSize();
        
        int32 mWidth, mHeight;
        int32 mRealWidth, mRealHeight;
        
        float mRotation;
        float mHScale, mVScale;
                        
        typedef std::map<int32, SoraLayer*> LayerMap;
        LayerMap mLayers;
        
        Sora2DScene* mParentScene;
        Sora2DCamera* mCamera;

        bool mRenderToCanvas;
        SoraBaseCanvas* mCanvas;
    };
    
    
} // namespace sora


#endif
