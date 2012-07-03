//
//  SoraResource.cpp
//  Sora
//
//  Created by Ruiwei Bu on 10/5/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraResource.h"
#include "SoraHandleManager.h"
#include "SoraCore.h"

#include "SoraTexture.h"
#include "SoraFont.h"
#include "SoraConfigUtil.h"
#include "SoraSoundFile.h"
#include "SoraShader.h"

namespace sora {
    
    SoraResource::Type SoraResource::IdentToType(const std::string& ident) {
        if(ident == "texture")
            return SoraResource::Texture;
        if(ident == "music")
            return SoraResource::Music;
        if(ident == "soundeffect")
            return SoraResource::SoundEffect;
        if(ident == "raw")
            return SoraResource::RawData;
        if(ident == "font")
            return SoraResource::Font;
        if(ident == "config")
            return SoraResource::Config;
        if(ident == "shader")
            return SoraResource::Shader;
        if(ident == "material")
            return SoraResource::Material;
        if(ident == "mesh")
            return SoraResource::Mesh;
        return SoraResource::Invalid;
    }
    
    std::string SoraResource::TypeToIdent(Type type) {
        switch(type) {
            case SoraResource::Texture: return "texture";
            case SoraResource::Music: return "music";
            case SoraResource::SoundEffect: return "soundeffect";
            case SoraResource::RawData: return "raw";
            case SoraResource::Font: return "font";
            case SoraResource::Shader: return "shader";
            case SoraResource::Material: return "material";
            case SoraResource::Mesh: return "mesh";
            default:
                return "unknown";
        }
    }
    
    SoraResource::SoraResource(Type type):
    mType(type),
    mHandle(SoraHandleManager::GetInstance().getUniqueHandle()) {
        
    }
    
    SoraResource::~SoraResource() {
        
    }
    
    SoraResource::Handle SoraResource::getHandle() const {
        return mHandle;
    }
    
    SoraResource::Type SoraResource::getType() const {
        return mType;
    }
    
    SoraTextureResource::SoraTextureResource(SoraTextureHandle tex):
    SoraResource(SoraResource::Texture),
    mTexture(tex) {
        
    }
    
    SoraTextureResource::~SoraTextureResource() {
        if(mTexture)
            SoraTexture::Release(mTexture);
    }
    
    SoraTextureResource::operator SoraTextureHandle() const {
        return mTexture;
    }
    
    SoraTextureHandle SoraTextureResource::getTexture() const {
        return mTexture;
    }
    
    SoraFontResource::SoraFontResource(SoraFont* font):
    SoraResource(SoraResource::Font),
    mFont(font) {
        
    }
    
    SoraFontResource::~SoraFontResource() {
        if(mFont)
            SoraFont::Release(mFont);
    }
    
    SoraFontResource::operator SoraFont*() const {
        return mFont;
    }
    
    SoraFont* SoraFontResource::getFont() const {
        return mFont;
    }
    
    SoraMusicResource::SoraMusicResource(SoraMusicFile* music):
    SoraResource(SoraResource::Music),
    mMusic(music) {
        
    }
    
    SoraMusicResource::~SoraMusicResource() {
        if(mMusic) {
            delete mMusic;
        }
    }
    
    SoraMusicResource::operator SoraMusicFile*() const {
        return mMusic;
    }
    
    SoraMusicFile* SoraMusicResource::getMusic() const {
        return mMusic;
    }
    
    SoraSoundEffectResource::SoraSoundEffectResource(SoraSoundEffectFile* se):
    SoraResource(SoraResource::SoundEffect),
    mSoundEffect(se) {
        
    }
    
    SoraSoundEffectResource::~SoraSoundEffectResource() {
        if(mSoundEffect) {
            delete mSoundEffect;
        }
    }
    
    SoraSoundEffectResource::operator SoraSoundEffectFile*() const {
        return mSoundEffect;
    }
    
    SoraSoundEffectFile* SoraSoundEffectResource::getSoundEffect() const {
        return mSoundEffect;
    }
    
    SoraConfigResource::SoraConfigResource(SoraConfigParser* parser):
    SoraResource(SoraResource::Config),
    mConfig(parser) {
        
    }
    
    SoraConfigResource::~SoraConfigResource() {
        if(mConfig) {
            delete mConfig;
        }
    }
    
    SoraConfigResource::operator SoraConfigParser*() const {
        return mConfig;
    }
    
    SoraConfigParser* SoraConfigResource::getConfig() const {
        return mConfig;
    }
    
    SoraShaderResource::SoraShaderResource(SoraShaderContext* context):
    SoraResource(SoraResource::Shader),
    mShaderContext(context) {
        
    }
    
    SoraShaderResource::~SoraShaderResource() {
        if(mShaderContext)
            delete mShaderContext;
    }
    
    SoraShaderResource::operator SoraShaderContext*() const {
        return mShaderContext;
    }
    
    SoraShaderContext* SoraShaderResource::getShaderContext() const {
        return mShaderContext;
    }
    
    
    
} // namespace sora
