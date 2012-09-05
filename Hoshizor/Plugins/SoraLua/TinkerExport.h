/*
 *  TinkerExport.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/10/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_TINKER_EXPORT_H_
#define SORA_TINKER_EXPORT_H_

#include "SoraSprite.h"

#include "LuaPlus/src/lua_tinker.h"

namespace sora {
	
	static void tinker_export_init(lua_State* L) {
		lua_tinker::init(L);
	}
	
	static void tinker_export_sprite(lua_State* L) {
		lua_tinker::class_add<SoraSprite>(L, "SoraSprite");
		lua_tinker::class_con<SoraSprite>(L, &lua_tinker::constructor<SoraTargetHandle>);
	}
	
} // namespace sora


#endif // SORA_TINKER_EXPORT_H_