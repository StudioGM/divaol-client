//
//  SoraDirectoryIteratorImplWindows.h
//  Sora
//
//  Created by Robert Bu on 7/11/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraDirectoryIteratorImplWindows_h
#define Sora_SoraDirectoryIteratorImplWindows_h

#include "SoraPlatform.h"

#ifdef OS_WIN32

#include <windows.h>

namespace sora {
    
    class SORA_API SoraDirectoryIteratorImpl {
    public:
        SoraDirectoryIteratorImpl(const std::string& path) {
            SoraPath p(path);
            p.makeDirectory();
            std::string findPath = p.toString()+"*";
            
            mFH = FindFirstFileA(findPath.c_str(), &mFD);
            if(mFH == INVALID_HANDLE_VALUE) {
				DWORD error = GetLastError();
                if(error != ERROR_NO_MORE_FILES) 
                    THROW_SORA_EXCEPTION(RuntimeException, "Error when iterating files");
            } else {
                mCurrent = mFD.cFileName;
                if(mCurrent == "." || mCurrent == "..")
                    next();
            }
        }
        
        ~SoraDirectoryIteratorImpl() {
            if(mFH != INVALID_HANDLE_VALUE)
                FindClose(mFH);
        }
        
        void duplicate();
        void release();
        
        const std::string& get() const;
        const std::string& next() {
            do {
                if(FindNextFileA(mFH, &mFD) != 0) {
                    mCurrent = mFD.cFileName;
                } else 
                    mCurrent.clear();
            }
            while(mCurrent == "." || mCurrent == "..");
           
            return mCurrent;
        }
        
    private:
        HANDLE mFH;
        WIN32_FIND_DATAA mFD;
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


#endif // OS_WIN32


#endif
