/****************************************************
 *  FunctionBase.h
 *
 *  Created by tempbuffer on 9/17/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  
 ****************************************************/

#ifndef _BASE_FUNCTION_BASE_H_
#define _BASE_FUNCTION_BASE_H_

#include "../TypeTraits.h"

namespace Base
{
	namespace function_detial {
		union any_ptr {
			void *value;

			void *obj_ptr;
			const void* const_obj_ptr;
			void (*func_ptr);
			
			char data;
		};

		inline any_ptr make_any_pointer(void *o) {
			any_ptr ptr;
			ptr.obj_ptr = o;
			return ptr;
		}

		inline any_ptr make_any_pointer(const void* o) {
			any_ptr ptr;
			ptr.const_obj_ptr = o;
			return ptr;
		}

		inline any_ptr make_any_pointer(void (*func)()) {
			any_ptr ptr;
			ptr.func_ptr = func;
			return ptr;
		}

		struct function_ptr_tag {};
		struct function_obj_tag {};
		struct member_ptr_tag {};
		struct function_obj_ref_tag {};

		template<typename F>
		class get_function_tag {
			typedef typename Traits::if_selector<Traits::is_pointer<F>::value,
				function_ptr_tag,
				function_obj_tag>::type ptr_or_obj_tag;

			typedef typename Traits::if_selector<Traits::is_member_pointer<F>::value,
				member_ptr_tag,
				ptr_or_obj_tag>::type ptr_or_obj_or_mem_tag;

			typedef typename Traits::if_selector<Traits::is_referrence_wrapper<F>::value,
				function_obj_ref_tag,
				ptr_or_obj_or_mem_tag>::type or_ref_tag;

		public:
			typedef or_ref_tag type;
		};

		template<typename sig>
		class Function;

		template<typename sig, typename obj>
		class MemFunc;

		template<typename obj, typename sig>
		class Binded;
	
		#define BASE_FUNCTION_TEMPLATE_PARAM BASE_ENUM_PARAMS(BASE_FUNCTION_NUM_ARGS, typename T)
		#define BASE_FUNCTION_TEMPLATE_PARAM_N(n) BASE_ENUM_PARAMS(n, typename T)
		#define BASE_FUNCTION_TEMPLATE_ARGS BASE_ENUM_PARAMS(BASE_FUNCTION_NUM_ARGS, T)
		#define BASE_FUNCTION_PARAM(n, d) BASE_JOIN(T, n) BASE_JOIN(a, n)
		#define BASE_FUNCTION_PARAMS BASE_ENUM_N(BASE_FUNCTION_NUM_ARGS, BASE_FUNCTION_PARAM, BASE_EMPTY)
		#define BASE_FUNCTION_PARAMS_N BASE_ENUM_N(n, BASE_FUNCTION_PARAM, BASE_EMPTY)
		#define BASE_FUNCTION_ARGS BASE_ENUM_PARAMS(BASE_FUNCTION_NUM_ARGS, a)
	}
}

#endif