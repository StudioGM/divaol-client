//
//  SoraMemory.h
//  Sora
//
//  Created by Robert Bu on 7/31/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraMemory_h
#define Sora_SoraMemory_h

#include "stddef.h"

namespace sora {
    
    // memory allocator for sora core
    struct SoraMemoryAllocFunc {
        typedef void* (*MallocFunc)(size_t);
        typedef void  (*FreeFunc)(void*);
        typedef void* (*ReallocFunc)(void*, size_t);
        
        MallocFunc sora_malloc;
        FreeFunc sora_free;
        ReallocFunc sora_realloc;
        
        SoraMemoryAllocFunc(MallocFunc ma,
                            FreeFunc fr,
                            ReallocFunc re):
        sora_malloc(ma),
        sora_free(fr),
        sora_realloc(re) {}
    };
    
    static SoraMemoryAllocFunc& defaultAllocator() {
        static SoraMemoryAllocFunc def(malloc, free, realloc);
        return def;
    }
    
    static SoraMemoryAllocFunc g_memory_allocator(malloc, free, realloc);
    
    static void setGlobalMalloc(SoraMemoryAllocFunc::MallocFunc ma) {
        g_memory_allocator.sora_malloc = ma;
    }
    
    static void setGlobalFree(SoraMemoryAllocFunc::FreeFunc fr) {
        g_memory_allocator.sora_free = fr;
    }
    
    static void setGlobalRealloc(SoraMemoryAllocFunc::ReallocFunc re) {
        g_memory_allocator.sora_realloc = re;
    }
    
    inline void* _sora_realloc(void* ptr, size_t size) {
        return realloc(ptr, size);
    }
    
    inline void* sora_malloc(size_t size) {
        return g_memory_allocator.sora_malloc(size);
    }
    
    inline void sora_free(void* ptr) {
        return g_memory_allocator.sora_free(ptr);
    }
    
    inline void* sora_realloc(void* ptr, size_t size) {
        return g_memory_allocator.sora_realloc(ptr, size);
    }
    
#define sora_malloc_t(t, size) \
    (t*)sora_malloc(sizeof(t) * size);
    

}


#endif
