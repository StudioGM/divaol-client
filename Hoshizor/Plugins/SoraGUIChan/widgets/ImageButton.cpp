//
//  ImageButton.cpp
//  Sora
//
//  Created by Ruiwei Bu on 8/28/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "ImageButton.h"

namespace sora {
    
    namespace gui {
        
        std::string ImageButton::IdTex              = "sora_imgbtn_tex";
        std::string ImageButton::IdTexNormal        = "sora_imgbtn_tex_normal";
        std::string ImageButton::IdTexHover         = "sora_imgbtn_tex_hover";
        std::string ImageButton::IdTexClick         = "sora_imgbtn_tex_click";
        std::string ImageButton::IdTexFocus         = "sora_imgbtn_tex_focus";
        std::string ImageButton::IdTexUnavail       = "sora_imgbtn_tex_unavail";
        std::string ImageButton::IdRectNormal       = "sora_imgbtn_rect_normal";
        std::string ImageButton::IdRectHover        = "sora_imgbtn_rect_hover";
        std::string ImageButton::IdRectClick        = "sora_imgbtn_rect_click";
        std::string ImageButton::IdRectFocus        = "sora_imgbtn_rect_focus";
        std::string ImageButton::IdRectUnavail      = "sora_imgbtn_rect_unavail";
        std::string ImageButton::IdColorNormal      = "sora_imgbtn_color_normal";
        std::string ImageButton::IdColorHover       = "sora_imgbtn_color_hover";
        std::string ImageButton::IdColorClick       = "sora_imgbtn_color_click";
        std::string ImageButton::IdColorFocus       = "sora_imgbtn_color_focus";
        std::string ImageButton::IdColorUnavail     = "sora_imgbtn_color_unavail";
        
        ImageButton::ImageButton():
        mBtnTex(0),
        mBtnTexNormal(0),
        mBtnTexHover(0),
        mBtnTexClick(0),
        mBtnTexUnavail(0),
        mRenderSeperate(false) {
            gcn::Style* style = getStyle();
            
            mBtnTex = style->image().get(IdTex);
            if(mBtnTex) {
                if(style->hasRect(IdRectNormal)) 
                    mBtnRectNormal = style->getRect(IdRectNormal);
                else
                    mBtnRectNormal = gcn::Rectangle(0,0,32,32);
               
                if(style->hasRect(IdRectHover))
                   mBtnRectHover = style->getRect(IdRectHover);
                else
                   mBtnRectHover = mBtnRectNormal;
                   
                if(style->hasRect(IdRectClick)) 
                    mBtnRectClick   = style->getRect(IdRectClick);
                else
                    mBtnRectClick = mBtnRectNormal;
                
                if(style->hasRect(IdRectUnavail))
                    mBtnRectUnavail = style->getRect(IdRectUnavail);
                else
                    mBtnRectUnavail = mBtnRectNormal;
                
                if(style->hasRect(IdRectFocus))
                    mBtnRectFocus = style->getRect(IdRectFocus);
                else
                    mBtnRectFocus = mBtnRectNormal;
                
                mRenderSeperate = false;
            } else {
                mBtnTexNormal = style->image().get(IdTexNormal);
                mBtnTexHover = style->image().get(IdTexHover);
                mBtnTexClick = style->image().get(IdTexClick);
                mBtnTexUnavail = style->image().get(IdTexUnavail);
                mBtnTexFocus = style->image().get(IdTexFocus);
                
                mRenderSeperate = true;
            }
            
            mBtnColorNormal = style->getColor(IdColorNormal);
            mBtnColorHover = style->getColor(IdColorHover);
            mBtnColorClick = style->getColor(IdColorNormal);
            mBtnColorUnavail = style->getColor(IdColorNormal);
            mBtnColorFocus = style->getColor(IdColorFocus);
        }
        
        void ImageButton::draw(gcn::Graphics* graphics) {
            if(!mRenderSeperate) {
                if(mBtnTex) {
                    if(isEnabled()) {
                        if(isPressed()) {
                            graphics->setColor(mBtnColorClick);
                            mBtnTex->setTextureRect(mBtnRectClick);
                        } else if(isHasMouse()) {
                            graphics->setColor(mBtnColorHover);
                            mBtnTex->setTextureRect(mBtnRectHover);
                        } else if(isFocused()) {
                            graphics->setColor(mBtnColorFocus);
                            mBtnTex->setTextureRect(mBtnRectFocus);
                        } else {
                            graphics->setColor(mBtnColorNormal);
                            mBtnTex->setTextureRect(mBtnRectNormal);
                        }
                    } else {
                        graphics->setColor(mBtnColorUnavail);
                        mBtnTex->setTextureRect(mBtnRectUnavail);
                    }
                    graphics->drawImage(mBtnTex, getX(), getY());
                }
            } else {
                if(isEnabled()) {
                    if(isPressed()) {
                        if(mBtnTexClick) {
                            graphics->setColor(mBtnColorClick);
                            graphics->drawImage(mBtnTexClick, getX(), getY());
                        }
                    } else if(isHasMouse()) {
                        if(mBtnTexHover) {
                            graphics->setColor(mBtnColorHover);
                            graphics->drawImage(mBtnTexHover, getX(), getY());
                        }
                    } else if(isFocused()) {
                        if(mBtnTexFocus) {
                            graphics->setColor(mBtnColorFocus);
                            graphics->drawImage(mBtnTexFocus, getX(), getY());
                        }
                    } else {
                        if(mBtnTexNormal) {
                            graphics->setColor(mBtnColorNormal);
                            graphics->drawImage(mBtnTexNormal, getX(), getY());
                        }
                    }
                } else {
                    if(mBtnTexUnavail) {
                        graphics->setColor(mBtnColorUnavail);
                        graphics->drawImage(mBtnTexUnavail, getX(), getY());
                    }
                }
            }
        }
    
    } // namespace gui
    
} // namespace sora
