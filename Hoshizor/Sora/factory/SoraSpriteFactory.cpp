//
//  SoraGlobalFactory.cpp
//  Sora
//
//  Created by Robert Bu on 8/19/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraSpriteFactory.h"

#include "SoraSprite.h"
#include "SoraCore.h"

#include "SoraConfigUtil.h"

namespace sora {
    
    static SoraSprite* SpriteCreatorFn(const StringType& path, SoraConfigParser* val) {
        SoraSprite* spr = SoraCore::Instance()->createSprite(path);
        
        return spr;
    }
    
    SORA_STATIC_RUN_CODE(REGISTER_SPRITE_PRODUCT("sprite", SpriteCreatorFn));
       
} // namespace sora