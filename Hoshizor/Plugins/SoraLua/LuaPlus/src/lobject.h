/*
** $Id: lobject.h,v 2.20.1.1 2007/12/27 13:02:25 roberto Exp $
** Type definitions for Lua objects
** See Copyright Notice in lua.h
*/


#ifndef lobject_h
#define lobject_h


#include <stdarg.h>


#include "llimits.h"
#include "lua.h"

NAMESPACE_LUA_BEGIN

/* tags for values visible from Lua */
#if LUA_WIDESTRING
#define LAST_TAG	LUA_TWSTRING
#else
#define LAST_TAG	LUA_TTHREAD
#endif /* LUA_WIDESTRING */

#define NUM_TAGS	(LAST_TAG+1)


/*
** Extra tags for non-values
*/
#define LUA_TPROTO	(LAST_TAG+1)
#define LUA_TUPVAL	(LAST_TAG+2)
#define LUA_TDEADKEY	(LAST_TAG+3)


/*
** Union of all collectable objects
*/
typedef union GCObject GCObject;


/*
** Common Header for all collectable objects (in macro form, to be
** included in other objects)
*/
#if LUA_REFCOUNT

#define CommonHeader	GCObject *next; GCObject* prev; lu_byte tt; lu_byte marked; unsigned short ref

#define luarc_addref(o) { TValue* i_o2 = (o); if(iscollectable(i_o2))	\
        { \
            gcvalue(i_o2)->gch.ref++; \
            lua_assert(gcvalue(i_o2)->gch.ref != 0); \
        }  }

#define luarc_addreftvalue(o) { gcvalue(o)->gch.ref++; lua_assert(gcvalue(o)->gch.ref != 0); }
#define luarc_addreftable(o) { o->ref++; lua_assert(o->ref != 0); }
#define luarc_addrefproto(o) { o->ref++; lua_assert(o->ref != 0); }
#define luarc_addrefupval(o) { o->ref++; lua_assert(o->ref != 0); }
#define luarc_addrefstring(o) { o->tsv.ref++; lua_assert(o->tsv.ref != 0); }

#define luarc_release(L,o) { TValue* i_o2 = (o); if(iscollectable(i_o2) && ((--gcvalue(i_o2)->gch.ref)<=0))	\
		{	\
			luarc_releaseobject(L,gcvalue(i_o2));	\
		}	}

#define luarc_releasetable(L,o) { Table* i_o2 = (o); if((--i_o2->ref)<=0) luarc_releaseobject(L,(GCObject*)i_o2); }
#define luarc_releaseproto(L,o) { Proto* i_o2 = (o); if((--i_o2->ref)<=0) luarc_releaseobject(L,(GCObject*)i_o2); }
#define luarc_releaseupval(L,o) { UpVal* i_o2 = (o); if((--i_o2->ref)<=0) luarc_releaseobject(L,(GCObject*)i_o2); }
#define luarc_releasestring(L,o) { TString* i_o2 = (o); if((--i_o2->tsv.ref)<=0) luarc_releaseobject(L,(GCObject*)i_o2); }

#define luarc_makevaluebackup(v) TValue bak = *v;

extern void luarc_releaseobject(lua_State *L, GCObject* header);

#define luarc_newvalue(o) { setnilvalue2n(L, (o)); }
#define luarc_newarray(from,to) { TValue* i_from = (from); TValue* i_to = (to); while (i_from < i_to) { luarc_newvalue(i_from++); } }
#define luarc_cleanvalue(o) { setnilvalue((o)); }
#define luarc_cleanarray(from,to) { TValue* i_from = (from); TValue* i_to = (to); while (i_from < i_to) { luarc_cleanvalue(i_from++); } }
#define luarc_cleanarrayreverse(to,from) { TValue* i_from = (from); TValue* i_to = (to); while (i_from >= i_to) { luarc_cleanvalue(i_from--); } }

#else /* !LUA_REFCOUNT */

#define CommonHeader	GCObject *next; lu_byte tt; lu_byte marked

#endif /* LUA_REFCOUNT */

/*
** Common header in struct form
*/
typedef struct GCheader {
  CommonHeader;
} GCheader;




/*
** Union of all Lua values
*/
typedef union {
  GCObject *gc;
  void *p;
  lua_Number n;
  int b;
} Value;


/*
** Tagged Values
*/

#define TValuefields	Value value; int tt

typedef struct lua_TValue {
  TValuefields;
} TValue;


