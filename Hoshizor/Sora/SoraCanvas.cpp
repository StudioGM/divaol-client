/*
 *  SoraCanvas.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/7/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraCanvas.h"
#include "SoraCore.h"
#include "SoraModifierAdapter.h"
#include "SoraLogger.h"

namespace sora {
        
    SoraBaseCanvas::SoraBaseCanvas(int32 w, int32 h, bool bDepthBuffer) {
        canvasTarget = SoraCore::Ptr->createTarget(w, h, bDepthBuffer);
		if(!canvasTarget) {
			log_mssg("SoraBaseCanvas: Error creating render target");
            return;
        }
        pCanvasSprite = new SoraSprite(0);
		  
		if(!canvasTarget || !pCanvasSprite) {
            log_mssg("SoraBaseCanvas: Error creating render target");
            return;
        }
    }
    
    SoraBaseCanvas::~SoraBaseCanvas() {
        if(canvasTarget)
            SoraCore::Ptr->freeTarget(canvasTarget);
        if(pCanvasSprite)
		      delete pCanvasSprite;
    }
    
    void SoraBaseCanvas::on3DEnabled(bool flag) {
        pCanvasSprite->enable3D(flag);
    }
    
    void SoraBaseCanvas::render() {
        if(pCanvasSprite) {
            if(!is3DEnabled()) {
                pCanvasSprite->setTexture(SoraCore::Ptr->getTargetTexture(canvasTarget));
                pCanvasSprite->render(getPositionX(), getPositionY());
            } else {
                pCanvasSprite->setTransform(getTransform());
                pCanvasSprite->render();
            }
        }
    }
    
    int32 SoraBaseCanvas::update(float dt) {
        if(pCanvasSprite)
            pCanvasSprite->update(dt);
		return 0;
    }
    
    SoraSprite* SoraBaseCanvas::getCanvasSprite() const { 
        if(pCanvasSprite)
            pCanvasSprite->setTexture(SoraCore::Ptr->getTargetTexture(canvasTarget));
        return pCanvasSprite;
    }
    
    void SoraBaseCanvas::beginRender(uint32 col) {
        if(canvasTarget) {
            bSceneBegan = true;
            SoraCore::Ptr->beginScene(col, canvasTarget);
        }
    }
    
    void SoraBaseCanvas::finishRender() {
        if(canvasTarget) {
            if(!bSceneBegan)
                THROW_SORA_EXCEPTION(IllegalStateException, "Render not began");
            SoraCore::Ptr->endScene();
        }
    }
      
} // namespace sora