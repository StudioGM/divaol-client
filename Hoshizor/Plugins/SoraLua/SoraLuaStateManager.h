#ifndef SORA_LUA_STATE_MANAGER_H
#define SORA_LUA_STATE_MANAGER_H

#include "SoraPlatform.h"
#include "SoraException.h"
#include "SoraSingleton.h"

#include "SoraLuaObject.h"

#include <map>

namespace sora {

	using std::map;
	using LuaPlus::LuaState;

	class SoraLuaObjectManager: public SoraSingleton<SoraLuaObjectManager> {
		friend class SoraSingleton<SoraLuaObjectManager>;

	public:
		SoraHandle registerLuaObject(SoraLuaObject* obj);
		void unregisterLuaObject(SoraLuaObject* obj);
		SoraLuaObject* getLuaObject(SoraHandle h);

		void sendMessage(const SoraString& message, const SoraString& params, SoraHandle receiver=0);

	private:
		void doSendMessage(const SoraString& message, const SoraString& params, SoraLuaObject* obj);

		typedef map<SoraHandle, SoraLuaObject*> LUA_OBJECT_MAP;

		LUA_OBJECT_MAP luaObjectMap;
	};


} // namespace sora

#endif