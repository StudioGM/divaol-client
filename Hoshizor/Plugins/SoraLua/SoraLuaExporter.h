//
//  SoraLuaExporter.h
//  Sora
//
//  Created by Robert Bu on 8/25/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraLuaExporter_h
#define Sora_SoraLuaExporter_h

#include "luaclass.h"
#include "LuaPlusHelper.h"
#include "SoraLuaConfig.h"
#include "signal/SoraSignal.h"

namespace sora {
    
    struct SoraLuaExporter {
        typedef SoraSignal<void(LuaPlus::LuaState*)> ExporterSignal;
        
		static ExporterSignal& getExporter() {
			static ExporterSignal mExporter;
			return mExporter;
		}

        template<typename T>
        static SoraConnection ConnectExporterFunc(const T& func) {
            return getExporter().connect(func);
        }
        
        static void OnExport(LuaPlus::LuaState* state);
        
    private:
        
    };
    
    /**
     * Helper macro that help check a symbol has been defined and register an export func
     **/
#define SORA_LUA_CHECK_AUTO_EXPORT_SYMBOL(Symbol) \
    SORA_LUA_AUTO_EXPORT_ALL || Symbol
    
#define SORA_LUA_AUTO_EXPORT_FUNC(fn) \
    SORA_STATIC_RUN_CODE_I(luaExport##fn, sora::SoraLuaExporter::ConnectExporterFunc(fn)) \
    
    
} // namespace sora


#endif
