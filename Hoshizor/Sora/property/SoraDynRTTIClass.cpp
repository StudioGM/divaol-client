//
//  SoraDynRTTIClass.cpp
//  Sora
//
//  Created by Robert Bu on 8/21/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraDynRTTIClass.h"

namespace sora {
    
    SoraDynRTTIClassManager::ClassPtrMap SoraDynRTTIClassManager::mClasses;
    
    void SoraDynRTTIClassManager::insert(SoraDynRTTIClass* cls) {
        sora_assert(cls);
        
        ClassPtrMap::const_iterator it = mClasses.find(cls->getName());
        if(it != mClasses.end())
            THROW_SORA_EXCEPTION(RttiException, "Class already registered");
        
        mClasses.insert(std::make_pair(cls->getName(), cls));
    }
    
    SoraDynRTTIClass* SoraDynRTTIClassManager::get(const std::string& name) {
        ClassPtrMap::const_iterator it = mClasses.find(name);
        if(it != mClasses.end()) 
            return it->second;
        return NULL;
    }
    
    void SoraDynRTTIClassManager::erase(SoraDynRTTIClass* cls) {
        sora_assert(cls);
        ClassPtrMap::const_iterator it = mClasses.find(cls->getName());
        if(it != mClasses.end())
            mClasses.erase(cls->getName());
    }
    
} // namespace sora