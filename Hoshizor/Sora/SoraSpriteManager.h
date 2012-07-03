//
//  SoraSpriteManager.h
//  Sora
//
//  Created by Ruiwei Bu on 10/6/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraSpriteManager_h
#define Sora_SoraSpriteManager_h

#include "SoraPlatform.h"
#include "SoraString.h"
#include "SoraSingleton.h"

namespace sora {
    
    class SoraSprite;
    class SoraFileSystem;
    
    class SORA_API SoraSpriteManager: public SoraSingleton<SoraSpriteManager> {
    private:
        SoraSpriteManager();
        
    public:
        friend class SoraSingleton<SoraSpriteManager>;
        
        void render(const StringType& path, float x, float y);
        void render(const StringType& path, float x, float y, float rot, float scalex, float scaley);
        
    private:
        SoraSprite* mSprite;
        SoraFileSystem* mResourceFileFinder;
    };
    
} // namespace sora

#endif
