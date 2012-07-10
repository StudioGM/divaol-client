//
//  style.h
//  Sora
//
//  Created by Robert Bu on 8/9/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_style_h
#define Sora_style_h

#include "guichan/color.hpp"
#include "guichan/rectangle.hpp"
#include "guichan/platform.hpp"

#include <string>
#include <map>

namespace gcn {
    
    class Image;

    /**
     * The base class contains the style configuration of the gui widgets
     * Such as base color, foreground color etc
     * It can also be configured to contain image paths of widget elements
     * for custom widgets
     *
     * @author Robert Bu(darkfall)
     * @since GuiChan for Hoshizora
     */
    
    /**
     * Struct for holding images
     **/
    struct GCN_CORE_DECLSPEC ImageContainer {
        typedef std::map<std::string, Image*> ImageMap;
        
        Image* get(const std::string& key) const;
        
        Image* load(const std::string& key, const std::string& image);
        
        bool has(const std::string& key) const;
        
        void release(const std::string& key);
       
        void releaseAll();
        
    private:
        ImageMap mImages;
    };
    
    class GCN_CORE_DECLSPEC Style {
    public:
        Style();
        
        /**
         * Get the default global style for all widgets
         * Equals to StyleHolder::Instance().getStyle("default");
         **/
        static Style* DefaultStyle();
        
        /**
         * Sets the global background color of all widgets
         *
         * @param The background color
         */
        void setBackgroundColor(const Color& col);
        
        /**
         * Sets the global foreground color of all widgets
         *
         * @param The foreground color
         */
        void setForegroundColor(const Color& col);
        
        /**
         * Sets the global base color of all widgets
         *
         * @param The base color
         */
        void setBaseColor(const Color& col);
        
        /**
         * Sets the global selection color of all widgets
         *
         * @param The selection color
         */
        void setSelectionColor(const Color& col);
        
        /**
         * Gets the global background color of all widgets
         * 
         */
        Color getBackgroundColor();
        
        /**
         * Gets the global foreground color of all widgets
         */
        Color getForegroundColor();
		
        /**
         * Gets the global base color of all widgets
         */
        Color getBaseColor();
		
        /**
         * Gets the global selection color of all widgets
         */
        Color getSelectionColor();
        
        /**
         * Get a style string with key
         * This is useful for configuring widget appearances
         * @Param the key of the style string
         **/
        std::string getString(const std::string& key) const;
        
        /**
         * Set a style string with key
         * @Param the key of the style string
         * @Param the style string value
         **/
        void setString(const std::string& key, const std::string& val);
        
        /**
         * Check a string available or not
         **/
        bool hasString(const std::string& key) const;
        
        /**
         * Get a style color with key
         **/
        gcn::Color getColor(const std::string& key) const;
        
        /**
         * Set a style color with given key
         **/
        void setColor(const std::string& key, const gcn::Color& col);
        
        /**
         * Check a color available or not
         **/
        bool hasColor(const std::string& key) const;
        
        
        /**
         * Get a rectangle with key
         **/
        gcn::Rectangle getRect(const std::string& key) const;
        
        /**
         * Set a rectangle with given key
         **/
        void setRect(const std::string& key, const gcn::Rectangle& rect);
        
        /**
         * Check a rect available or not
         **/
        bool hasRect(const std::string& key) const;
        
        /**
         * Get the image container
         **/
        ImageContainer& image();
        
    private:
        /**
         * Holds the global foreground color
         */
        Color mGlobalForegroundColor;
        /**
         * Holds the global background color
         */
        Color mGlobalBackgroundColor;
		/**
         * Holds the global base color
         */
        Color mGlobalBaseColor;
        /**
         * Holds the global selection color
         */
        Color mGlobalSelectionColor;
        
        typedef std::map<std::string, std::string> StyleStringMap;
        StyleStringMap mStyleStrings;        
        
        typedef std::map<std::string, gcn::Color> StyleColorMap;
        StyleColorMap mStyleColors;
        
        typedef std::map<std::string, gcn::Rectangle> StyleRectMap;
        StyleRectMap mStyleRects;
        
        ImageContainer mImageCont;
    };
    
} // namespace gcn


#endif
