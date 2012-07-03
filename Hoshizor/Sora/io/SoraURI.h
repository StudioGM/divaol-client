//
//  SoraURI.h
//  Sora
//
//  Created by Ruiwei Bu on 9/6/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraURI_h
#define Sora_SoraURI_h

#include "SoraPlatform.h"
#include "SoraString.h"

namespace sora {
    
    /**
     An URI object can split a Uniform Resource Identifier string into
     its components or build a string from URI components. Please note
     that the memory footprint of an URI object is always bigger then
     a pure String object, so if memory usage is of concern, it is advised
     to keep paths as String objects around, and only use URI objects
     to encode and decode them.
     
     An URI is made of the following components, where most of them
     are optional:
     
     Scheme://UserInfo@Host:Port/LocalPath#Fragment?Query
     
     Example URIs:
     
     http://user:password@www.myserver.com:8080/index.html#main
     http://www.myserver.com/query?user=bla
     ftp://ftp.myserver.com/pub/bla.zip
     file:///c:/temp/bla.txt
     file://SambaServer/temp/blub.txt
     
     Note that assigns will be resolved before splitting a URI into its
     components, for instance the assign "textures" could be defined
     as:
     
     Assign("textures", "http://www.dataserv.com/myapp/textures/");
     
     So a path to a texture URI could be defined as:
     
     URI("textures:mytex.dds")
     
     Which would actually resolve into:
     
     http://www.dataserv.com/myapp/textures/mytex.dds
     
     Decoding into components happens in the init constructor or the 
     Set() method in the following steps:
     
     - resolve any assigns in the original string
     - split into Scheme, Host and Path blocks
     - resolve Host and Path blocks further
     
     Enconding from components into string happens in the AsString()
     method in the following steps:
     
     - concatenate URI string from components
     - convert part of the string back into an existing assign
     **/
    
    class SoraURI {
    public:
        SoraURI();
        SoraURI(const StringType& s);
        SoraURI(const char* s);
        SoraURI(const SoraURI& s);
        
        void operator=(const SoraURI& rhs);
        bool operator==(const SoraURI& rhs) const;
        bool operator!=(const SoraURI& rhs) const;
        
        void set(const StringType& s);
        StringType asString() const;
        
        bool isEmpty() const;
        bool isValid() const;
        
        void clear();
        void setScheme(const StringType& s);
        const StringType& scheme() const;
        void setUserInfo(const StringType& s);
        const StringType& userInfo() const;
        void setHost(const StringType& s);
        const StringType& host() const;
        void setPort(const StringType& s);
        const StringType& port() const;
        void setLocalPath(const StringType& s);
        const StringType& localPath() const;
        void setFragment(const StringType& frag);
        const StringType& fragment() const;
        void setQuery(const StringType& query);
        const StringType& query() const;
        
        void appendLocalPath(const StringType& path);
        StringType getTail() const;
        
    private:
        bool split(const StringType& s);
        StringType build() const;
        
        bool mEmpty;
        StringType mScheme;
        StringType mUserInfo;
        StringType mHost;
        StringType mPort;
        StringType mLocalPath;
        StringType mFragment;
        StringType mQuery;
    };
    
    inline SoraURI::SoraURI():
    mEmpty(true) {
        
    }
    
    inline SoraURI::SoraURI(const StringType& s):
    mEmpty(true) {
        bool validUri = this->split(s);
        sora_assert(validUri);
    }
    
    inline SoraURI::SoraURI(const char* s):
    mEmpty(true) {
        sora_assert(s);
        bool validUri = this->split(s);
        sora_assert(validUri);
    }
    
    inline SoraURI::SoraURI(const SoraURI& rhs):
    mEmpty(rhs.mEmpty),
    mScheme(rhs.mScheme),
    mUserInfo(rhs.mUserInfo),
    mHost(rhs.mHost),
    mPort(rhs.mPort),
    mLocalPath(rhs.mLocalPath),
    mFragment(rhs.mFragment),
    mQuery(rhs.mQuery) {
        
    }
    
    inline void SoraURI::operator=(const SoraURI& rhs) {
        if(this != &rhs) {  
            mEmpty = rhs.mEmpty;
            mScheme = rhs.mScheme;
            mUserInfo = rhs.mUserInfo;
            mHost = rhs.mHost;
            mPort = rhs.mPort;
            mLocalPath = rhs.mLocalPath;
            mFragment = rhs.mFragment;
            mQuery = rhs.mQuery;
        }
    }
    
    inline bool SoraURI::operator==(const SoraURI& rhs) const {
        if(this->mEmpty && rhs.mEmpty)
            return true;
        else {
            return ((mScheme == rhs.mScheme) &&
                    (mUserInfo == rhs.mUserInfo) &&
                    (mHost == rhs.mHost) &&
                    (mPort == rhs.mPort) &&
                    (mLocalPath == rhs.mLocalPath) &&
                    (mFragment == rhs.mFragment) &&
                    (mQuery == rhs.mQuery));
        }
    }
    
    inline bool SoraURI::operator!=(const SoraURI& rhs) const {
        return !(*this == rhs);
    }
    
    inline bool SoraURI::isEmpty() const {
        return mEmpty;
    }
    
    inline bool SoraURI::isValid() const {
        return !(mEmpty);
    }
    
    inline void SoraURI::clear() {
        mEmpty = true;
        mScheme.clear();
        mUserInfo.clear();
        mHost.clear();
        mPort.clear();
        mLocalPath.clear();
        mFragment.clear();
        mQuery.clear();
    }
    
    inline void SoraURI::set(const StringType& s) {
        this->split(s);
    }
    
    inline StringType SoraURI::asString() const {
        return this->build();
    }
    
    inline void SoraURI::setScheme(const StringType& s) {
        mEmpty = false;
        mScheme = s;
    }
    
    inline const StringType& SoraURI::scheme() const {
        return mScheme;
    }
    
    inline void SoraURI::setUserInfo(const StringType& s) {
        mEmpty = false;
        mUserInfo = s;
    }
    
    inline const StringType& SoraURI::userInfo() const {
        return mUserInfo;
    }
    
    inline void SoraURI::setHost(const StringType& s) {
        mEmpty = false;
        mHost = s;
    }
    
    inline const StringType& SoraURI::host() const {
        return mHost;
    }
    
    inline void SoraURI::setPort(const StringType& s) {
        mEmpty =false;
        mPort = s;
    }
    
    inline const StringType& SoraURI::port() const {
        return mPort;
    }
    
    inline void SoraURI::setLocalPath(const StringType& s) {
        mEmpty = false;
        mLocalPath = s;
    }
    
    inline const StringType& SoraURI::localPath() const {
        return mLocalPath;
    }
    
    inline void SoraURI::setFragment(const StringType& s) {
        mEmpty = false;
        mFragment = s;
    }
    
    inline const StringType& SoraURI::fragment() const {
        return mFragment;
    }
    
    inline void SoraURI::setQuery(const StringType& s) {
        mEmpty = false;
        mQuery = s;
    }
    
    inline const StringType& SoraURI::query() const {
        return mQuery;
    }
    
} // namespace sora


#endif
