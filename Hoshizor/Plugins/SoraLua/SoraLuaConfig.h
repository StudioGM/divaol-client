//
//  SoraLuaConfig.h
//  Sora
//
//  Created by Robert Bu on 8/25/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraLuaConfig_h
#define Sora_SoraLuaConfig_h

// here defines the auto export configuration for lua
// this file maybe updated as there goes more plugins

namespace sora {

    /**
     * export all auto export supporters
     * would override all other symbols
     **/
#define SORA_LUA_AUTO_EXPORT_ALL 0
    
    // SoraEnvValues class -> Env
    
#define SORA_LUA_AUTO_EXPORT_ENV_VAL 1

    // SoraFont -> Font
    
#define SORA_LUA_AUTO_EXPORT_FONT 1

    // SoraGuichan -> Gui
    
#define SORA_LUA_AUTO_EXPORT_GUI 1
    
    // SoraSprite -> Sprite
    
#define SORA_LUA_AUTO_EXPORT_SPRITE 1
    
    // SoraCore class -> Core
    
#define SORA_LUA_AUTO_EXPORT_CORE 1
    
    // constant symbols such as MB_OK defined in Sora
    
#define SORA_LUA_AUTO_EXPORT_SYMBOLS 1
    
    // SoraSpriteAnimation -> AnimatedSprite(this is weird...)

#define SORA_LUA_AUTO_EXPORT_ANIMATED_SPRITE 1
    
    // SoraGifSprite -> spritelib extension
    
#define SORA_LUA_AUTO_EXPORT_GIF_SPRITE 1
    
    // SoraBGMManager class -> BGMManager and SEManager
    
#define SORA_LUA_AUTO_EXPORT_BGM_MANAGER 1
        
} // namespace sora


#endif
