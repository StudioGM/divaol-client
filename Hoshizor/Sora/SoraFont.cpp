//
//  SoraFont.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/18/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraFont.h"
#include "SoraCore.h"

namespace sora {
    
    SoraFont* SoraFont::LoadFromFile(const StringType& path, uint32 size) {
        return SoraCore::Instance()->createFont(path, size);
    }
    
    SoraFont* SoraFont::LoadFromMemory(void* data, uint32 size, uint32 fontSize, const StringType& name) {
        return SoraCore::Instance()->createFont(data, size, fontSize, name);
    }
    
    void SoraFont::Release(SoraFont* font) {
        SoraCore::Instance()->releaseFont(font);
    }
    
} // namespace sora