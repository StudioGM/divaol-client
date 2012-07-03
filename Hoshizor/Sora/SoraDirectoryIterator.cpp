//
//  SoraDirectoryIterator.cpp
//  Sora
//
//  Created by Robert Bu on 7/11/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraDirectoryIterator.h"

#ifdef OS_WIN32
#include "SoraDirectoryIteratorImplWindows.h"
#else
#include "SoraDirectoryIteratorImplUnix.h"
#endif


namespace sora {
    
    SoraDirectoryIterator::SoraDirectoryIterator():
    mImpl(NULL) {
        mIsEnd = false;
    }
    
    SoraDirectoryIterator::SoraDirectoryIterator(const SoraDirectoryIterator& iterator):
    mPath(iterator.mPath),
    mImpl(iterator.mImpl) {
        mIsEnd = false;
        if(mImpl) {
            mImpl->duplicate();
        }
    }
    
    SoraDirectoryIterator::SoraDirectoryIterator(const std::string& path):
    mPath(path),
    mImpl(new SoraDirectoryIteratorImpl(path)) {
        mIsEnd = false;
        mPath.makeDirectory();
        mPath.setFileName(mImpl->get());
    }
    
    SoraDirectoryIterator::SoraDirectoryIterator(const std::wstring& path):
    mPath(path) {
        mImpl = new SoraDirectoryIteratorImpl(mPath.toString());
        mIsEnd = false;
        mPath.makeDirectory();
        mPath.setFileName(mImpl->get());
    }
    
    SoraDirectoryIterator::SoraDirectoryIterator(const char* path):
    mPath(path),
    mImpl(new SoraDirectoryIteratorImpl(path)) {
        mIsEnd = false;
        mPath.makeDirectory();
        mPath.setFileName(mImpl->get());
    }
    
    SoraDirectoryIterator::SoraDirectoryIterator(const wchar_t* path):
    mPath(path) {
        mImpl = new SoraDirectoryIteratorImpl(mPath.toString());
        mIsEnd = false;
        mPath.makeDirectory();
        mPath.setFileName(mImpl->get());
    }
    
    SoraDirectoryIterator::SoraDirectoryIterator(const SoraPath& path):
    mPath(path),
    mImpl(new SoraDirectoryIteratorImpl(path.toString())) {
        mIsEnd = false;
        mPath.makeDirectory();
        mPath.setFileName(mImpl->get());
    }
    
    SoraDirectoryIterator::~SoraDirectoryIterator() {
        if(mImpl)
            mImpl->release();
    }
    
    SoraDirectoryIterator& SoraDirectoryIterator::operator =(const SoraDirectoryIterator& rhs) {
        if(mImpl)
            mImpl->duplicate();
        mImpl = rhs.mImpl;
        if(mImpl) {
            mImpl->duplicate();
            mPath = rhs.mPath;
            mIsEnd = rhs.mIsEnd;
        }
        return *this;
    }
    
    
    SoraDirectoryIterator& SoraDirectoryIterator ::operator++() {
        if(mImpl) {
            std::string n = mImpl->next();
            if(n.empty())
                mIsEnd = true;
            else        
                mPath.setFileName(n);
        }
        return *this;
    }
    
    
} // namespace sora
