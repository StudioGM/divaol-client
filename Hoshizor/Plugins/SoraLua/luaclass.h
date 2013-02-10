/*
**  =======================================================
**              Galaxy2D Game Engine       
**                                
**       版权所有(c) 2005 沈明. 保留所有权利.
**    主页地址: http://www.cppblog.com/jianguhan/
**			 电子邮箱: jianguhan@126.com
**  =======================================================
*/

/*
	导出C++类到Lua模块

	该模块用于导出类到Lua中，
	支持继承和多继承类的导出，
	并且可在Lua脚本里继承导出类。

	注意：
	1.使用LuaClass模块需要luaplus库。
	2.不可在Coroutine内使用。

	***********************************************************
	使用说明：
	***********************************************************

	-----------------------------------------------------------
	导出类 LuaClass<Class>(LuaState, ClassName, P)
	-----------------------------------------------------------

	ClassName为导出的类名，可以忽略，如果忽略仅仅把这个类导到Lua中用以在继承或导出类指针中使用。
	P为导出类指针，用于导出类的同时导出该类的实例，如果使用这个参数需要指定ClassName。

	class A
	{
	public:
		void Fun1() {}
		virtual void Fun2() {}
	};

	A *p = new A;
	LuaClass<A>(LuaState, "A", p)
	.def("Fun1", &A::Fun1)	//导出类中的函数
	.def("Fun2", &A::Fun2);

	如上导出后在Lua中可以这样调用类上的函数：

	A:Fun1()
	A:Fun2()

	-----------------------------------------------------------
	导出继承类 LuaClassInherit<A, B>(LuaState, ClassName, P)
	-----------------------------------------------------------

	class B:public A
	{
	public:
		void Fun2() {}
		void Fun3() {}
	}

	先导出基类：

	LuaClass<A>(LuaState)
	.def("Fun1", &A::Fun1)
	.def("Fun2", &A::Fun2);

	再导出继承类：

	LuaClassInherit<A, B>(LuaState, classname, p)
	.def("Fun2", &B::Fun2)
	.def("Fun2", &B::Fun3);

	如果B是多继承类，比如还从C类继承，只需再导出一次即可：

	LuaClassInherit<C, B>(LuaState, classname, p);

	-----------------------------------------------------------
	导出类指针 LuaExportPointer<A>(LuaState, p)
	-----------------------------------------------------------
	该函数返回一个LuaObject，
	可以直接传给Lua：

	A *p = new A;
	LuaObject obj = LuaState->GetGlobals()["LuaFunc"];
	LuaFunction<void> LuaFunc = obj;
	LuaFunc(LuaExportPointer<A>(LuaState, p))
*/
#ifndef LUA_CLASS_H
#define LUA_CLASS_H

#include <typeinfo>
#include <string>
#include "LuaPlus/LuaPlus.h"

namespace gge
{
	using namespace LuaPlus;

	template<typename Object> class LuaClassBase
	{
	public:
		LuaClassBase(LuaState *state):m_luastate(state) {}

		template<typename Func>	inline LuaClassBase& def(const char* name, Func func)
		{
			m_metaTableObj.RegisterObjectDirect(name, (Object*) 0, func);
			return *this;
		}

	protected:
		void CreateClassMetaTable(const char* mname)
		{
			if (!m_luastate) return;

			std::string metaTableStr("MetaClassGGE_");
			metaTableStr += mname;

			LuaObject table = m_luastate->GetGlobals();
			m_metaTableObj = table.CreateTable(metaTableStr.c_str());
			m_metaTableObj.SetObject("__index", m_metaTableObj);

			if (!table[metaTableStr.c_str()].IsTable())
			{		
				char buf[512];
				sprintf(buf, "创建MetaTable%s失败!", mname);
		//		MessageBoxA(NULL, buf, "Error", MB_OK);
			}
		}

		void CreateInheritMetaTable(const char* pname, const char* mname)
		{
			if (!m_luastate) return;

			std::string ParentClassMetaTableName("MetaClassGGE_");
			ParentClassMetaTableName += pname;

			std::string metaTableStr("MetaClassGGE_");
			metaTableStr += mname;

			LuaObject ParentMetaTable = m_luastate->GetGlobal(ParentClassMetaTableName.c_str());

			if (!ParentMetaTable.IsTable()) 
			{
				char buf[512];
				sprintf(buf, "父类%s的导出类未找到!", pname);
		//		MessageBox(NULL, buf, "Error", MB_OK);
				return;
			}

			LuaObject ParentMetaIndex = ParentMetaTable["__index"];
			ParentMetaTable.SetNil("__index");
			m_metaTableObj = ParentMetaTable.Clone();
			ParentMetaTable.SetObject("__index", ParentMetaIndex);
			m_metaTableObj.SetObject("__index", m_metaTableObj);
			m_luastate->GetGlobals().SetObject(metaTableStr.c_str(), m_metaTableObj);
		}
		
		void ExportClass(const char* classname, void *p)
		{
			if (!m_luastate) return;

			if (p)
			{
				LuaObject tableObj = m_luastate->GetGlobals().CreateTable(classname);
				tableObj.SetLightUserData("__object", p);
				tableObj.SetMetaTable(m_metaTableObj);
			}
			else if (classname) m_luastate->GetGlobals().SetObject(classname, m_metaTableObj);
		}

	protected:
		LuaState	*m_luastate;
		LuaObject	m_metaTableObj;
	};

	// 导出类
	template<typename Object> class LuaClass:public LuaClassBase<Object>
	{
	public:
		LuaClass(LuaState *state, const char* classname = 0, void *p = 0):LuaClassBase<Object>(state)
		{
			LuaClassBase<Object>::CreateClassMetaTable(typeid(Object).name());
			LuaClassBase<Object>::ExportClass(classname, p);
		}
	};

	// 导出继承类
	template<typename ParentClass, typename Object> class LuaClassInherit:public LuaClassBase<Object>
	{
	public:
		LuaClassInherit(LuaState *state, const char* classname = 0, void *p = 0):LuaClassBase<Object>(state)
		{
			LuaClassBase<Object>::CreateInheritMetaTable(typeid(ParentClass).name(), typeid(Object).name());
			LuaClassBase<Object>::ExportClass(classname, p);
		}
	};

	// 导出类指针
	template<typename Object> inline LuaObject LuaExportPointer(LuaState *state, void *p)
	{
		if (!state || !p)
		{
			LuaObject obj(state);
			return obj;
		}

		const char* name = typeid(Object).name();

		std::string metaTableStr("MetaClassGGE_");
		metaTableStr += name;

		LuaObject metaTableObj = state->GetGlobal(metaTableStr.c_str());
		if (!metaTableObj.IsTable()) 
		{
			char buf[512];
			sprintf(buf, "导出%s失败!", name);
	//		MessageBox(NULL, buf, "Error", MB_OK);
			return metaTableObj;
		}

		LuaObject tableObj;
		tableObj.AssignNewTable(state);
		tableObj.SetLightUserData("__object", p);
		tableObj.SetMetaTable(metaTableObj);

		return tableObj;
	}
}

#endif