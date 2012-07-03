//
//  SoraText.h
//  Sora
//
//  Created by Ruiwei Bu on 9/17/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraText_h
#define Sora_SoraText_h

#include "SoraPlatform.h"
#include "SoraFont.h"
#include "SoraObject.h"
#include "SoraColor.h"
#include "SoraMath.h"
#include "SoraShaderable.h"

namespace sora {
    
    /**
     * A simple wrapper class for rendering texts
     * Can also render to a sprite and release itself
     **/
    
    class SoraSprite;
    
    class SORA_API SoraText: public SoraObject, public SoraShaderable {
    public:
        SoraText();
        SoraText(const SoraWString& str, SoraFont* font);
        virtual ~SoraText();
        
        void set(const SoraWString& str, SoraFont* font);
        
        void setText(const SoraWString& str);
        void setFont(SoraFont* font);
        void setStyle(SoraFont::Alignment style);
        void setColor(const SoraColorRGBA& col);
        void setRotation(float rotation);
        void setCharRotation(float crotation);

        SoraFont::Alignment getStyle() const;
        SoraFont* getFont() const;
        const SoraWString& getText() const;
        const SoraColorRGBA& getColor() const;
        float getRotation() const;
        float getCharRotation() const;
        
        void render();
        void renderTo(float x, float y);
        
        SoraRect getBoundingBox() const;
        
    public:
        
        /**
         * Render to a sprite(if RenderTarget is allowed in the RenderSystem)
         * Not calling this during the normal frame rendering is suggested
         * Suggested for preinitiazing static text(initializing static font texts into sprites etc)
         * Notice the sprite would be released when the text is released(because the release of RenderTarget texture)
         * And after this call, the rendering of the text will change to a sprite rendering and set* function will not work
         * Though some functions would continue to work such as fadeto
         **/
        void enableRenderToSprite(bool flag);
        SoraSprite* getSprite() const;
        
        /**
         * ONLY works when system font had been set in SoraCore
         **/
        static void Render(const SoraWString& str, float x, float y);
        
    private:
        void spriteRender();
        void release();
        
        SoraFont* mFont;
        uint32 mFontSize;
        SoraFont::Alignment mStyle;
        SoraColorRGBA mColor;
        float mRotation;
        float mCharRotation;
        SoraWString mText;
        
        bool mRenderToSprite;
        SoraTargetHandle mTarget;
        SoraSprite* mTextSprite;
    };
    
} // namespace sora



#endif
