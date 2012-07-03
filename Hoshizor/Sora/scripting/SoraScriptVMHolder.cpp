//
//  SoraScriptVMHolder.cpp
//  Sora
//
//  Created by Robert Bu on 8/20/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraScriptVMHolder.h"

namespace sora {
    
    SoraScriptVMHolder::~SoraScriptVMHolder() {
        
    }
    
    bool SoraScriptVMHolder::attachVM(ScriptVMPtr ptr, const SoraString& tag) {
        if(tag.empty() || !ptr)
            return NULL;
        
        mVMs.insert(std::make_pair(tag, ptr));
        mSigAttached(tag, ptr);
        return true;
    }
    
    ScriptVMPtr SoraScriptVMHolder::detachVM(const SoraString& tag) {
        VmMap::iterator it = mVMs.find(tag);
        if(it != mVMs.end()) {
            ScriptVMPtr ptr = it->second;
            mVMs.erase(it);
            mSigDetached(tag, ptr);
            return ptr;
        }
        return ScriptVMPtr();
    }
    
    ScriptVMPtr SoraScriptVMHolder::getVM(const SoraString& tag) const {
        VmMap::const_iterator it = mVMs.find(tag);
        if(it != mVMs.end()) {
            return it->second;
        }
        return ScriptVMPtr();
    }

    
} // namespace sora