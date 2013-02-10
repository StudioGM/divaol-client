//
//  ImageButton.h
//  Sora
//
//  Created by Robert Bu on 8/28/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_ImageButton_h
#define Sora_ImageButton_h

#include "guichan.hpp"

namespace sora {
    
    namespace gui {
        
        /**
         * Button that renders itself according to style configuration
         * Config Keys: 
         * @Image sora_imgbtn_tex
         * @Image sora_imgbtn_tex_normal
         * @Image sora_imgbtn_tex_hover
         * @Image sora_imgbtn_tex_click
         * @Image sora_imgbtn_tex_unavail
         * @Color sora_imgbtn_color_hover
         * @color sora_imgbtn_color_normal
         * @color sora_imgbtn_color_click
         * @color sora_imgbtn_color_unavail
         * @Rect  sora_imgbtn_rect_hover
         * @Rect  sora_imgbtn_rect_normal
         * @Rect  sora_imgbtn_rect_click
         * @Rect  sora_imgbtn_rect_unavail
         *
         * Note that if sora_imgbtn_tex is available
         * Then Texture Rects are used to render the button
         * Otherwise would use different textures
         **/
        
        class ImageButton: public gcn::Button {
        public:
            static std::string IdTex;
            static std::string IdTexNormal;
            static std::string IdTexHover;
            static std::string IdTexClick;
            static std::string IdTexUnavail;
            static std::string IdTexFocus;
            static std::string IdRectNormal;
            static std::string IdRectHover;
            static std::string IdRectClick;
            static std::string IdRectUnavail;
            static std::string IdRectFocus;
            static std::string IdColorNormal;
            static std::string IdColorHover;
            static std::string IdColorClick;
            static std::string IdColorUnavail;
            static std::string IdColorFocus;
            
            ImageButton();
            
            virtual void draw(gcn::Graphics* graphics);
            
        private:
            gcn::Image* mBtnTex;
            
            gcn::Image* mBtnTexNormal;
            gcn::Image* mBtnTexHover;
            gcn::Image* mBtnTexClick;
            gcn::Image* mBtnTexUnavail;
            gcn::Image* mBtnTexFocus;
            
            gcn::Rectangle mBtnRectNormal;
            gcn::Rectangle mBtnRectHover;
            gcn::Rectangle mBtnRectClick;
            gcn::Rectangle mBtnRectUnavail;
            gcn::Rectangle mBtnRectFocus;
            
            gcn::Color mBtnColorNormal;
            gcn::Color mBtnColorHover;
            gcn::Color mBtnColorClick;
            gcn::Color mBtnColorUnavail;
            gcn::Color mBtnColorFocus;
            
            bool mRenderSeperate;
        };
        
        
    } // namespace gcn
    
    
} // namespace sora



#endif
