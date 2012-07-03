//
//  SoraScriptVM.h
//  Sora
//
//  Created by Robert Bu on 8/20/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraScriptVM_h
#define Sora_SoraScriptVM_h

#include "SoraPlatform.h"
#include "SoraAutoPtr.h"
#include "SoraUncopyable.h"

namespace sora {
    
    class SoraScriptingSystem;
    class SoraScript;
    class SoraScriptVM;
    
    typedef SoraAutoPtr<SoraScript> ScriptPtr;
    typedef SoraAutoPtr<SoraScriptVM> ScriptVMPtr;
    
    /**
     * Interfaces for work with script
     **/
    
    class SORA_API SoraScript {
    public:
        virtual bool isLoaded() = 0;
        virtual void setData(const SoraString& data) = 0;
    };
    
    class SORA_API SoraScriptVM: SoraUncopyable {
    public:
        virtual ~SoraScriptVM() {}
        
        virtual SoraScriptingSystem* getCreator() const = 0;
        virtual void execute(ScriptPtr script) = 0;
        virtual void callVoidFunc(const SoraString& funcName) = 0;
    };
    
    class SORA_API SoraScriptingSystem {
    public:
        enum Language {
            Lua = 0,
            LScript,
        };
        
        virtual int32 getLanguage() const = 0;
        virtual SoraString getName() const = 0;
        
        virtual ScriptVMPtr createVM() = 0;
        virtual ScriptPtr createScriptFromFile(const SoraString& file) = 0;
    };
    
} // namespace sora

#endif
