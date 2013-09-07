/****************************************************
 *  Observer.h
 *
 *  Created by tempbuffer on 9/27/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  A base class which is singleton
 ****************************************************/

#ifndef _BASE_PUBLISHER_
#define _BASE_PUBLISHER_

#include "../Common.h"
#include "../Function/Function.h"
#include <vector>
#include <set>

namespace Base
{
	using std::vector;
	using std::map;

	template<typename sig>
	class Publisher {
		typedef Function<sig> func_type;
		std::vector<func_type> once_funcs;
		std::vector<func_type> funcs;
	public:
		bool empty() const {return once_funcs.empty() && funcs.empty();}
		void clear() {funcs.clear(); once_funcs.clear();}

		Publisher& operator+=(func_type func) {
			return on(func);
		}
		Publisher& operator-=(func_type func) {
			return remove(func);
		}
		Publisher& on(func_type func) {
			for (func_type &_func : funcs) {
				if (_func == func)
					return *this;
			}
			funcs.push_back(func);
			return *this;
		}
		Publisher& once(func_type func) {
			for (func_type &_func : once_funcs) {
				if (_func == func)
					return *this;
			}
			once_funcs.push_back(func);
			return *this;
		}
		Publisher& remove(func_type func) {
			std::remove(funcs.begin(), funcs.end(), func);
			std::remove(once_funcs.begin(), once_funcs.end(), func);
			return *this;
		}

		void operator()() {
			for (func_type &func : funcs)
				func();
			for (func_type &func : once_funcs)
				func();
			once_funcs.clear();
		}
		template<typename T0>
		void operator()(T0 a0) {
			for (func_type &func : funcs)
				func(a0);
			for (func_type &func : once_funcs)
				func(a0);
			once_funcs.clear();
		}
		template<typename T0, typename T1>
		void operator()(T0 a0, T1 a1) {
			for (func_type &func : funcs)
				func(a0, a1);
			for (func_type &func : once_funcs)
				func(a0, a1);
			once_funcs.clear();
		}
		template<typename T0, typename T1, typename T2>
		void operator()(T0 a0, T1 a1, T2 a2) {
			for (func_type &func : funcs)
				func(a0, a1, a2);
			for (func_type &func : once_funcs)
				func(a0, a1, a2);
			once_funcs.clear();
		}
		template<typename T0, typename T1, typename T2, typename T3>
		void operator()(T0 a0, T1 a1, T2 a2, T3 a3) {
			for (func_type &func : funcs)
				func(a0, a1, a2, a3);
			for (func_type &func : once_funcs)
				func(a0, a1, a2, a3);
			once_funcs.clear();
		}
		template<typename T0, typename T1, typename T2, typename T3, typename T4>
		void operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4) {
			for (func_type &func : funcs)
				func(a0, a1, a2, a3, a4);
			for (func_type &func : once_funcs)
				func(a0, a1, a2, a3, a4);
			once_funcs.clear();
		}
		template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
		void operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
			for (func_type &func : funcs)
				func(a0, a1, a2, a3, a4, a5);
			for (func_type &func : once_funcs)
				func(a0, a1, a2, a3, a4, a5);
			once_funcs.clear();
		}
		template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
		void operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) {
			for (func_type &func : funcs)
				func(a0, a1, a2, a3, a4, a5, a6);
			for (func_type &func : once_funcs)
				func(a0, a1, a2, a3, a4, a5, a6);
			once_funcs.clear();
		}
		template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
		void operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) {
			for (func_type &func : funcs)
				func(a0, a1, a2, a3, a4, a5, a6, a7);
			for (func_type &func : once_funcs)
				func(a0, a1, a2, a3, a4, a5, a6, a7);
			once_funcs.clear();
		}
		template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
		void operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) {
			for (func_type &func : funcs)
				func(a0, a1, a2, a3, a4, a5, a6, a7, a8);
			for (func_type &func : once_funcs)
				func(a0, a1, a2, a3, a4, a5, a6, a7, a8);
			once_funcs.clear();
		}
		template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
		void operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9) {
			for (func_type &func : funcs)
				func(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
			for (func_type &func : once_funcs)
				func(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
			once_funcs.clear();
		}
	};
}

#endif