/* Macros to test type */
#define ttisnil(o)	(ttype(o) == LUA_TNIL)
#define ttisnumber(o)	(ttype(o) == LUA_TNUMBER)
#define ttisstring(o)	(ttype(o) == LUA_TSTRING)
#define ttistable(o)	(ttype(o) == LUA_TTABLE)
#define ttisfunction(o)	(ttype(o) == LUA_TFUNCTION)
#define ttisboolean(o)	(ttype(o) == LUA_TBOOLEAN)
#define ttisuserdata(o)	(ttype(o) == LUA_TUSERDATA)
#define ttisthread(o)	(ttype(o) == LUA_TTHREAD)
#define ttislightuserdata(o)	(ttype(o) == LUA_TLIGHTUSERDATA)
#if LUA_WIDESTRING
#define ttiswstring(o)	(ttype(o) == LUA_TWSTRING)
#endif /* LUA_WIDESTRING */

/* Macros to access values */
#define ttype(o)	((o)->tt)
#define gcvalue(o)	check_exp(iscollectable(o), (o)->value.gc)
#define pvalue(o)	check_exp(ttislightuserdata(o), (o)->value.p)
#define nvalue(o)	check_exp(ttisnumber(o), (o)->value.n)
#define rawtsvalue(o)	check_exp(ttisstring(o), &(o)->value.gc->ts)
#define tsvalue(o)	(&rawtsvalue(o)->tsv)
#define rawuvalue(o)	check_exp(ttisuserdata(o), &(o)->value.gc->u)
#define uvalue(o)	(&rawuvalue(o)->uv)
#define clvalue(o)	check_exp(ttisfunction(o), &(o)->value.gc->cl)
#define hvalue(o)	check_exp(ttistable(o), &(o)->value.gc->h)
#define bvalue(o)	check_exp(ttisboolean(o), (o)->value.b)
#define thvalue(o)	check_exp(ttisthread(o), &(o)->value.gc->th)
#if LUA_WIDESTRING
#define rawtwsvalue(o)	check_exp(ttiswstring(o), &(o)->value.gc->ts)
#define twsvalue(o)	(&rawtwsvalue(o)->tsv)
#endif /* LUA_WIDESTRING */

#define l_isfalse(o)	(ttisnil(o) || (ttisboolean(o) && bvalue(o) == 0))

/*
** for internal debug only
*/
#define checkconsistency(obj) \
  lua_assert(!iscollectable(obj) || (ttype(obj) == (obj)->value.gc->gch.tt))

#define checkliveness(g,obj) \
  lua_assert(!iscollectable(obj) || \
  ((ttype(obj) == (obj)->value.gc->gch.tt) && !isdead(g, (obj)->value.gc)))


/* Macros to set values */
#if LUA_REFCOUNT
#define setnilvalue(obj) { TValue *i_o=(obj); luarc_release(L, i_o); i_o->tt=LUA_TNIL; }

#define setnvalue(obj,x) \
  { TValue *i_o=(obj); luarc_release(L, i_o); i_o->value.n=(x); i_o->tt=LUA_TNUMBER; }

#define setpvalue(obj,x) \
  { TValue *i_o=(obj); luarc_release(L, i_o); i_o->value.p=(x); i_o->tt=LUA_TLIGHTUSERDATA; }

#define setbvalue(obj,x) \
  { TValue *i_o=(obj); luarc_release(L, i_o); i_o->value.b=(x); i_o->tt=LUA_TBOOLEAN; }

#define setsvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    luarc_makevaluebackup(i_o); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TSTRING; \
    luarc_addreftvalue(i_o); luarc_release(L, &bak); \
    checkliveness(G(L),i_o); }

#define setuvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    luarc_makevaluebackup(i_o); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TUSERDATA; \
    luarc_addreftvalue(i_o); luarc_release(L, &bak); \
    checkliveness(G(L),i_o); }

#define setthvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    luarc_makevaluebackup(i_o); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TTHREAD; \
    luarc_addreftvalue(i_o); luarc_release(L, &bak); \
    checkliveness(G(L),i_o); }

#define setclvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    luarc_makevaluebackup(i_o); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TFUNCTION; \
    luarc_addreftvalue(i_o); luarc_release(L, &bak); \
    checkliveness(G(L),i_o); }

#define sethvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    luarc_makevaluebackup(i_o); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TTABLE; \
    luarc_addreftvalue(i_o); luarc_release(L, &bak); \
    checkliveness(G(L),i_o); }

#define setptvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    luarc_makevaluebackup(i_o); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TPROTO; \
    luarc_addreftvalue(i_o); luarc_release(L, &bak); \
    checkliveness(G(L),i_o); }

#define setobj(L,obj1,obj2) \
  { TValue *o2=(TValue *)(obj2); TValue *o1=(obj1); \
    luarc_addref(o2); luarc_release(L, o1); \
    o1->value = o2->value; o1->tt=o2->tt; \
    checkliveness(G(L),o1); }

#define setobj2n(L,obj1,obj2) \
  { TValue *o2=(TValue *)(obj2); TValue *o1=(obj1); \
    luarc_addref(o2); \
    o1->value = o2->value; o1->tt=o2->tt; \
    checkliveness(G(L),o1); }

