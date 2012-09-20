/****************************************************
 *  PreDefine
 *
 *  Created by tempbuffer on 9/17/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 ****************************************************/

#ifndef _BASE_PRE_DEFINE_H_
#define _BASE_PRE_DEFINE_H_

namespace Base
{
	// defines

	// prevent multi macro spread 
	#define BASE_JOIN(x, y) BASE_DO_JOIN(x, y)
	#define BASE_DO_JOIN(x, y) BASE_DO_JOIN_2(x, y)
	#define BASE_DO_JOIN_2(x, y) x##y

	#define BASE_UNIQUE_NAME(name)\
		BASE_JOIN(name, __LINE__)

	#define BASE_REGISTER_CLASS_NAME(_name)\
		virtual std::string getName() {return #_name;} \
		static std::string name() {return #_name;}

	#define BASE_BOOL_0 0
	#define BASE_BOOL_1 1
	#define BASE_BOOL_2 1
	#define BASE_BOOL_3 1
	#define BASE_BOOL_4 1
	#define BASE_BOOL_5 1
	#define BASE_BOOL_6 1
	#define BASE_BOOL_7 1
	#define BASE_BOOL_8 1
	#define BASE_BOOL_9 1
	#define BASE_BOOL_10 1
	#define BASE_BOOL_11 1
	#define BASE_BOOL_12 1
	#define BASE_BOOL_13 1
	#define BASE_BOOL_14 1
	#define BASE_BOOL_15 1
	#define BASE_BOOL_16 1
	#define BASE_BOOL_17 1
	#define BASE_BOOL_18 1
	#define BASE_BOOL_19 1
	#define BASE_BOOL_20 1

	#define BASE_COMMA() ,
	#define BASE_EMPTY() 

	#define BASE_IF_IIF_0(t, f) t
	#define BASE_IF_IIF_1(t, f) f

	#define BASE_BOOL_N(n) BASE_BOOL_##n

	#define BASE_IIF_I(cond, t, f) BASE_IF_IIF_##cond(t, f)
	#define BASE_IIF(cond, t, f) BASE_IIF_I(cond, t, f)

		// condition select macro
		// cond must be a number >= 0
		// if cond = 0, then t is used
		// otherwise f is used
	#define BASE_IF(cond, t, f) BASE_IIF(BASE_BOOL_N(cond), t, f)

	#define BASE_COMMA_IF(n) BASE_IF(n, BASE_EMPTY, BASE_COMMA)()
	
	#define BASE_FUNCTION_MERGE_PARAM(n, d) BASE_COMMA_IF(n) d ## n

	#define BASE_REPEAT_1_0(m, d)
	#define BASE_REPEAT_1_1(m, d) m(0, d)
	#define BASE_REPEAT_1_2(m, d) BASE_REPEAT_1_1(m, d) m(1, d)
	#define BASE_REPEAT_1_3(m, d) BASE_REPEAT_1_2(m, d) m(2, d)
	#define BASE_REPEAT_1_4(m, d) BASE_REPEAT_1_3(m, d) m(3, d)
	#define BASE_REPEAT_1_5(m, d) BASE_REPEAT_1_4(m, d) m(4, d)
	#define BASE_REPEAT_1_6(m, d) BASE_REPEAT_1_5(m, d) m(5, d)
	#define BASE_REPEAT_1_7(m, d) BASE_REPEAT_1_6(m, d) m(6, d)
	#define BASE_REPEAT_1_8(m, d) BASE_REPEAT_1_7(m, d) m(7, d)
	#define BASE_REPEAT_1_9(m, d) BASE_REPEAT_1_8(m, d) m(8, d)
	#define BASE_REPEAT_1_10(m, d) BASE_REPEAT_1_9(m, d) m(9, d)
	#define BASE_REPEAT_1_11(m, d) BASE_REPEAT_1_10(m, d) m(10, d)
	#define BASE_REPEAT_1_12(m, d) BASE_REPEAT_1_11(m, d) m(11, d)

	#define BASE_REPEAT_2_0(m, d, d2) 
	#define BASE_REPEAT_2_1(m, d, d2) m(0, d, d2)
	#define BASE_REPEAT_2_2(m, d, d2) BASE_REPEAT_2_1(m, d, d2) m(1, d, d2)
	#define BASE_REPEAT_2_3(m, d, d2) BASE_REPEAT_2_2(m, d, d2) m(2, d, d2)
	#define BASE_REPEAT_2_4(m, d, d2) BASE_REPEAT_2_3(m, d, d2) m(3, d, d2)
	#define BASE_REPEAT_2_5(m, d, d2) BASE_REPEAT_2_4(m, d, d2) m(4, d, d2)
	#define BASE_REPEAT_2_6(m, d, d2) BASE_REPEAT_2_5(m, d, d2) m(5, d, d2)
	#define BASE_REPEAT_2_7(m, d, d2) BASE_REPEAT_2_6(m, d, d2) m(6, d, d2)
	#define BASE_REPEAT_2_8(m, d, d2) BASE_REPEAT_2_7(m, d, d2) m(7, d, d2)
	#define BASE_REPEAT_2_9(m, d, d2) BASE_REPEAT_2_8(m, d, d2) m(8, d, d2)
	#define BASE_REPEAT_2_10(m, d, d2) BASE_REPEAT_2_9(m, d, d2) m(9, d, d2)
	#define BASE_REPEAT_2_11(m, d, d2) BASE_REPEAT_2_10(m, d, d2) m(10, d, d2)
	#define BASE_REPEAT_2_12(m, d, d2) BASE_REPEAT_2_11(m, d, d2) m(11, d, d2)

	#define BASE_REPEAT_N(n, m, d) BASE_REPEAT_1_##n(m, d)
	#define BASE_REPEAT_N_2(n, m, d, d2) BASE_REPEAT_2_##n(m, d, d2)

	#define BASE_ENUM_M(n, m, d) BASE_COMMA_IF(n) m(n, d)
	#define BASE_ENUM_N(n, m, d) BASE_REPEAT_N_2(n, BASE_ENUM_M, m, d)
	#define BASE_ENUM_PARAMS(n, d) BASE_REPEAT_N(n, BASE_FUNCTION_MERGE_PARAM, d)
}

#endif