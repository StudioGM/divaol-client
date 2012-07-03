//
//  SoraSpriteFactory.h
//  Sora
//
//  Created by Robert Bu on 8/19/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraSpriteFactory_h
#define Sora_SoraSpriteFactory_h

#include "SoraFactory.h"
#include "SoraSprite.h"
#include "SoraString.h"

namespace sora {
    
    /**
     * Global Factories for Creating Sora Objects
     * Users can register their types
     * Some plugins such as SoraSpriteAnimation and SoraGifSprite 
     *  would register their type into the factory 
     **/
    
    /**
     * Global Sprite type factory
     * All sprite creation in core goes there
     **/
    
    class SoraConfigParser;
    
    typedef SoraAbstractFactory<SoraSprite, SoraSprite*(const StringType&, SoraConfigParser*)> SoraSpriteFactory;
        
#define GlobalSpriteFactory \
    SoraSpriteFactory::Instance()
    
#define REGISTER_SPRITE_PRODUCT(name, fn) \
    GlobalSpriteFactory->reg(name, fn);
    
    template<typename T>
    static void RegisterSpriteType(const std::string& type, const T& fn) {
        SoraSpriteFactory::Instance()->reg(type, fn);
    }
    
    static SoraSprite* CreateSpriteType(const std::string& type, const StringType& path, SoraConfigParser* val=0) {
        return SoraSpriteFactory::Instance()->createInstance(type, path, val);
    }
       
}

#endif
