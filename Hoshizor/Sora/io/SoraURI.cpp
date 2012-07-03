//
//  SoraURI.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/6/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraURI.h"
#include "SoraPath.h"

namespace sora {
    
    bool SoraURI::split(const StringType& s) {
        sora_assert(s.isValid());
        clear();
        mEmpty = true;
        
        std::string str = s;
        // to resolve assign
      
    }
    
    StringType SoraURI::build() const {
        sora_assert(!mEmpty);
        
        StringType str;
        if(mScheme.isValid()) {
            str += mScheme;
            str += "://";
        }
        if(mUserInfo.isValid()) {
            str += mUserInfo;
            str += "@";
        }
        if(mHost.isValid()) {
            str += mHost;
        }
        if(mPort.isValid()) {
            str += ":";
            str += mPort;
        }
        if(mLocalPath.isValid()) {
            str += "/";
            str += mLocalPath;
        }
        if(mFragment.isValid()) {
            str += "#";
            str += mFragment;
        }
        if(mQuery.isValid()) {
            str += "?";
            str += mQuery;
        }
        return str;
    }
    
    StringType SoraURI::getTail() const {
        StringType str;
        if(mLocalPath.isValid()) {
            str += mLocalPath;
        }
        if(mFragment.isValid()) {
            str += "#";
            str += mFragment;
        }
        if(mQuery.isValid()) {
            str += "?";
            str += mQuery;
        }
        return str;
    }
    
    void SoraURI::appendLocalPath(const StringType& path) {
        mLocalPath += "/";
        mLocalPath += path;
    }
} // namespace sora