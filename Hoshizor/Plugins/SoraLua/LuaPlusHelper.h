#ifndef LUA_PLUS_HELPER_H
#define LUA_PLUS_HELPER_H

#include "LuaPlus/LuaPlus.h"
#include <string>
#include <typeinfo>

using namespace LuaPlus;

class LuaConvert
{
public:
	LuaConvert(const LuaObject& obj)
	{
        refobj = obj;
	}
	operator int()
	{
		return refobj.GetInteger();
	}
    operator unsigned int() {
        return refobj.GetInteger();
    }
    operator unsigned long() {
        return refobj.GetInteger();
    }
	operator float()
	{
		return refobj.GetFloat();
	}
	operator double()
	{
		return refobj.GetDouble();
	}
	operator const char* ()
	{
		return refobj.GetString();
	}
	operator const wchar_t* ()
	{
		// the line was changed by Darkfall
		return reinterpret_cast<const wchar_t*>(refobj.GetWString());
	}
	operator std::string()
	{
		return std::string(refobj.GetString());
	}
	operator std::wstring()
	{
		// the line was changed by Darkfall
		return std::wstring(reinterpret_cast<const wchar_t*>(refobj.GetWString()));
	}
	operator void* ()
	{
		return refobj.GetUserData();
	}
	template<typename T>
		operator T* ()
	{
		return (T*)refobj.GetUserData();
	}
	template<typename R>
		operator LuaFunction<R> ()
	{
		return LuaFunction<R>(refobj);
	}
private:
	LuaObject refobj;
};

template<typename Object>
static	int ConstructorHelperc(LuaState* state, Object* pObj)
{
    std::string metaname("MetaClass_");
    metaname += typeid(Object).name();
    
    LuaObject obj = state->BoxPointer(pObj);
    obj.SetMetaTable(state->GetGlobal(metaname.c_str()));
    obj.Push();
    return 1;
}

template<typename Object, typename A1>
static	int Constructor1c(LuaState* state)
{
    LuaConvert a1 = LuaObject(state, 1);
    return ConstructorHelperc(state, new Object((A1)a1) );
}

template<typename Object, typename A1, typename A2>
static	int Constructor2c(LuaState* state)
{
    LuaConvert a1 = LuaObject(state, 1);
    LuaConvert a2 = LuaObject(state, 2);
    return ConstructorHelperc(state, new Object((A1)a1, (A2)a2) );
}

template<typename Object, typename A1, typename A2, typename A3>
static	int Constructor3c(LuaState* state)
{
    LuaConvert a1 = LuaObject(state, 1);
    LuaConvert a2 = LuaObject(state, 2);
    LuaConvert a3 = LuaObject(state, 3);
    return ConstructorHelperc(state, new Object((A1)a1, (A2)a2, (A3)a3) );
}

template<typename Object, typename A1, typename A2, typename A3, typename A4>
static	int Constructor4c(LuaState* state)
{
    LuaConvert a1 = LuaObject(state, 1);
    LuaConvert a2 = LuaObject(state, 2);
    LuaConvert a3 = LuaObject(state, 3);
    LuaConvert a4 = LuaObject(state, 4);
    return ConstructorHelperc(state, new Object((A1)a1, (A2)a2, (A3)a3, (A4)a4) );
}

template<typename Object, typename A1, typename A2, typename A3, typename A4, typename A5>
static	int Constructor5c(LuaState* state)
{
    LuaConvert a1 = LuaObject(state, 1);
    LuaConvert a2 = LuaObject(state, 2);
    LuaConvert a3 = LuaObject(state, 3);
    LuaConvert a4 = LuaObject(state, 4);
    LuaConvert a5 = LuaObject(state, 5);
    return ConstructorHelperc(state, new Object((A1)a1, (A2)a2, (A3)a3, (A4)a4, (A5)a5) );
}

template<typename Object, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
static	int Constructor6c(LuaState* state)
{
    LuaConvert a1 = LuaObject(state, 1);
    LuaConvert a2 = LuaObject(state, 2);
    LuaConvert a3 = LuaObject(state, 3);
    LuaConvert a4 = LuaObject(state, 4);
    LuaConvert a5 = LuaObject(state, 5);
    LuaConvert a6 = LuaObject(state, 6);
    return ConstructorHelperc(state, new Object((A1)a1, (A2)a2, (A3)a3, (A4)a4, (A5)a5), (A6)a6 );
}

template<typename Object, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
static	int Constructor7c(LuaState* state)
{
    LuaConvert a1 = LuaObject(state, 1);
    LuaConvert a2 = LuaObject(state, 2);
    LuaConvert a3 = LuaObject(state, 3);
    LuaConvert a4 = LuaObject(state, 4);
    LuaConvert a5 = LuaObject(state, 5);
    LuaConvert a6 = LuaObject(state, 6);
    LuaConvert a7 = LuaObject(state, 7);
    return ConstructorHelperc(state, new Object((A1)a1, (A2)a2, (A3)a3, (A4)a4, (A5)a5), (A6)a6, (A7)a7 );
}

