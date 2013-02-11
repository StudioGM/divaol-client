#include "SoraLuaObject.h"

#include "LuaPlus/src/lua.h"

extern "C" {
#include "pluto.h"
}

#include "SoraLuaStateManager.h"
#include "SoraLuaExport.h"

#include "SoraLogger.h"
#include "SoraLuaExporter.h"

namespace sora {
	
	using namespace LuaPlus;
	
	SoraLuaObject::SoraLuaObject(bool heavy) {
		luaState = LuaState::Create(true);

        if(luaState && heavy) {
            SoraLuaExporter::OnExport(luaState);

            // pluto
            luaopen_pluto(luaState->GetCState());
           
            SoraLuaObjectManager::Instance()->registerLuaObject(this);
            
            // set self
            getGlobals().SetInteger("SELF", SoraObject::getUniqueId());
            
        } else 
            log_error("Error creating LuaState");
	}

	SoraLuaObject::SoraLuaObject(const SoraWString& scriptPath, bool heavy) {
		luaState = LuaState::Create(true);
		if(luaState && heavy) {
            SoraLuaExporter::OnExport(luaState);
		
            // pluto
            luaopen_pluto(luaState->GetCState());

            SoraLuaObjectManager::Instance()->registerLuaObject(this);
            
            // set self
            getGlobals().SetInteger("SELF", SoraObject::getUniqueId());

            doScript(scriptPath);
        } else 
            log_error("Error creating LuaState");
	}

	SoraLuaObject::SoraLuaObject(LuaState* state, bool heavy) {
		luaState = state;
        sora_assert(luaState);
        
        // set self
        getGlobals().SetInteger("SELF", SoraObject::getUniqueId());
        
        SoraLuaObjectManager::Instance()->registerLuaObject(this);
	}

	SoraLuaObject::~SoraLuaObject() {
        LuaState::Destroy(luaState);
		SoraLuaObjectManager::Instance()->unregisterLuaObject(this);
	}

	int32 SoraLuaObject::doScript(const SoraWString& scriptPath) {
		uint32 size;
		void* pdata = SoraCore::Ptr->getResourceFile(scriptPath, size);
		if(pdata) {
			return doString((const char*)pdata);
			SoraCore::Ptr->freeResourceFile(pdata);
		}
		return 0;
	}

	int32 SoraLuaObject::doString(const SoraString& str) {
		int32 result = luaState->DoString(str.c_str());
		if(result != 0) {
	
			lua_Debug debug;
			lua_getstack(luaState->GetCState(), 0, &debug);
	
			log_error(vamssg("Lua Error: %d, what:\n %s", result, debug.what!=NULL?debug.what:"???"));
		}
		return result;
	}

	int32 SoraLuaObject::doBuffer(const char* buff, size_t size, const char* name) {
		int32 result = luaState->DoBuffer(buff, size, name);
		if(result != 0) {
			
			lua_Debug debug;
			lua_getstack(luaState->GetCState(), 0, &debug);
			
			log_error(vamssg("Lua Error: %d, what:\n %s", result, debug.what!=NULL?debug.what:"???"));
		}
		return result;
	}

	int32 SoraLuaObject::loadScript(const SoraWString& scriptPath) {
		uint32 size;
		void* pdata = SoraCore::Ptr->getResourceFile(scriptPath, size);
		if(pdata) {
			return loadString((const char*)pdata);
			SoraCore::Ptr->freeResourceFile(pdata);
		}
		return 0;
	}

	int32 SoraLuaObject::loadString(const SoraString& str) {
		int32 result = luaState->LoadString(str.c_str());
		if(result != 0) {
			
			lua_Debug debug;
			lua_getstack(luaState->GetCState(), 0, &debug);
			
			log_error(vamssg("Lua Error: %d, what:\n %s", result, debug.what!=NULL?debug.what:"???"));
		}
		return result;
	}

	int32 SoraLuaObject::loadBuffer(const char* buffer, size_t size, const char* name) {
		int32 result = luaState->LoadBuffer(buffer, size, name);
		if(result != 0) {
			
			lua_Debug debug;
			lua_getstack(luaState->GetCState(), 0, &debug);
			
			log_error(vamssg("Lua Error: %d, what:\n %s", result, debug.what!=NULL?debug.what:"???"));
		}
		return result;
	}

	LuaObject SoraLuaObject::createThread() {
		return luaState->CreateThread(luaState);
	}

	LuaStackObject SoraLuaObject::createTable(int narr, int nrec) {
		return luaState->CreateTable(narr, nrec);
	}

	LuaStackObject SoraLuaObject::createUserData(size_t size) {
		return luaState->NewUserData(size);
	}

	LuaObject SoraLuaObject::getGlobals() {
		return luaState->GetGlobals();
	}

	LuaObject SoraLuaObject::get(const SoraString& name) {
		return luaState->GetGlobal(name.c_str());
	}

	LuaObject SoraLuaObject::operator[](const SoraString& name) {
		return luaState->GetGlobal(name.c_str());
	}

	int32 SoraLuaObject::getInteger(const SoraString& name) {
		return luaState->GetGlobal(name.c_str()).GetInteger();
	}

	float SoraLuaObject::getFloat(const SoraString& name) {
		return luaState->GetGlobal(name.c_str()).GetFloat();
	}

	double SoraLuaObject::getDouble(const SoraString& name) {
		return luaState->GetGlobal(name.c_str()).GetDouble();
	}

	SoraString SoraLuaObject::getString(const SoraString& name) {
		return luaState->GetGlobal(name.c_str()).GetString();
	}

	lua_CFunction SoraLuaObject::getCFunction(const SoraString& name) {
		return luaState->GetGlobal(name.c_str()).GetCFunction();
	}

	void* SoraLuaObject::getUserData(const SoraString& name) {
		return luaState->GetGlobal(name.c_str()).GetUserData();
	}

	bool SoraLuaObject::getBoolean(const SoraString& name) {
		return luaState->GetGlobal(name.c_str()).GetBoolean();
	}

	LuaState* SoraLuaObject::getState() const {
		return luaState;
	}
	
    void SoraLuaObject::callFuncVoid(const SoraString& funcName) {
        LuaObject obj = get(funcName);
        if(obj.IsFunction()) {
            LuaFunctionVoid func = obj;
            func();
        }
    }
	
    int32 SoraLuaObject::update(float dt) {
        LuaObject obj = get("update");
        if(obj.IsFunction()) {
            LuaFunction<void> func = obj;
            func(dt);
        }
        return 0;
    }
   
    void SoraLuaObject::render() {
        callFuncVoid("render");
    }
} // namespace sora