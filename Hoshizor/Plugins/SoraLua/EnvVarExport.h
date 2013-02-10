/*
 *  GlobalValuesLuaExport.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/2/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef GLOBAL_VALUES_LUA_EXPORT_H_
#define GLOBAL_VALUES_LUA_EXPORT_H_

#include "SoraEnvValues.h"
#include "luaclass.h"
#include "SoraLuaExporter.h"

namespace sora {
	
	void lua_export_env_val(LuaPlus::LuaState* state);
	
#if	SORA_LUA_CHECK_AUTO_EXPORT_SYMBOL(SORA_LUA_AUTO_EXPORT_ENV_VAL)
    SORA_LUA_AUTO_EXPORT_FUNC(lua_export_env_val)
#endif
    
} // namespace sora

#endif // GLOBAL_VALUES_LUA_EXPORT_H_