#if LUA_WIDESTRING

#define setwsvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    luarc_makevaluebackup(i_o); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TWSTRING; \
    luarc_addreftvalue(i_o); luarc_release(L, &bak); \
    checkliveness(G(L),i_o); }

#endif /* LUA_WIDESTRING */

#define lua_addreftobject(obj)

#define setnvalue2n(obj,x) \
  { TValue *i_o=(obj); i_o->value.n=(x); i_o->tt=LUA_TNUMBER; }

#define setpvalue2n(obj,x) \
  { TValue *i_o=(obj); i_o->value.p=(x); i_o->tt=LUA_TLIGHTUSERDATA; }

#define setbvalue2n(obj,x) \
  { TValue *i_o=(obj); i_o->value.b=(x); i_o->tt=LUA_TBOOLEAN; }

#define setsvalue2n(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TSTRING; \
    luarc_addreftvalue(i_o); \
    checkliveness(G(L),i_o); }

#define setuvalue2n(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TUSERDATA; \
    luarc_addreftvalue(i_o); \
    checkliveness(G(L),i_o); }

#define setthvalue2n(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TTHREAD; \
    luarc_addreftvalue(i_o); \
    checkliveness(G(L),i_o); }

#define setclvalue2n(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TFUNCTION; \
    luarc_addreftvalue(i_o); \
    checkliveness(G(L),i_o); }

#define sethvalue2n(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TTABLE; \
    luarc_addreftvalue(i_o); \
    checkliveness(G(L),i_o); }

#define setptvalue2n(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TPROTO; \
    luarc_addreftvalue(i_o); \
    checkliveness(G(L),i_o); }

#if LUA_WIDESTRING
#define setwsvalue2n(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TWSTRING; \
    luarc_addreftvalue(i_o); \
    checkliveness(G(L),i_o); }
#endif /* LUA_WIDESTRING */

#else /* !LUA_REFCOUNT */

#define setnilvalue(obj) ((obj)->tt=LUA_TNIL)

#define setnvalue(obj,x) \
  { TValue *i_o=(obj); i_o->value.n=(x); i_o->tt=LUA_TNUMBER; }

#define setpvalue(obj,x) \
  { TValue *i_o=(obj); i_o->value.p=(x); i_o->tt=LUA_TLIGHTUSERDATA; }

#define setbvalue(obj,x) \
  { TValue *i_o=(obj); i_o->value.b=(x); i_o->tt=LUA_TBOOLEAN; }

#define setsvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TSTRING; \
    checkliveness(G(L),i_o); }

#define setuvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TUSERDATA; \
    checkliveness(G(L),i_o); }

#define setthvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TTHREAD; \
    checkliveness(G(L),i_o); }

#define setclvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TFUNCTION; \
    checkliveness(G(L),i_o); }

#define sethvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TTABLE; \
    checkliveness(G(L),i_o); }

#define setptvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TPROTO; \
    checkliveness(G(L),i_o); }




#define setobj(L,obj1,obj2) \
  { const TValue *o2=(obj2); TValue *o1=(obj1); \
    o1->value = o2->value; o1->tt=o2->tt; \
    checkliveness(G(L),o1); }

#if LUA_WIDESTRING
#define setwsvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TWSTRING; \
    checkliveness(G(L),i_o); }
#endif /* LUA_WIDESTRING */

#endif /* LUA_REFCOUNT */

/*
** different types of sets, according to destination
*/

/* from stack to (same) stack */
#define setobjs2s	setobj
/* to stack (not from same stack) */
#define setobj2s	setobj
#define setsvalue2s	setsvalue
#if LUA_WIDESTRING
#define setwsvalue2s	setwsvalue
#endif /* LUA_WIDESTRING */
#define sethvalue2s	sethvalue
#define setptvalue2s	setptvalue
/* from table to same table */
#define setobjt2t	setobj
/* to table */
#define setobj2t	setobj
/* to new object */
#if LUA_REFCOUNT
#define setnilvalue2n(L,obj) ((obj)->tt=LUA_TNIL)
#else
#define setobj2n	setobj
#define setsvalue2n	setsvalue
#if LUA_WIDESTRING
#define setwsvalue2n	setwsvalue
#endif /* LUA_WIDESTRING */
#endif /* LUA_REFCOUNT */

#define setttype(obj, tt) (ttype(obj) = (tt))


#define iscollectable(o)	(ttype(o) >= LUA_TSTRING)



typedef TValue *StkId;  /* index to stack elements */


/*
** String headers for string table
*/
typedef union TString {
  L_Umaxalign dummy;  /* ensures maximum alignment for strings */
  struct {
    CommonHeader;
    lu_byte reserved;
    unsigned int hash;
    size_t len;
  } tsv;
} TString;


