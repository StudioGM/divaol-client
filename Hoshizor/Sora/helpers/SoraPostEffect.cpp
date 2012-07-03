//
//  SoraPostEffect.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/18/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraPostEffect.h"
#include "SoraSprite.h"
#include "SoraCore.h"
#include "function/SoraBind.h"

namespace sora {
    
    SoraPostEffect::SoraPostEffect():
    mSprite(0) {
        mSprite = new SoraSprite(0);
        sora_assert(mSprite);
        
        attach();
        
        sora::SoraCore::Instance()->enableCoreFeature(SoraCore::FeatureRenderToBuffer);
    }
    
    SoraPostEffect::~SoraPostEffect() {
        if(mSprite)
            delete mSprite;
        sora::SoraCore::Instance()->unregisterFullscreenBufferDelegate();
    }
    
    void SoraPostEffect::attach() {
        sora::SoraCore::Instance()->registerFullscreenBufferDelegate(Bind(this, &SoraPostEffect::onBufferRender));
    }
    
    void SoraPostEffect::onBufferRender(SoraTextureHandle texture) {
        if(mSprite->getTexture() == 0)
            mSprite->setTexture(texture);
        
        this->attachShaderToRender();
        mSprite->render();
        this->detachShaderFromRender();
    }
    
    SoraSprite* SoraPostEffect::getSprite() const {
        return mSprite;
    }
    
} // namespace sora
