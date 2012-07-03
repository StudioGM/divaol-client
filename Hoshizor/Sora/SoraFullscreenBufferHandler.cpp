//
//  SoraFullscreenBufferHandler.cpp
//  Sora
//
//  Created by Robert Bu on 7/15/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraFullscreenBufferHandler.h"
#include "SoraSprite.h"

namespace sora {
    
    SoraFullscreenBufferHandler::SoraFullscreenBufferHandler() {
        mBufferSprite = new SoraSprite(0);
        assert(mBufferSprite);
    }
    
    SoraFullscreenBufferHandler::~SoraFullscreenBufferHandler() {

    }
    
    void SoraFullscreenBufferHandler::onBufferRender(SoraTextureHandle bufferTex) {
        if(!mDelegate) {    
            if(mBufferSprite) {
                mBufferSprite->setTexture(bufferTex);
                mBufferSprite->render();
            } else {
                mBufferSprite = new SoraSprite(bufferTex);
                if(!mBufferSprite) {
                    THROW_SORA_EXCEPTION(NullPointerException, "No buffer sprite available");
                }
            }
        } else 
            mDelegate(bufferTex);
    }

    void SoraFullscreenBufferHandler::registerDelegate(const DelegateFuncType& delegate) {
        mDelegate = delegate;
    }
    
    void SoraFullscreenBufferHandler::unregisterDelegate() {
        mDelegate.clear();
    }
    
    
    
} // namespace sora
