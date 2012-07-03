/*
 *  SoraDllHelper.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraDynLibrary.h"

#ifndef OS_WIN32
#include <dlfcn.h>
#else
#endif 

#include "SoraLogger.h"

#include "SoraFileUtility.h"

namespace sora {
    
#ifdef OS_WIN32
    
    class SoraDynLibraryImpl {
    public:
        bool open(const char* path) {
            module = LoadLibraryA(path);
            if(module == NULL) {
                log_error(vamssg("SoraDynLibraryImpl: Error locating dll %s", path));
                return false;
            }
            return true;
        }
        void close() {
            if(module) {
                FreeLibrary(module);
            }
            module = NULL;
        }
        void* getProc(const char* proc) {
            if(module) {
                return GetProcAddress(module, proc);
            }
            return NULL;
        }
        
    private:
        HMODULE module;
    };
    
#elif defined(OS_OSX) || defined(OS_LINUX) || defined(OS_IOS) 
    
    class SoraDynLibraryImpl {
    public:
        bool open(const char* path) {
            dyn = dlopen(path, RTLD_LOCAL);
            if(!dyn) {
                log_error(std::string("SoraDynLibrary: ")+dlerror());
                return false;
            }
            return true;
        }
        void close() {
            if(dyn)
                dlclose(dyn);
            dyn = NULL;
        }
        void* getProc(const char* proc) {
            if(dyn) {
                return dlsym(dyn, proc);
            }
            return NULL;
        }
        
    private:
        void* dyn;
    };
    
#else
    
    // null impl
    
    class SoraDynLibraryImpl {
    public:
        bool open(const char* path) { return false; }
        void close() {}
        void* getProc(const char* proc) { return NULL; }
    };
    
#endif
    
    SoraDynLibrary::SoraDynLibrary(): mImpl(new SoraDynLibraryImpl) {
        
    }
    
    SoraDynLibrary::SoraDynLibrary(const char* path): mImpl(new SoraDynLibraryImpl) {
        mImpl->open(path);
    }
    
    SoraDynLibrary::~SoraDynLibrary() {
        if(mImpl) {
            mImpl->close();
            delete mImpl;
        }
    }
    
    bool SoraDynLibrary::open(const char* path) {
        return mImpl->open(path);
    }
    
    void SoraDynLibrary::close() {
        mImpl->close();
    }
    
    void* SoraDynLibrary::getProc(const char* name) {
        return mImpl->getProc(name);
    }


} // namespace sora