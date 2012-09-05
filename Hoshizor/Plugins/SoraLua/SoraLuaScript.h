//
//  SoraLuaScript.h
//  Sora
//
//  Created by Robert Bu on 8/25/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraLuaScript_h
#define Sora_SoraLuaScript_h

#include "Scripting/SoraScriptVM.h"
#include "SoraLuaObject.h"
#include "SoraResourceFile.h"

namespace sora  {
    
    class SORA_API SoraLuaScript: public SoraScript {
    public:
        SoraLuaScript():
        mLoaded(false) {
            
        }
        SoraLuaScript(const SoraStringData& data):
        mData(data),
        mLoaded(true) {
            
        }
        
        virtual bool isLoaded() {
            return mLoaded;
        }
        
        virtual void setData(const SoraString& data) {
            mData = data;
            mLoaded = true;
        }
        
    private:
        friend class SoraLuaScriptVM;
        
        bool mLoaded;
        SoraString mData;
    };
    
    class SORA_API SoraLuaScriptVM: SoraScriptVM {
    public:
        SoraLuaScriptVM(SoraScriptingSystem owner):
        mCreator(creator),
        mLuaVM() {
            
        }
        
        virtual SoraScriptingSystem* getCreator() const {
            return mCreator;
        }
        
        virtual void execute(ScriptPtr script) {
            mLuaVM.doString(script->mData.c_str());
        }
        
        virtual void callVoidFunc(const SoraString& funcName) {
            mLuaVM.callFuncVoid(funcName);
        }
        
    private:
        SoraScriptingSystem* mCreator;

        SoraLuaObject mLuaVM;
    };
    
    class SORA_API SoraLuaScriptingSystem: SoraScriptingSystem {
    public:
        int32 getLanguage() const {
            return L_LUA;
        }
        SoraString getName() const {
            return "lua";
        }
        
        virtual ScriptVMPtr createVM() {
            return ScriptVMPtr(new SoraLuaScriptVM());
        }
        
        virtual ScriptPtr createScriptFromFile(const SoraString& file) {
            SoraResourceFile data(file);
            
            SoraString strData((const char*)data, data.size());
            return ScriptPtr(new SoraLuaScript(strData));
        }
    };
    
} // namespace sora



#endif
