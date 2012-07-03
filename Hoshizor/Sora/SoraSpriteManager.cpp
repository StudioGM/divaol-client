//
//  File.cpp
//  Sora
//
//  Created by Ruiwei Bu on 10/6/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraSpriteManager.h"
#include "SoraCore.h"
#include "SoraSprite.h"
#include "SoraFileSystem.h"

namespace sora {
    
    SoraSpriteManager::SoraSpriteManager() {
        mResourceFileFinder = SoraCore::Instance()->getFileSystem();
        sora_assert(mResourceFileFinder);
        
        mSprite = new SoraSprite;
        sora_assert(mSprite);
    }
    
    void SoraSpriteManager::render(const StringType& path, float x, float y) {
        SoraTextureHandle tex = mResourceFileFinder->getTextureResource(path);
        if(tex) {
            mSprite->setTexture(tex);
            mSprite->render(x, y);
        }
    }
    
    void SoraSpriteManager::render(const StringType& path, float x, float y, float rot, float scalex, float scaley) {
        SoraTextureHandle tex = mResourceFileFinder->getTextureResource(path);
        if(tex) {
            mSprite->setTexture(tex);
            mSprite->setRotation(rot);
            mSprite->setScale(scalex, scaley);
            mSprite->render(x, y);
        }
    }
    
    
} // namespace sora
