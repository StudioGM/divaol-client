/*
**  =======================================================
**              Galaxy2D Game Engine       
**                                
**       ��Ȩ����(c) 2005 ����. ��������Ȩ��.
**    ��ҳ��ַ: http://www.cppblog.com/jianguhan/
**			 ��������: jianguhan@126.com
**  =======================================================
*/

/*
	����C++�ൽLuaģ��

	��ģ�����ڵ����ൽLua�У�
	֧�ּ̳кͶ�̳���ĵ�����
	���ҿ���Lua�ű���̳е����ࡣ

	ע�⣺
	1.ʹ��LuaClassģ����Ҫluaplus�⡣
	2.������Coroutine��ʹ�á�

	***********************************************************
	ʹ��˵����
	***********************************************************

	-----------------------------------------------------------
	������ LuaClass<Class>(LuaState, ClassName, P)
	-----------------------------------------------------------

	ClassNameΪ���������������Ժ��ԣ�������Խ���������ർ��Lua�������ڼ̳л򵼳���ָ����ʹ�á�
	PΪ������ָ�룬���ڵ������ͬʱ���������ʵ�������ʹ�����������Ҫָ��ClassName��

	class A
	{
	public:
		void Fun1() {}
		virtual void Fun2() {}
	};

	A *p = new A;
	LuaClass<A>(LuaState, "A", p)
	.def("Fun1", &A::Fun1)	//�������еĺ���
	.def("Fun2", &A::Fun2);

	���ϵ�������Lua�п��������������ϵĺ�����

	A:Fun1()
	A:Fun2()

	-----------------------------------------------------------
	�����̳��� LuaClassInherit<A, B>(LuaState, ClassName, P)
	-----------------------------------------------------------

	class B:public A
	{
	public:
		void Fun2() {}
		void Fun3() {}
	}

	�ȵ������ࣺ

	LuaClass<A>(LuaState)
	.def("Fun1", &A::Fun1)
	.def("Fun2", &A::Fun2);

	�ٵ����̳��ࣺ

	LuaClassInherit<A, B>(LuaState, classname, p)
	.def("Fun2", &B::Fun2)
	.def("Fun2", &B::Fun3);

	���B�Ƕ�̳��࣬���绹��C��̳У�ֻ���ٵ���һ�μ��ɣ�

	LuaClassInherit<C, B>(LuaState, classname, p);

	-----------------------------------------------------------
	������ָ�� LuaExportPointer<A>(LuaState, p)
	-----------------------------------------------------------
	�ú�������һ��LuaObject��
	����ֱ�Ӵ���Lua��

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
				sprintf(buf, "����MetaTable%sʧ��!", mname);
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
				sprintf(buf, "����%s�ĵ�����δ�ҵ�!", pname);
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

	// ������
	template<typename Object> class LuaClass:public LuaClassBase<Object>
	{
	public:
		LuaClass(LuaState *state, const char* classname = 0, void *p = 0):LuaClassBase<Object>(state)
		{
			LuaClassBase<Object>::CreateClassMetaTable(typeid(Object).name());
			LuaClassBase<Object>::ExportClass(classname, p);
		}
	};

	// �����̳���
	template<typename ParentClass, typename Object> class LuaClassInherit:public LuaClassBase<Object>
	{
	public:
		LuaClassInherit(LuaState *state, const char* classname = 0, void *p = 0):LuaClassBase<Object>(state)
		{
			LuaClassBase<Object>::CreateInheritMetaTable(typeid(ParentClass).name(), typeid(Object).name());
			LuaClassBase<Object>::ExportClass(classname, p);
		}
	};

	// ������ָ��
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
			sprintf(buf, "����%sʧ��!", name);
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