//
//  SoraCpuInfo.h
//  Sora
//
//  Created by Ruiwei Bu on 11/14/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraCpuInfo_h
#define Sora_SoraCpuInfo_h

#include "SoraPlatform.h"

namespace sora {
    
    class SoraCpuInfo {
    public:
        enum Feature {
            HTT             = 1UL << 0,
            MMX             = 1UL << 1,
            SSE             = 1UL << 2,
            SSE2            = 1UL << 3,
            SSE3            = 1UL << 4,
            SSSE3           = 1UL << 5,
            SSE41           = 1UL << 6,
            SSE42           = 1UL << 7,
            SSE4A           = 1UL << 8,
            MisalignedSSE   = 1UL << 9,
            X64             = 1UL << 10,
            FMA             = 1UL << 11,
            MOVBE           = 1UL << 12,
            POPCNT          = 1UL << 13,
            AES             = 1UL << 14,
            AVX             = 1UL << 15,
            LZCNT           = 1UL << 16
        };
        
    public:
        SoraCpuInfo();
        
        const std::string& getCPUString() const {
            return mCPUString;
        }
        
        const std::string& getCPUBrandString() const {
            return mCPUBrandString;
        }
        
        bool testFeature(Feature feature) const {
            return mFeatureMask & feature ? true : false;
        }
        
        int getNumHWThreads() const {
            return mNumHWThreads;
        }
        
        int getNumCores() const {
            return mNumCores;
        }
        
    private:
        std::string mCPUString;
        std::string mCPUBrandString;
        uint64 mFeatureMask;
        
        int mNumCores;
        int mNumHWThreads;
    };
    
} // namespace sora


#endif
