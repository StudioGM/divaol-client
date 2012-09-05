/*
 *  SoundManagerLuaExport.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/30/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SOUND_MANAGER_LUA_EXPORT_H_
#define SOUND_MANAGER_LUA_EXPORT_H_

#include "SoraLua/luaclass.h"
#include "SoraBGMManager.h"
#include "SoraSoundEffectManager.h"

#include "SoraLua/SoraLuaExporter.h"

namespace sora {

	static void lua_export_soundmanager(LuaPlus::LuaState* state) {
		gge::LuaClass<sora::SoraBGMManager>(state, "BGMManager", sora::SoraBGMManager::Instance())
			.def("play", &sora::SoraBGMManager::play)
			.def("stop", &sora::SoraBGMManager::stop)
			.def("pause", &sora::SoraBGMManager::pause)
			.def("resume", &sora::SoraBGMManager::resume)
			.def("freeAll", &sora::SoraBGMManager::freeAllBGMs)
			.def("toNext", &sora::SoraBGMManager::toNextBGM)
			.def("toPrev", &sora::SoraBGMManager::toPrevBGM)
			.def("setVolume", &sora::SoraBGMManager::setVolume)
			.def("setPitch", &sora::SoraBGMManager::setPitch)
			.def("setPan", &sora::SoraBGMManager::setPan)
			.def("setFadeTime", &sora::SoraBGMManager::setFadeTime)
			.def("enableRandomPlay", &sora::SoraBGMManager::enableRandomBGMQueuePlay)
            .def("playBGS", &sora::SoraBGMManager::playBGS)
            .def("stopBGS", &sora::SoraBGMManager::stopBGS)
            .def("adjustBGSVolume", &sora::SoraBGMManager::adjustBGSVolume)
            .def("toNextBGM", &sora::SoraBGMManager::toNextBGM)
            .def("toPrevBGM", &sora::SoraBGMManager::toPrevBGM);
			
		gge::LuaClass<sora::SoraSoundEffectManager>(state, "SEManager", sora::SoraSoundEffectManager::Instance())
			.def("load", &sora::SoraSoundEffectManager::load)
			.def("unload", &sora::SoraSoundEffectManager::unload)
			.def("play", &sora::SoraSoundEffectManager::play)
			.def("stop", &sora::SoraSoundEffectManager::stop)
			.def("playDirect", &sora::SoraSoundEffectManager::playDirect)
			.def("setVolume", &sora::SoraSoundEffectManager::setVolume);
	}
	
#if	SORA_LUA_CHECK_AUTO_EXPORT_SYMBOL(SORA_LUA_AUTO_EXPORT_BGM_MANAGER)
    SORA_LUA_AUTO_EXPORT_FUNC(lua_export_soundmanager)
#endif
    
}

#endif