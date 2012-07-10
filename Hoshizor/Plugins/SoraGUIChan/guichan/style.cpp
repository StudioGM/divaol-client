//
//  style.cpp
//  Sora
//
//  Created by Robert Bu on 8/10/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "guichan/style.hpp"
#include "guichan/image.hpp"

namespace gcn {
    
    Style* Style::DefaultStyle() {
        static Style global_style;
        return &global_style;
    }
    
    Image* ImageContainer::get(const std::string& key) const {
        ImageMap::const_iterator it = mImages.find(key);
        if(it != mImages.end()) {
            return it->second;
        }
        return 0;
    }
    
    Image* ImageContainer::load(const std::string& key, const std::string& imagepath) {
        ImageMap::const_iterator it = mImages.find(key);
        if(it != mImages.end()) {
            return it->second;
        }
        
        Image* image = Image::load(imagepath);
        if(image) {
            mImages.insert(std::make_pair(key, image));
        }
        return image;
    }
    
    bool ImageContainer::has(const std::string& key) const {
        ImageMap::const_iterator it = mImages.find(key);
        return it != mImages.end();
    }
    
    void ImageContainer::release(const std::string& key) {
        ImageMap::iterator it = mImages.find(key);
        if(it != mImages.end()) {
            delete it->second;
            mImages.erase(it);
        }
    }
    
    void ImageContainer::releaseAll() {
        ImageMap::const_iterator it = mImages.begin();
        for(; it != mImages.end(); ++it) {
            delete it->second;
        }
        mImages.clear();
    }
    
    Style::Style():
    mGlobalBackgroundColor(0xffffff),
    mGlobalForegroundColor(0x000000),
    mGlobalBaseColor(0x808090),
    mGlobalSelectionColor(0xc3d9ff) {
        
    }
    
	void Style::setBackgroundColor(const Color& col) {
		mGlobalBackgroundColor = col;
	}
	
	void Style::setForegroundColor(const Color& col) {
		mGlobalForegroundColor = col;
	}
	
	void Style::setBaseColor(const Color& col) {
		mGlobalBaseColor = col;
	}
	
	void Style::setSelectionColor(const Color& col) {
		mGlobalSelectionColor = col;
	}
	
	Color Style::getBackgroundColor() {
		return mGlobalBackgroundColor;
	}
	
	Color Style::getForegroundColor() {
		return mGlobalForegroundColor;
	}
	
	Color Style::getBaseColor() {
		return mGlobalBaseColor;
	}
	
	Color Style::getSelectionColor() {
		return mGlobalSelectionColor;
	}
    
    std::string Style::getString(const std::string& key) const {
        StyleStringMap::const_iterator it = mStyleStrings.find(key);
        if(it != mStyleStrings.end())
            return it->second;
        return std::string();
    }
    
    void Style::setString(const std::string& key, const std::string& val) {
        StyleStringMap::iterator it = mStyleStrings.find(key);
        if(it != mStyleStrings.end())
            it->second = val;
        else
            mStyleStrings.insert(std::make_pair(key, val));
    }
    
    Color Style::getColor(const std::string& col) const {
        StyleColorMap::const_iterator it = mStyleColors.find(col);
        if(it != mStyleColors.end())
            return it->second;
        return gcn::Color();
    }
    
    void Style::setColor(const std::string& key, const gcn::Color& col) {
        mStyleColors[key] = col;
    }
    
    Rectangle Style::getRect(const std::string& rect) const {
        StyleRectMap::const_iterator it = mStyleRects.find(rect);
        if(it != mStyleRects.end())
            return it->second;
        return gcn::Rectangle();
    }
    
    void Style::setRect(const std::string& key, const gcn::Rectangle& rect) {
        mStyleRects.insert(std::make_pair(key, rect));
    }
    
    ImageContainer& Style::image() {
        return mImageCont;
    }
    
    bool Style::hasRect(const std::string& key) const {
        StyleRectMap::const_iterator it = mStyleRects.find(key);
        return it != mStyleRects.end();
    }
    
    bool Style::hasString(const std::string& key) const {
        StyleStringMap::const_iterator it = mStyleStrings.find(key);
        return it != mStyleStrings.end();
    }
    
    bool Style::hasColor(const std::string& key) const {
        StyleColorMap::const_iterator it = mStyleColors.find(key);
        return it != mStyleColors.end();
    }
    
} // namespace gcn
