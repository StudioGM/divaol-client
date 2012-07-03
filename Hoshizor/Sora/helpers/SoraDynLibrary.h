/*
 *  SoraDLHelper.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_DYNAMIC_LIBRARY_HELPER
#define SORA_DYNAMIC_LIBRARY_HELPER

#include "SoraPlatform.h"
#include "SoraSingleton.h"

#include <map>

namespace sora {
    
    class SoraDynLibraryImpl;
	
    class SORA_API SoraDynLibrary {
    public:
        SoraDynLibrary();
        SoraDynLibrary(const char* path);
        ~SoraDynLibrary();
        
        bool open(const char* path);
        void close();
        
        void* getProc(const char* name);
        
    private:
        SoraDynLibraryImpl* mImpl;
    };
	
} // namespace sora

#endif // SORA_DYNAMIC_LIBRARY_HELPER