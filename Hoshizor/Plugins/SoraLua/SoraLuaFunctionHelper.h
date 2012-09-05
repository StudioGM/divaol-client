/*
 *  SoraLuaFunctionHelper.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/3/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_LUA_FUNCTION_HELPER_H_
#define SORA_LUA_FUNCTION_HELPER_H_

#include "SoraLua/LuaPlus/LuaPlus.h"

namespace sora {
	
		
	template<typename P>
	void callLuaFunc(LuaPlus::LuaState* state, const SoraString& name, P param) {
		LuaPlus::LuaObject func = state->GetGlobal(name.c_str());
		if(func.IsFunction()) {
			LuaPlus::LuaFunctionVoid realFunc =(LuaPlus::LuaFunctionVoid)func;
			realFunc(param);
		}
	}
	
	template<typename P, typename P2>
	void callLuaFunc(LuaPlus::LuaState* state, const SoraString& name, P p1, P2 p2) {
		LuaPlus::LuaObject func = state->GetGlobal(name.c_str());
		if(func.IsFunction()) {
			LuaPlus::LuaFunctionVoid realFunc =(LuaPlus::LuaFunctionVoid)func;
			realFunc(p1, p2);
		}
	}
	
	template<typename P, typename P2, typename P3>
	void callLuaFunc(LuaPlus::LuaState* state, const SoraString& name, P p1, P2 p2, P3 p3) {
		LuaPlus::LuaObject func = state->GetGlobal(name.c_str());
		if(func.IsFunction()) {
			LuaPlus::LuaFunctionVoid realFunc =(LuaPlus::LuaFunctionVoid)func;
			realFunc(p1, p2, p3);
		}
	}
	
	template<typename P, typename P2, typename P3, typename P4>
	void callLuaFunc(LuaPlus::LuaState* state, const SoraString& name, P p1, P2 p2, P3 p3, P4 p4) {
		LuaPlus::LuaObject func = state->GetGlobal(name.c_str());
		if(func.IsFunction()) {
			LuaPlus::LuaFunctionVoid realFunc =(LuaPlus::LuaFunctionVoid)func;
			realFunc(p1, p2, p3, p4);
		}
	}
	
	template<typename P, typename P2, typename P3, typename P4, typename P5>
	void callLuaFunc(LuaPlus::LuaState* state, const SoraString& name, P p1, P2 p2, P3 p3, P4 p4, P5 p5) {
		LuaPlus::LuaObject func = state->GetGlobal(name.c_str());
		if(func.IsFunction()) {
			LuaPlus::LuaFunctionVoid realFunc =(LuaPlus::LuaFunctionVoid)func;
			realFunc(p1, p2, p3, p4, p5);
		}
	}
	
	template<typename P, typename P2, typename P3, typename P4, typename P5, typename P6>
	void callLuaFunc(LuaPlus::LuaState* state, const SoraString& name, P p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) {
		LuaPlus::LuaObject func = state->GetGlobal(name.c_str());
		if(func.IsFunction()) {
			LuaPlus::LuaFunctionVoid realFunc =(LuaPlus::LuaFunctionVoid)func;
			realFunc(p1, p2, p3, p4, p5, p6);
		}
	}
	
	template<typename P, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	void callLuaFunc(LuaPlus::LuaState* state, const SoraString& name, P p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) {
		LuaPlus::LuaObject func = state->GetGlobal(name.c_str());
		if(func.IsFunction()) {
			LuaPlus::LuaFunctionVoid realFunc =(LuaPlus::LuaFunctionVoid)func;
			realFunc(p1, p2, p3, p4, p5, p6, p7);
		}
	}
	
	template<typename P, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	void callLuaFunc(LuaPlus::LuaState* state, const SoraString& name, P p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8) {
		LuaPlus::LuaObject func = state->GetGlobal(name.c_str());
		if(func.IsFunction()) {
			LuaPlus::LuaFunctionVoid realFunc =(LuaPlus::LuaFunctionVoid)func;
			realFunc(p1, p2, p3, p4, p5, p6, p7, p8);
		}
	}
	
	template<typename RT>
	RT callLuaFunc(LuaPlus::LuaState* state, const SoraString& name) {
		LuaPlus::LuaObject func = state->GetGlobal(name.c_str());
		if(func.IsFunction()) {
			LuaPlus::LuaFunction<RT> realFunc =(LuaPlus::LuaFunction<RT>)func;
			return realFunc();
		}
	}
	
	template<typename RT, typename P>
	RT callLuaFunc(LuaPlus::LuaState* state, const SoraString& name, P param) {
		LuaPlus::LuaObject func = state->GetGlobal(name.c_str());
		if(func.IsFunction()) {
			LuaPlus::LuaFunction<RT> realFunc =(LuaPlus::LuaFunction<RT>)func;
			return realFunc(param);
		}
	}
	
	template<typename RT, typename P, typename P2>
	RT callLuaFunc(LuaPlus::LuaState* state, const SoraString& name, P p1, P2 p2) {
		LuaPlus::LuaObject func = state->GetGlobal(name.c_str());
		if(func.IsFunction()) {
			LuaPlus::LuaFunction<RT> realFunc =(LuaPlus::LuaFunction<RT>)func;
			return realFunc(p1, p2);
		}
	}
	
	template<typename RT, typename P, typename P2, typename P3>
	RT callLuaFunc(LuaPlus::LuaState* state, const SoraString& name, P p1, P2 p2, P3 p3) {
		LuaPlus::LuaObject func = state->GetGlobal(name.c_str());
		if(func.IsFunction()) {
			LuaPlus::LuaFunction<RT> realFunc =(LuaPlus::LuaFunction<RT>)func;
			return realFunc(p1, p2, p3);
		}
	}
	
	template<typename RT, typename P, typename P2, typename P3, typename P4>
	RT callLuaFunc(LuaPlus::LuaState* state, const SoraString& name, P p1, P2 p2, P3 p3, P4 p4) {
		LuaPlus::LuaObject func = state->GetGlobal(name.c_str());
		if(func.IsFunction()) {
			LuaPlus::LuaFunction<RT> realFunc =(LuaPlus::LuaFunction<RT>)func;
			return realFunc(p1, p2, p3, p4);
		}
	}
	
	template<typename RT, typename P, typename P2, typename P3, typename P4, typename P5>
	RT callLuaFunc(LuaPlus::LuaState* state, const SoraString& name, P p1, P2 p2, P3 p3, P4 p4, P5 p5) {
		LuaPlus::LuaObject func = state->GetGlobal(name.c_str());
		if(func.IsFunction()) {
			LuaPlus::LuaFunction<RT> realFunc =(LuaPlus::LuaFunction<RT>)func;
			return realFunc(p1, p2, p3, p4, p5);
		}
	}
	
	template<typename RT, typename P, typename P2, typename P3, typename P4, typename P5, typename P6>
	RT callLuaFunc(LuaPlus::LuaState* state, const SoraString& name, P p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) {
		LuaPlus::LuaObject func = state->GetGlobal(name.c_str());
		if(func.IsFunction()) {
			LuaPlus::LuaFunction<RT> realFunc =(LuaPlus::LuaFunction<RT>)func;
			return realFunc(p1, p2, p3, p4, p5, p6);
		}
	}
	
	template<typename RT, typename P, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	RT callLuaFunc(LuaPlus::LuaState* state, const SoraString& name, P p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) {
		LuaPlus::LuaObject func = state->GetGlobal(name.c_str());
		if(func.IsFunction()) {
			LuaPlus::LuaFunction<RT> realFunc =(LuaPlus::LuaFunction<RT>)func;
			return realFunc(p1, p2, p3, p4, p5, p6, p7);
		}
	}
	
	template<typename RT, typename P, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	RT callLuaFunc(LuaPlus::LuaState* state, const SoraString& name, P p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8) {
		LuaPlus::LuaObject func = state->GetGlobal(name.c_str());
		if(func.IsFunction()) {
			LuaPlus::LuaFunction<RT> realFunc =(LuaPlus::LuaFunction<RT>)func;
			return realFunc(p1, p2, p3, p4, p5, p6, p7, p8);
		}
	}
	
} // namespace sora

#endif // SORA_LUA_FUNCTION_HELPER_H_