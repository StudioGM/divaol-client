//
//  modifier.cpp
//  Guichan_Sora
//
//  Created by Robert Bu(darkfall) 8/10/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "guichan/widget.hpp"
#include "guichan/modifier.hpp"

namespace gcn {
    
    Modifier::Modifier(bool autorelease):
    mFinished(false),
    mAutoRelease(autorelease),
    mOwner(0) {
    }
        
    void Modifier::release() {
        delete this;
    }		
	
    void Modifier::setFinish(bool flag) { 
        mFinished = flag;
    }
    
    bool Modifier::isFinished() const { 
        return mFinished;
    }
    
    void Modifier::setAutoRelease(bool flag) {
        mAutoRelease = flag;
    }
    
    bool Modifier::isAutoRelease() const {
        return mAutoRelease;
    }
    
    void Modifier::setOwner(Widget* widget) {
        mOwner = widget;
    }
    
    Widget* Modifier::getOwner() const {
        return mOwner;
    }
    
} // namespace gcn
