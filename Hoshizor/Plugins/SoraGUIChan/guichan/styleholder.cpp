//
//  styleholder.cpp
//  Sora
//
//  Created by Robert Bu on 8/26/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "guichan/styleholder.hpp"
#include "guichan/style.hpp"
#include "guichan/exception.hpp"

namespace gcn {
    
    StyleHolder::StyleHolder() {
        addStyle("default", Style::DefaultStyle());
    }
    
    StyleHolder::~StyleHolder() {
        StyleMap::iterator it = mStyles.begin();
        while(it != mStyles.end()) {
            delete it->second;
            ++it;
        }
        mStyles.clear();
    }
    
    StyleHolder& StyleHolder::Instance() {
        static StyleHolder instance;
        return instance;
    }
    
    Style* StyleHolder::addStyle(const std::string& tag) {
        StyleMap::const_iterator it = mStyles.find(tag);
        if(it == mStyles.end()) {
            Style* style = new Style();
            mStyles.insert(std::make_pair(tag, style));
        } else
            throw GCN_EXCEPTION("Style with name " + tag + " already exists");
        return NULL;
    }
    
    void StyleHolder::addStyle(const std::string& tag, Style* style) {
        StyleMap::const_iterator it = mStyles.find(tag);
        if(it == mStyles.end()) {
            mStyles.insert(std::make_pair(tag, style));
        } else
            throw GCN_EXCEPTION("Style with name " + tag + " already exists");
    }
    
    Style* StyleHolder::removeStyle(const std::string& tag) {
        StyleMap::iterator it = mStyles.find(tag);
        if(it != mStyles.end()) {
            Style* style = it->second;
            mStyles.erase(it);
            return style;
        }
        return NULL;
    }
    
    Style* StyleHolder::getStyle(const std::string& tag) const {
        StyleMap::const_iterator it = mStyles.find(tag);
        if(it != mStyles.end()) {
            return it->second;
        }
        return NULL;
    }
    
} // namespace gcn