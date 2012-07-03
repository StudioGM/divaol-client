//
//  SoraResource.h
//  Sora
//
//  Created by Ruiwei Bu on 10/5/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraResource_Interface_h
#define Sora_SoraResource_Interface_h

#include "SoraPlatform.h"
#include "SoraUncopyable.h"
#include "SoraAutoPtr.h"
#include "SoraString.h"

namespace sora {
    
    class SoraFont;
    class SoraConfigParser;
    class SoraMusicFile;
    class SoraSoundEffectFile;
    class SoraShaderContext;
    
    /**
     * A resource is a special kind of object that hold a kind of resource data
     * Such as sprite, fontdata, material
     * Resources would be freed after they dealloced
     **/  
    
    class SORA_API SoraResource: SoraUncopyable {
    public:
        typedef SoraAutoPtr<SoraResource> Ptr;

        enum Type {
            Invalid,
            Font,
            Texture,
            Config,
            Music,
            SoundEffect,
            Shader,
            SpriteSheet,
            Material,
            Mesh,
            RawData,
        };
        static Type IdentToType(const std::string& ident);
        static std::string TypeToIdent(Type type);
        
        SoraResource(Type type);
        virtual ~SoraResource();
        
        typedef SoraHandle Handle;
        
        Type getType() const;
        Handle getHandle() const;
        
    protected:
        Type mType;
        Handle mHandle;
    };
    
    class SORA_API SoraTextureResource: public SoraResource {
    public:
        explicit SoraTextureResource(SoraTextureHandle tex);
        ~SoraTextureResource();
        
        operator SoraTextureHandle() const;
        
        SoraTextureHandle getTexture() const;
        
    private:
        SoraTextureHandle mTexture;
    };
    
    class SORA_API SoraFontResource: public SoraResource {
    public:
        explicit SoraFontResource(SoraFont* font);
        ~SoraFontResource();
        
        operator SoraFont*() const;
        
        SoraFont* getFont() const;
        
    private:
        SoraFont* mFont;
    };
    
    class SORA_API SoraMusicResource: public SoraResource {
    public:
        explicit SoraMusicResource(SoraMusicFile* music);
        ~SoraMusicResource();
        
        operator SoraMusicFile*() const;
        
        SoraMusicFile* getMusic() const;
        
    private:
        SoraMusicFile* mMusic;
    };
    
    class SORA_API SoraSoundEffectResource: public SoraResource {
    public:
        explicit SoraSoundEffectResource(SoraSoundEffectFile* se);
        ~SoraSoundEffectResource();
        
        operator SoraSoundEffectFile*() const;
        
        SoraSoundEffectFile* getSoundEffect() const;
        
    private:
        SoraSoundEffectFile* mSoundEffect;
    };
    
    class SORA_API SoraConfigResource: public SoraResource {
    public:
        explicit SoraConfigResource(SoraConfigParser* parser);
        ~SoraConfigResource();
        
        operator SoraConfigParser*() const;
        
        SoraConfigParser* getConfig() const;
        
    private:
        SoraConfigParser* mConfig;
    };
    
    class SORA_API SoraShaderResource: public SoraResource {
    public:
        explicit SoraShaderResource(SoraShaderContext* context);
        ~SoraShaderResource();
        
        operator SoraShaderContext*() const;
        SoraShaderContext* getShaderContext() const;
        
    private:
        SoraShaderContext* mShaderContext;
    };
    
} // namespace sora


#endif
