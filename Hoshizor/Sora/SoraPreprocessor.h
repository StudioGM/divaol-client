//
//  SoraPreprocessor.h
//  zeromq_client
//
//  Created by Ruiwei Bu on 8/16/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#define SORA_REPEAT_1_0(m, d)
#define SORA_REPEAT_1_1(m, d) m(0, d)
#define SORA_REPEAT_1_2(m, d) SORA_REPEAT_1_1(m, d) m(1, d)
#define SORA_REPEAT_1_3(m, d) SORA_REPEAT_1_2(m, d) m(2, d)
#define SORA_REPEAT_1_4(m, d) SORA_REPEAT_1_3(m, d) m(3, d)
#define SORA_REPEAT_1_5(m, d) SORA_REPEAT_1_4(m, d) m(4, d)
#define SORA_REPEAT_1_6(m, d) SORA_REPEAT_1_5(m, d) m(5, d)
#define SORA_REPEAT_1_7(m, d) SORA_REPEAT_1_6(m, d) m(6, d)
#define SORA_REPEAT_1_8(m, d) SORA_REPEAT_1_7(m, d) m(7, d)
#define SORA_REPEAT_1_9(m, d) SORA_REPEAT_1_8(m, d) m(8, d)
#define SORA_REPEAT_1_10(m, d) SORA_REPEAT_1_9(m, d) m(9, d)
#define SORA_REPEAT_1_11(m, d) SORA_REPEAT_1_10(m, d) m(10, d)
#define SORA_REPEAT_1_12(m, d) SORA_REPEAT_1_11(m, d) m(11, d)

#define SORA_REPEAT_2_0(m, d, d2) 
#define SORA_REPEAT_2_1(m, d, d2) m(0, d, d2)
#define SORA_REPEAT_2_2(m, d, d2) SORA_REPEAT_2_1(m, d, d2) m(1, d, d2)
#define SORA_REPEAT_2_3(m, d, d2) SORA_REPEAT_2_2(m, d, d2) m(2, d, d2)
#define SORA_REPEAT_2_4(m, d, d2) SORA_REPEAT_2_3(m, d, d2) m(3, d, d2)
#define SORA_REPEAT_2_5(m, d, d2) SORA_REPEAT_2_4(m, d, d2) m(4, d, d2)
#define SORA_REPEAT_2_6(m, d, d2) SORA_REPEAT_2_5(m, d, d2) m(5, d, d2)
#define SORA_REPEAT_2_7(m, d, d2) SORA_REPEAT_2_6(m, d, d2) m(6, d, d2)
#define SORA_REPEAT_2_8(m, d, d2) SORA_REPEAT_2_7(m, d, d2) m(7, d, d2)
#define SORA_REPEAT_2_9(m, d, d2) SORA_REPEAT_2_8(m, d, d2) m(8, d, d2)
#define SORA_REPEAT_2_10(m, d, d2) SORA_REPEAT_2_9(m, d, d2) m(9, d, d2)
#define SORA_REPEAT_2_11(m, d, d2) SORA_REPEAT_2_10(m, d, d2) m(10, d, d2)
#define SORA_REPEAT_2_12(m, d, d2) SORA_REPEAT_2_11(m, d, d2) m(11, d, d2)

// repeat definition macro
// would expanded to m([0-(n-1)], d)
// m should be a macro provided by user, receiving two params
// for example
//      #define expand(n, d) d##n = n;
//      SORA_REPEAT_N(4, expand, myVar)
// would be expanded to
// myVar0 = 0; myVar1 = 1; myVar2 = 2; myVar3 = 3;
// n max = 12
#define SORA_REPEAT_N(n, m, d) SORA_REPEAT_1_##n(m, d)
#define SORA_REPEAT_N_2(n, m, d, d2) SORA_REPEAT_2_##n(m, d, d2)

#define SORA_ENUM_N(n, m, d) SORA_REPEAT_N_2(n, SORA_ENUM_M, m, d)

#define SORA_ENUM_M(n, m, d) SORA_COMMA_IF(n) m(n, d)

// function param repeat definition macro
// would expanded to d##0, d##1, d##2, d##n-1
// n max = 12
#define SORA_ENUM_PARAMS(n, d) SORA_REPEAT_N(n, SORA_FUNCTION_M, d)

#define SORA_PARAM_REPEAT(n, m, d)

#define SORA_BOOL_0 0
#define SORA_BOOL_1 1
#define SORA_BOOL_2 1
#define SORA_BOOL_3 1
#define SORA_BOOL_4 1
#define SORA_BOOL_5 1
#define SORA_BOOL_6 1
#define SORA_BOOL_7 1
#define SORA_BOOL_8 1
#define SORA_BOOL_9 1
#define SORA_BOOL_10 1
#define SORA_BOOL_11 1
#define SORA_BOOL_12 1
#define SORA_BOOL_13 1
#define SORA_BOOL_14 1
#define SORA_BOOL_15 1
#define SORA_BOOL_16 1
#define SORA_BOOL_17 1
#define SORA_BOOL_18 1
#define SORA_BOOL_19 1
#define SORA_BOOL_20 1

#define SORA_COMMA() ,
#define SORA_EMPTY() 

#define SORA_IF_IIF_0(t, f) t
#define SORA_IF_IIF_1(t, f) f

#define SORA_BOOL_N(n) SORA_BOOL_##n

#define SORA_IIF_I(cond, t, f) SORA_IF_IIF_##cond(t, f)
#define SORA_IIF(cond, t, f) SORA_IIF_I(cond, t, f)

// condition select macro
// cond must be a number >= 0
// if cond = 0, then t is used
// otherwise f is used
#define SORA_IF(cond, t, f) SORA_IIF(SORA_BOOL_N(cond), t, f)

#define SORA_COMMA_IF(n) SORA_IF(n, SORA_EMPTY, SORA_COMMA)()

#define SORA_FUNCTION_M(n, d) SORA_COMMA_IF(n) d ## n

// cat
#define SORA_CAT(a, b) SORA_CAT_I(a, b)
#define SORA_CAT_I(a, b) a##b

#define SORA_PP_DEC_0 0
#define SORA_PP_DEC_1 0
#define SORA_PP_DEC_2 1
#define SORA_PP_DEC_3 2
#define SORA_PP_DEC_4 3
#define SORA_PP_DEC_5 4
#define SORA_PP_DEC_6 5
#define SORA_PP_DEC_7 6
#define SORA_PP_DEC_8 7
#define SORA_PP_DEC_9 8
#define SORA_PP_DEC_10 9
#define SORA_PP_DEC_11 10
#define SORA_PP_DEC_12 11
#define SORA_PP_DEC_13 12
#define SORA_PP_DEC_14 13
#define SORA_PP_DEC_15 14
#define SORA_PP_DEC_16 15
#define SORA_PP_DEC_17 16
#define SORA_PP_DEC_18 17
#define SORA_PP_DEC_19 18
#define SORA_PP_DEC_20 19
#define SORA_PP_DEC_21 20
#define SORA_PP_DEC_22 21
#define SORA_PP_DEC_23 22
#define SORA_PP_DEC_24 23
#define SORA_PP_DEC_25 24
#define SORA_PP_DEC_26 25
#define SORA_PP_DEC_27 26
#define SORA_PP_DEC_28 27
#define SORA_PP_DEC_29 28
#define SORA_PP_DEC_30 29




