#ifndef SORA_LUA_EXPORT
#define SORA_LUA_EXPORT

#include "luaclass.h"
#include "LuaPlusHelper.h"
#include "SoraCore.h"

#include <vector>
#include <cassert>

#include "spriteWrapper.h"
#include "fontWrapper.h"
#include "SoraEventManager.h"
#include "SoraMath.h"

#include "EnvVarExport.h"
#include "SoraLuaExporter.h"

//#include "TinkerExport.h"

// 重构

namespace sora { 
	
    void lua_export_soracore(LuaState* state);
        
#if	SORA_LUA_CHECK_AUTO_EXPORT_SYMBOL(SORA_LUA_AUTO_EXPORT_CORE)
    SORA_LUA_AUTO_EXPORT_FUNC(lua_export_soracore)
#endif

    void lua_export_sprites(LuaState* state);
    
#if	SORA_LUA_CHECK_AUTO_EXPORT_SYMBOL(SORA_LUA_AUTO_EXPORT_SPRITE)
    SORA_LUA_AUTO_EXPORT_FUNC(lua_export_sprites)
#endif
    
    void lua_export_font(LuaState* state);

#if	SORA_LUA_CHECK_AUTO_EXPORT_SYMBOL(SORA_LUA_AUTO_EXPORT_FONT)
    SORA_LUA_AUTO_EXPORT_FUNC(lua_export_font)
#endif
    
    void lua_export_constants(LuaState* state);
    
#if	SORA_LUA_CHECK_AUTO_EXPORT_SYMBOL(SORA_LUA_AUTO_EXPORT_SYMBOLS)
    SORA_LUA_AUTO_EXPORT_FUNC(lua_export_constants)
#endif
    
} // namespace sora

#endif