template<typename Object, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
static	int Constructor8c(LuaState* state)
{
    LuaConvert a1 = LuaObject(state, 1);
    LuaConvert a2 = LuaObject(state, 2);
    LuaConvert a3 = LuaObject(state, 3);
    LuaConvert a4 = LuaObject(state, 4);
    LuaConvert a5 = LuaObject(state, 5);
    LuaConvert a6 = LuaObject(state, 6);
    LuaConvert a7 = LuaObject(state, 7);
    LuaConvert a8 = LuaObject(state, 8);
    return ConstructorHelperc(state, new Object((A1)a1, (A2)a2, (A3)a3, (A4)a4, (A5)a5), (A6)a6, (A7)a7, (A8)a8 );
}


template<typename Object>
class LuaConstructor
{
private:

	static	int ConstructorHelper(LuaState* state, Object* pObj)
	{
		std::string metaname("MetaClass_");
		metaname += typeid(Object).name();
		
		LuaObject obj = state->BoxPointer(pObj);
		obj.SetMetaTable(state->GetGlobal(metaname.c_str()));
		obj.Push();
		return 1;
	}

public:

	static	int Constructor(LuaState* state)
	{
		return ConstructorHelper(state, new Object());
    }

	static int Destructor(LuaState* state)
	{
		LuaObject o(state, 1);

		delete (Object*)state->UnBoxPointer(1);

		LuaObject meta = state->GetGlobal("MetaClass_Nil");
		if(meta.IsNil())
		{
			meta = state->GetGlobals().CreateTable("MetaClass_Nil");
		}

		o.SetMetaTable(meta);
		return 0;
	}
};

template<typename Object>
class LuaClass
{
public:
	LuaClass(LuaState* state)
	{
		luaGlobals = state->GetGlobals();

		std::string metaname("MetaClass_");
		metaname += typeid(Object).name();

		metaTableObj = luaGlobals.CreateTable(metaname.c_str());
		metaTableObj.SetObject("__index", metaTableObj);
		//metaTableObj.Register("__gc", &Destructor);
		//metaTableObj.Register("Free", &Destructor);
	}

	template<typename Func>
		inline LuaClass& def(const char* name, Func func)
	{
		metaTableObj.RegisterObjectDirect(name, (Object*) 0, func);
		return *this;
	}

	inline LuaClass& create(const char* name)
	{
		luaGlobals.Register(name, LuaConstructor<Object>::Constructor);
		return *this;
	}

	template<typename A1>
        LuaClass& create(const char* name)
	{
		luaGlobals.Register(name, Constructor1c<Object, A1>);
		return *this;
	}
	template<typename A1, typename A2>
        LuaClass& create(const char* name)
	{
		luaGlobals.Register(name, Constructor2c<Object, A1, A2>);
		return *this;
	}
	template<typename A1, typename A2, typename A3>
        LuaClass& create(const char* name)
	{
		luaGlobals.Register(name, Constructor3c<Object, A1, A2, A3>);
		return *this;
	}
	template<typename A1, typename A2, typename A3, typename A4>
        LuaClass& create(const char* name)
	{
		luaGlobals.Register(name, Constructor4c<Object, A1, A2, A3, A4>);
		return *this;
	}
	template<typename A1, typename A2, typename A3, typename A4, typename A5>
        LuaClass& create(const char* name)
	{
		luaGlobals.Register(name, Constructor5c<Object, A1, A2, A3, A4, A5>);
		return *this;
	}
    template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
    LuaClass& create(const char* name)
	{
		luaGlobals.Register(name, Constructor6c<Object, A1, A2, A3, A4, A5, A6>);
		return *this;
	}
    template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
    LuaClass& create(const char* name)
	{
		luaGlobals.Register(name, Constructor7c<Object, A1, A2, A3, A4, A5, A6, A7>);
		return *this;
	}
    template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
    LuaClass& create(const char* name)
	{
		luaGlobals.Register(name, Constructor8c<Object, A1, A2, A3, A4, A5, A6, A7, A8>);
		return *this;
	}
	inline LuaClass& destroy(const char* name)
	{
		metaTableObj.Register(name, LuaConstructor<Object>::Destructor);
		return *this;
	}

private:
	LuaObject metaTableObj;
	LuaObject luaGlobals;
};

class LuaModule
{
public:
	LuaModule(LuaState* state)
	{
		luaModuleObj = state->GetGlobals();
	}

	LuaModule(LuaState* state, const char* name)
	{
		luaModuleObj = state->GetGlobals().CreateTable(name);
	}

	template<typename Func>
		inline LuaModule& def(const char* name, Func func)
	{
		luaModuleObj.RegisterDirect(name, func);
		return *this;
	}

	template<typename Object, typename Func>
		inline LuaModule& def(const char* name, Object& o, Func func)
	{
		luaModuleObj.RegisterDirect(name, o, func);
		return *this;
	}

private:
	LuaObject luaModuleObj;
};

#endif