#define getstr(ts)	cast(const char *, (ts) + 1)
#define svalue(o)       getstr(rawtsvalue(o))
#if LUA_WIDESTRING
#define getwstr(ts)	cast(const lua_WChar *, (ts) + 1)
#define wsvalue(o)      getwstr(rawtsvalue(o))
#endif /* LUA_WIDESTRING */



typedef union Udata {
  L_Umaxalign dummy;  /* ensures maximum alignment for `local' udata */
  struct {
    CommonHeader;
    struct Table *metatable;
    struct Table *env;
    size_t len;
  } uv;
} Udata;




/*
** Function Prototypes
*/
typedef struct Proto {
  CommonHeader;
  TValue *k;  /* constants used by the function */
  Instruction *code;
  struct Proto **p;  /* functions defined inside the function */
  int *lineinfo;  /* map from opcodes to source lines */
  struct LocVar *locvars;  /* information about local variables */
  TString **upvalues;  /* upvalue names */
  TString  *source;
  int sizeupvalues;
  int sizek;  /* size of `k' */
  int sizecode;
  int sizelineinfo;
  int sizep;  /* size of `p' */
  int sizelocvars;
  int linedefined;
  int lastlinedefined;
  GCObject *gclist;
  lu_byte nups;  /* number of upvalues */
  lu_byte numparams;
  lu_byte is_vararg;
  lu_byte maxstacksize;
} Proto;


/* masks for new-style vararg */
#define VARARG_HASARG		1
#define VARARG_ISVARARG		2
#define VARARG_NEEDSARG		4


typedef struct LocVar {
  TString *varname;
  int startpc;  /* first point where variable is active */
  int endpc;    /* first point where variable is dead */
} LocVar;



/*
** Upvalues
*/

typedef struct UpVal {
  CommonHeader;
  TValue *v;  /* points to stack or to its own value */
  union {
    TValue value;  /* the value (when closed) */
    struct {  /* double linked list (when open) */
      struct UpVal *prev;
      struct UpVal *next;
    } l;
  } u;
} UpVal;


/*
** Closures
*/

#define ClosureHeader \
	CommonHeader; lu_byte isC; lu_byte nupvalues; GCObject *gclist; \
	struct Table *env

typedef struct CClosure {
  ClosureHeader;
  lua_CFunction f;
  TValue upvalue[1];
} CClosure;


typedef struct LClosure {
  ClosureHeader;
  struct Proto *p;
  UpVal *upvals[1];
} LClosure;


typedef union Closure {
  CClosure c;
  LClosure l;
} Closure;


#define iscfunction(o)	(ttype(o) == LUA_TFUNCTION && clvalue(o)->c.isC)
#define isLfunction(o)	(ttype(o) == LUA_TFUNCTION && !clvalue(o)->c.isC)


/*
** Tables
*/

typedef union TKey {
  struct {
    TValuefields;
    struct Node *next;  /* for chaining */
  } nk;
  TValue tvk;
} TKey;


typedef struct Node {
  TValue i_val;
  TKey i_key;
} Node;


typedef struct Table {
  CommonHeader;
  lu_byte flags;  /* 1<<p means tagmethod(p) is not present */ 
  lu_byte lsizenode;  /* log2 of size of `node' array */
  struct Table *metatable;
  TValue *array;  /* array part */
  Node *node;
  Node *lastfree;  /* any free position is before this position */
  GCObject *gclist;
  int sizearray;  /* size of `array' array */
} Table;



/*
** `module' operation for hashing (size is always a power of 2)
*/
#define lmod(s,size) \
	(check_exp((size&(size-1))==0, (cast(int, (s) & ((size)-1)))))


#define twoto(x)	(1<<(x))
#define sizenode(t)	(twoto((t)->lsizenode))


#define luaO_nilobject		(&luaO_nilobject_)

LUAI_DATA const TValue luaO_nilobject_;

#define ceillog2(x)	(luaO_log2((x)-1) + 1)

LUAI_FUNC int luaO_log2 (unsigned int x);
LUAI_FUNC int luaO_int2fb (unsigned int x);
LUAI_FUNC int luaO_fb2int (int x);
LUAI_FUNC int luaO_rawequalObj (const TValue *t1, const TValue *t2);
LUAI_FUNC int luaO_str2d (const char *s, lua_Number *result);
#if LUA_WIDESTRING
LUAI_FUNC int luaO_wstr2d (const lua_WChar *s, lua_Number *result);
#endif /* LUA_WIDESTRING */
LUAI_FUNC const char *luaO_pushvfstring (lua_State *L, const char *fmt,
                                                       va_list argp);
LUAI_FUNC const char *luaO_pushfstring (lua_State *L, const char *fmt, ...);
LUAI_FUNC void luaO_chunkid (char *out, const char *source, size_t len);

NAMESPACE_LUA_END

#endif

