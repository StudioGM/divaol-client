//
//  SoraDirectoryIteratorImplUnix.h
//  Sora
//
//  Created by Robert Bu on 7/11/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraDirectoryIteratorImplUnix_h
#define Sora_SoraDirectoryIteratorImplUnix_h

#include "SoraPath.h"
#include "SoraException.h"
#include <dirent.h>

namespace sora {
    
    class SORA_API SoraDirectoryIteratorImpl {
    public:
        SoraDirectoryIteratorImpl(const std::string& path) {
            SoraPath p(path);
            p.makeDirectory();
            
            mDir = opendir(p.toString().c_str());
            
            if(!mDir)
                THROW_SORA_EXCEPTION(PathNotFoundException, "Error opening dir");
            else
                next();
        }
        ~SoraDirectoryIteratorImpl() {
            if(mDir)
                closedir(mDir);
        }
        
        void duplicate();
        void release();
        
        const std::string& get() const;
        const std::string& next() {
            do {
                struct dirent* entry = readdir(mDir);
                if(entry)
                    mCurrent = entry->d_name;
                else
                    mCurrent.clear();
            }
            while(mCurrent == "." || mCurrent == ".." || (mCurrent.size() > 0 && mCurrent[0] == '.'));
            
            return mCurrent;
        }
        
    private:
        DIR* mDir;
        std::string mCurrent;
        int mRC;
    };
    
    const std::string& SoraDirectoryIteratorImpl::get() const {
        return mCurrent;
    }
    
    inline void SoraDirectoryIteratorImpl::duplicate() {
        ++mRC;
    }
    
    inline void SoraDirectoryIteratorImpl::release() {
        if (--mRC) {
            delete this;
        }
    }
} // namespace sora

#endif
