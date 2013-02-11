#include "SoraLuaStateManager.h"

namespace sora {

	SoraHandle SoraLuaObjectManager::registerLuaObject(SoraLuaObject* obj) {
		luaObjectMap[(SoraHandle)obj] = obj;
		return (SoraHandle)obj;
	}

	void SoraLuaObjectManager::unregisterLuaObject(SoraLuaObject* obj) {
		LUA_OBJECT_MAP::iterator p = luaObjectMap.find((SoraHandle)obj);
		if(p != luaObjectMap.end()) {
			luaObjectMap.erase(p);
		}
	}

	SoraLuaObject* SoraLuaObjectManager::getLuaObject(SoraHandle h) {
		LUA_OBJECT_MAP::iterator p = luaObjectMap.find(h);
		if(p != luaObjectMap.end())
			return p->second;
		return 0;
	}

	void SoraLuaObjectManager::sendMessage(const SoraString& mssg, const SoraString& params, SoraHandle receiver) {
		LUA_OBJECT_MAP::iterator itObj = luaObjectMap.begin();
		
		while(itObj != luaObjectMap.end()) {
			if(receiver != 0) {
				if(receiver == (SoraHandle)itObj->second) {
					doSendMessage(mssg, params, itObj->second);
				}
			} else
				doSendMessage(mssg, params, itObj->second);

			++itObj;
		}
	}

	void SoraLuaObjectManager::doSendMessage(const SoraString& mssg, const SoraString& params, SoraLuaObject* obj) {
		LuaPlus::LuaObject luaObj = obj->get(mssg);
		if(luaObj.IsFunction()) {
			LuaPlus::LuaFunction<void> mssgFunc = luaObj;
			mssgFunc(params);
		}
	}

} // namespace sora