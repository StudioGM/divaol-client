//
//  SoraScriptVMHolder.h
//  Sora
//
//  Created by Robert Bu on 8/20/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraScriptVMHolder_h
#define Sora_SoraScriptVMHolder_h

#include "SoraScriptVM.h"
#include "signal/SoraSignal.h"

#include <map>

namespace sora {
    
    struct SoraScriptVMHolder {
        SoraScriptVMHolder() { }
        ~SoraScriptVMHolder();
        
        bool attachVM(ScriptVMPtr, const SoraString& tag);
        ScriptVMPtr detachVM(const SoraString& tag);
        ScriptVMPtr getVM(const SoraString& tag) const;
        
        typedef SoraSignal<void(const SoraString&, ScriptVMPtr)> VMAttachedSignal;
        typedef SoraSignal<void(const SoraString&, ScriptVMPtr)> VMDetachedSignal;
        
        template<typename T>
        SoraConnection subscribeToVMAttached(T);
        
        template<typename T>
        SoraConnection subscribeToVMDetached(T);
        
    private:
        VMAttachedSignal mSigAttached;
        VMDetachedSignal mSigDetached;
        
        typedef std::map<SoraString, ScriptVMPtr> VmMap;
        VmMap mVMs;
    };
    
    template<typename T>
    SoraConnection SoraScriptVMHolder::subscribeToVMAttached(T fn) {
        return mSigAttached.connect(fn);
    }
    
    template<typename T>
    SoraConnection SoraScriptVMHolder::subscribeToVMDetached(T fn) {
        return mSigDetached.connect(fn);
    }
    
} // namespace sora


#endif
