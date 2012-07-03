//
//  SoraSpriteSheet.cpp
//  Sora
//
//  Created by Ruiwei Bu on 10/7/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraSpriteSheet.h"
#include "SoraResourceFile.h"
#include "SoraTexture.h"
#include "SoraSprite.h"
#include "SoraLogger.h"
#include "SoraCore.h"

#include "config/SoraConfigParser.h"

namespace sora {
    
    SoraSpriteSheet::SoraSpriteSheet():
    SoraResource(SoraResource::SpriteSheet),
    mTexture(0) {
        
    }
    
    SoraSpriteSheet::SoraSpriteSheet(const StringType& path):
    SoraResource(SoraResource::SpriteSheet),
    mTexture(0) {
        load(path);
    }
    
    SoraSpriteSheet::~SoraSpriteSheet() {
        release();
    }
    
    void SoraSpriteSheet::release() {
        if(mTexture)
            SoraCore::Instance()->releaseTexture(mTexture);
        
        if(mSheet.size() != 0) {
            SheetInfo::iterator it = mSheet.begin();
            SheetInfo::iterator end = mSheet.end();
            
            for(; it != end; ++it) {
                if(it->second.mSprite != 0) {
                    delete it->second.mSprite;
                }
            }
            mSheet.clear();
        }
    }
    
    bool SoraSpriteSheet::load(const StringType& path) {
        SoraResourceFile data(path);
        if(data.isValid()) {
            return loadFromMemory(data, data.size());
        }
        return false;
    }
    
    bool SoraSpriteSheet::loadFromMemory(void* data, uint32 size) {
        release();
        
        SoraConfigParser parser;
        if(parser.open(data, size)) {
            if(parser.toNode("/spritesheet")) {
                std::string texture = parser.getString("texture");
                
                mTexture = SoraTexture::LoadFromFile(texture);
                if(mTexture) {
                    parser.toFirstChild();
                    do {
                        SheetSpriteInfo info;
                        info.mTextureRect.x1 = parser.getFloat("x");
                        info.mTextureRect.y1 = parser.getFloat("y");
                        info.mTextureRect.x2 = parser.getFloat("w");
                        info.mTextureRect.y2 = parser.getFloat("h");
                        
                        info.mTag = parser.getString("tag");
                        info.mSprite = 0;
                        
                        if(!info.mTag.empty() &&
                           info.mTextureRect.x2 != 0.f &&
                           info.mTextureRect.y2 != 0.f) {
                            mSheet[info.mTag] = info;
                        }
                        
                    } while(parser.toNextChild());
                    
                    return true;
                }
            }
        }
        return false;
    }
    
    void SoraSpriteSheet::render(const StringType& tag, float x, float y) {
        if(mTexture) {
            SheetInfo::iterator it = mSheet.find(tag);
            if(it != mSheet.end()) {
                SheetSpriteInfo& info = it->second;
                if(info.mSprite == 0) {
                    info.mSprite = new SoraSprite(mTexture);
                    sora_assert(info.mSprite);
                    
                    info.mSprite->setTextureRect(info.mTextureRect.x1,
                                                 info.mTextureRect.y1,
                                                 info.mTextureRect.x2,
                                                 info.mTextureRect.y2);
                }
                
                info.mSprite->render(x, y);
            }
        } else
            log_error("SoraSpriteSheet: no texture available, maybe forgot to load sheet?");
    }
    
    SoraSprite* SoraSpriteSheet::getSprite(const StringType& tag) {
        if(mTexture) {
            SheetInfo::iterator it = mSheet.find(tag);
            if(it != mSheet.end()) {
                SheetSpriteInfo& info = it->second;
                if(info.mSprite == 0) {
                    info.mSprite = new SoraSprite(mTexture);
                    sora_assert(info.mSprite);
                    
                    info.mSprite->setTextureRect(info.mTextureRect.x1,
                                                 info.mTextureRect.y1,
                                                 info.mTextureRect.x2,
                                                 info.mTextureRect.y2);
                }
                return info.mSprite;
            }
        } else
            log_error("SoraSpriteSheet: no texture available, maybe forgot to load sheet?");
        return 0;
    }
    
    
} // namespace sora
