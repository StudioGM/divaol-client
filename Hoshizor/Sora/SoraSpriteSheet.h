//
//  SoraSpriteSheet.h
//  Sora
//
//  Created by Ruiwei Bu on 10/7/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraSpriteSheet_h
#define Sora_SoraSpriteSheet_h

#include "SoraPlatform.h"
#include "SoraUncopyable.h"
#include "SoraString.h"
#include "SoraShaderable.h"
#include "SoraResource.h"
#include "math/SoraRect.h"

namespace sora {
    
    class SoraSprite;
    
    class SoraSpriteSheet: public SoraResource {
    public:
        SoraSpriteSheet();
        SoraSpriteSheet(const StringType& path);
        virtual ~SoraSpriteSheet();
        
        void release();
        
        bool load(const StringType& path);
        bool loadFromMemory(void* data, uint32 size);
        
        void render(const StringType& tag, float x, float y);
        
        SoraSprite* getSprite(const StringType& tag);
        
    private:
        SoraTextureHandle mTexture;
        
        struct SheetSpriteInfo {
            SoraRect    mTextureRect;
            SoraSprite* mSprite;
            StringType  mTag;
        };
        
        typedef sora_hash_map<std::string, SheetSpriteInfo> SheetInfo;
        SheetInfo mSheet;
    };
    
} // namespace sora


#endif
