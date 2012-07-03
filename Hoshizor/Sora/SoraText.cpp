//
//  SoraText.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/17/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraText.h"
#include "SoraSprite.h"
#include "SoraCore.h"
#include "SoraRenderSystem.h"
#include "SoraFont.h"

namespace sora {
    
    SoraText::SoraText():
    mFont(0),
    mFontSize(20),
    mStyle(SoraFont::AlignmentLeft),
    mColor(SoraColorRGBA()),
    mRotation(0.f),
    mCharRotation(0.f),
    mText(StringType()),
    mTarget(0),
    mTextSprite(0) {
        mFont = SoraCore::Instance()->getSystemFont();
        if(mFont)
            mFontSize = mFont->getFontSize();
    }
    
    SoraText::SoraText(const SoraWString& str, SoraFont* font):
    mFont(font),
    mFontSize(font->getFontSize()),
    mStyle(SoraFont::AlignmentLeft),
    mColor(SoraColorRGBA()),
    mRotation(0.f),
    mCharRotation(0.f),
    mText(str),
    mTarget(0),
    mTextSprite(0) {
        sora_assert(font);
    }
    
    SoraText::~SoraText() {
        release();
    }
    
    void SoraText::set(const SoraWString& str, SoraFont* font) {
        mText = str;
        mFont = font;
        
        if(mRenderToSprite && mTarget && mTextSprite) {
            spriteRender();
        }
    }
    
    void SoraText::setText(const SoraWString& str) {
        mText = str;
        
        if(mRenderToSprite && mTarget && mTextSprite) {
            spriteRender();
        }
    }
    
    void SoraText::setFont(SoraFont* font) {
        mFont = font;
        
        if(mRenderToSprite && mTarget && mTextSprite) {
            spriteRender();
        }
    }
    
    void SoraText::setStyle(SoraFont::Alignment style) {
        mStyle = style;
        
        if(mRenderToSprite && mTarget && mTextSprite) {
            spriteRender();
        }
    }
    
    void SoraText::setColor(const SoraColorRGBA& col) {
        mColor = col;
        
        if(mRenderToSprite && mTarget && mTextSprite) {
            spriteRender();
        }
    }
    
    void SoraText::setRotation(float rotation) {
        mRotation = rotation;
        
        if(mRenderToSprite && mTarget && mTextSprite) {
            spriteRender();
        }
    }
    
    void SoraText::setCharRotation(float rotation) {
        mCharRotation = rotation;
        
        if(mRenderToSprite && mTarget && mTextSprite) {
            spriteRender();
        }
    }
    
    float SoraText::getRotation() const {
        return mRotation;
    }
    
    float SoraText::getCharRotation() const {
        return mCharRotation;
    }
    
    const SoraColorRGBA& SoraText::getColor() const {
        return mColor;
    }
    
    SoraFont::Alignment SoraText::getStyle() const {
        return mStyle;
    }
    
    const SoraWString& SoraText::getText() const {
        return mText;
    }
    
    SoraFont* SoraText::getFont() const {
        return mFont;
    }
    
    void SoraText::render() {        
        if(!is3DEnabled()) {
            if(mTarget && mTextSprite) {
                mTextSprite->render(getPositionX(), getPositionY());
            }
            else if(mFont) {
                mFont->setCharRotation(mCharRotation);
                mFont->setLineRotation(mRotation);
                mFont->setColor(mColor.getHWColor());
                mFont->print(getPositionX(), getPositionY(), mStyle, mText.c_str());
            }
        } else {
            if(mTarget && mTextSprite) {
                mTextSprite->enable3D(true);
                mTextSprite->setTransform(getTransform());
                mTextSprite->render();
                mTextSprite->enable3D(false);
            }
            else if(mFont) {
                mFont->setCharRotation(mCharRotation);
                mFont->setLineRotation(mRotation);
                mFont->setColor(mColor.getHWColor());
                mFont->renderIn3D(getPositionX(), getPositionY(), mStyle, mText.c_str(), getTransform().getTransformMatrix());
            }
        }
    }
    
    void SoraText::renderTo(float x, float y) {
        if(mTarget && mTextSprite) {
            mTextSprite->render(x, y);
        }
        else if(mFont) {
            mFont->setCharRotation(mCharRotation);
            mFont->setLineRotation(mRotation);
            mFont->setColor(mColor.getHWColor());
            mFont->print(x, y, mStyle, mText.c_str());
        }
    }
    
    void SoraText::release() {
        if(mTarget) {
            SoraCore::Instance()->freeTarget(mTarget);
        }
        if(mTextSprite) {
            delete mTextSprite;
            mTextSprite = 0;
        }
    }
    
    void SoraText::spriteRender() {
        SoraVector dimensions = mFont->getStringDimensions(mText.c_str());

        SoraCore::Instance()->beginScene(0x00000000, mTarget);
        
        if(mFont) {
            mFont->setCharRotation(mCharRotation);
            mFont->setLineRotation(mRotation);
            mFont->setColor(mColor.getHWColor());
            if(mStyle != SoraFont::AlignmentLeft) {
                if(mStyle == SoraFont::AlignmentRight) {
                    mFont->print(dimensions.x, 0.f, mStyle, mText.c_str());
                } else {
                    mFont->print((float)((int32)dimensions.x >> 1), 0.f, mStyle, mText.c_str());
                }
            } else {
                mFont->print(0.f, 0.f, mStyle, mText.c_str());
            }
        }
        
        SoraCore::Instance()->endScene();
    }
    
    void SoraText::enableRenderToSprite(bool flag) {
        mRenderToSprite = flag;
        if(mTarget != 0 || mTextSprite != 0) {
            release();
        }
      
        if(mRenderToSprite) {
            if(mFont && !mText.empty()) {
                SoraVector dimensions = mFont->getStringDimensions(mText.c_str());
                
                mTarget = SoraCore::Instance()->createTarget(dimensions.x,
                                                             dimensions.y);
                if(mTarget) {
                    // render the text
                    spriteRender();
                    
                    mTextSprite = new SoraSprite(SoraCore::Instance()->getTargetTexture(mTarget));
                    sora_assert(mTextSprite);
                }
            }
        }
    }
    
    SoraSprite* SoraText::getSprite() const {
        return mTextSprite;
    }
    
    SoraRect SoraText::getBoundingBox() const {
        if(mTarget != 0 && mTextSprite != 0) {
            return SoraRect(getPositionX(),
                            getPositionY(),
                            getPositionX()+mTextSprite->getSpriteWidth(),
                            getPositionY()+mTextSprite->getSpriteHeight());
        } else if(mFont) {
            return SoraRect(getPositionX(), 
                            getPositionY(),
                            mFont->getStringWidth(mText.c_str()),
                            mFont->getHeight());
        }
        return SoraRect();
                      
    }
    
    void SoraText::Render(const SoraWString& str, float x, float y) {
        SoraFont* font = SoraCore::Instance()->getSystemFont();
        if(font != 0) {
            font->render(x, y, str.c_str());
        }
    }
    
} // namespace sora