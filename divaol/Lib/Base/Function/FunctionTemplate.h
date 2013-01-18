/****************************************************
 *  FunctionTemplate.h
 *
 *  Created by tempbuffer on 9/17/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  function_tempate
 ****************************************************/

#include "Common.h"
#include "FunctionBase.h"

namespace Base {

#ifdef BASE_FUNCTION_NUM_ARGS

	namespace function_detial {
#define BASE_FUNCTION BASE_JOIN(Function, BASE_FUNCTION_NUM_ARGS)
#define BASE_INVOKER BASE_JOIN(invoker, BASE_FUNCTION_NUM_ARGS)
#define BASE_OBJ_INVOKER BASE_JOIN(obj_invoker, BASE_FUNCTION_NUM_ARGS)
#define BASE_GET_INVOKER BASE_JOIN(get_function_invoker, BASE_FUNCTION_NUM_ARGS)
#define BASE_GET_OBJ_INVOKER BASE_JOIN(get_function_obj_invoker, BASE_FUNCTION_NUM_ARGS)
#define BASE_FUNCTOR_RELEASE BASE_JOIN(functor_release, BASE_FUNCTION_NUM_ARGS)
#define BASE_GET_FUNCTOR_RELEASE BASE_JOIN(get_functor_release, BASE_FUNCTION_NUM_ARGS)

#if BASE_FUNCTION_NUM_ARGS == 0
#define BASE_FUNCTION_COMMA
#else
#define BASE_FUNCTION_COMMA ,
#endif

		//**********************
		// MemFunc
		//**********************
		template<typename R BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_PARAM, typename obj>
		class MemFunc<R(BASE_FUNCTION_TEMPLATE_ARGS), obj> {
		public:
			typedef obj ObjType;
			typedef R ReturnType;

			typedef R (obj::*MemFunction)(BASE_FUNCTION_PARAMS);

			explicit MemFunc(MemFunction func):mFunc(func) {}

			inline R invoke(obj *left BASE_FUNCTION_COMMA BASE_FUNCTION_PARAMS) {
				return (left->*mFunc)(BASE_FUNCTION_ARGS);
			}

			inline R operator()(obj *left BASE_FUNCTION_COMMA BASE_FUNCTION_PARAMS) {
				return (left->*mFunc)(BASE_FUNCTION_ARGS);
			}

			operator bool() {
				return mFunc!=NULL;
			}

		private:
			MemFunction mFunc;
		};	

		template<typename R
			BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_PARAM,
			typename obj>
			inline MemFunc<R(BASE_FUNCTION_TEMPLATE_ARGS), obj> MakeMemFunc(R (obj::*memFunc)(BASE_FUNCTION_PARAMS)) {
				return MemFunc<R(BASE_FUNCTION_TEMPLATE_ARGS), obj>(memFunc);
		}

		//**********************
		// Invoker
		//**********************
		template<typename Functor, typename R BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_PARAM>
		struct BASE_INVOKER {
			static R invoke(any_ptr ptr BASE_FUNCTION_COMMA BASE_FUNCTION_PARAMS) {
				Functor f = reinterpret_cast<Functor>(ptr.func_ptr);
				return f(BASE_FUNCTION_ARGS);
			}
		};

		template<typename Functor, typename R BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_PARAM>
		struct BASE_OBJ_INVOKER {
			static R invoke(any_ptr ptr BASE_FUNCTION_COMMA BASE_FUNCTION_PARAMS) {
				Functor *f = reinterpret_cast<Functor*>(ptr.obj_ptr);
				return (*f)(BASE_FUNCTION_ARGS);
			}
		};

		// now the assembler can support void type to return, so we do not need a specific template

		/*template<typename Functor BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_PARAM>
		struct BASE_INVOKER<Functor, void BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_ARGS> {
			static void invoke(any_ptr ptr BASE_FUNCTION_COMMA BASE_FUNCTION_PARAMS) {
				Functor f = reinterpret_cast<Functor>(ptr.func_ptr);
				f(BASE_FUNCTION_ARGS);
			}
		};*/

		/*template<typename Functor BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_PARAM>
		struct BASE_OBJ_INVOKER<Functor, void BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_ARGS> {
			static void invoke(any_ptr ptr BASE_FUNCTION_COMMA BASE_FUNCTION_PARAMS) {
				Functor f = reinterpret_cast<Functor*>(ptr.obj_ptr);
				(*f)(BASE_FUNCTION_ARGS);
			}
		};*/

		template<typename Functor, typename R BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_PARAM>
		struct BASE_FUNCTOR_RELEASE {
			static void release(any_ptr ptr) {
				Functor* f = reinterpret_cast<Functor*>(ptr.obj_ptr);
				delete f;
			}
		};

		template<typename Functor, typename R BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_PARAM>
		struct BASE_GET_INVOKER {
			typedef BASE_INVOKER<Functor, R BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_ARGS> type;
		};

		template<typename Functor, typename R BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_PARAM>
		struct BASE_GET_OBJ_INVOKER {
			typedef BASE_OBJ_INVOKER<Functor, R BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_ARGS> type;
		};

		template<typename Functor, typename R BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_PARAM>
		struct BASE_GET_FUNCTOR_RELEASE {
			typedef BASE_FUNCTOR_RELEASE<Functor, R BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_ARGS> type;
		};

		//**********************
		// FunctionN Class
		//**********************
		template<typename R BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_PARAM>
		class BASE_FUNCTION {
		public:
			typedef BASE_FUNCTION<R BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_PARAM> self_type;
			typedef R result_type;
			typedef result_type (*invoker_type)(any_ptr BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_PARAM);
			typedef void (*releaser_type)(any_ptr);
			static const int ARG_NUM = BASE_FUNCTION_NUM_ARGS;

			template<typename Functor>
			BASE_FUNCTION(const Functor& functor):
			mInvoker(0),mReleaser(0),mRefCount(NULL) {
				assign(functor);
			}

			BASE_FUNCTION():
			mInvoker(0),mReleaser(0),mRefCount(NULL) {
			}

			BASE_FUNCTION(const self_type &rhs) {
				assign(rhs);
			}

			virtual ~BASE_FUNCTION() {
				clear();
			}

			void setArg(BASE_FUNCTION_PARAMS) {
				#define BASE_ARGUMENT_MERGE(n,d) d##[##n##]=a##n;
				#define BASE_ENUM_ARGUMENT_ASSIGNMENT(n) BASE_REPEAT_N(n,BASE_ARGUMENT_MERGE,mArg)
				mArg = AnyArray(BASE_FUNCTION_NUM_ARGS);
				//for(int i = 0; i < BASE_FUNCTION_NUM_ARGS; i++)
				//	mArguments[i] = BASE_JOIN(a,);
				BASE_ENUM_ARGUMENT_ASSIGNMENT(BASE_FUNCTION_NUM_ARGS)
				//BASE_ARGUMENT_MERGE(0,mArguments)

				#undef BASE_ARGUMENT_MERGE
				#undef BASE_ENUM_ARGUMENT_ASSIGNMENT
			}

			void clear() {
				if(mInvoker && mRefCount) {
					--*mRefCount;
					if(*mRefCount <= 0)
					{
						delete mRefCount;

						if(mReleaser)
							mReleaser(mPtr);
					}
				}
				mRefCount = NULL;
				mReleaser = NULL;
				mInvoker = NULL;
				mPtr.value = 0;
			}

			template<typename Functor>
			void assign(Functor f) {
				clear();

				typedef typename get_function_tag<Functor>::type tag;
				assign(f, tag());
			}

			inline R operator()(BASE_FUNCTION_PARAMS) const {
				invoke(BASE_FUNCTION_ARGS);
			}

			inline R invoke(BASE_FUNCTION_PARAMS) const {
				assert(mInvoker);

				return mInvoker(mPtr BASE_FUNCTION_COMMA BASE_FUNCTION_ARGS);
			}

			inline R invokeWithArg() const {
				assert(mInvoker);

#define BASE_ARGUMENT_EXTRACT(n,d) BASE_COMMA_IF(n) (d##[##n##].as<T##n>())
#define BASE_ENUM_ARGUMENT_EXTRACT(n) BASE_REPEAT_N(n,BASE_ARGUMENT_EXTRACT,mArg)
				return mInvoker(mPtr BASE_FUNCTION_COMMA BASE_ENUM_ARGUMENT_EXTRACT(BASE_FUNCTION_NUM_ARGS));
#undef BASE_ENUM_ARGUMENT_EXTRACT
#undef BASE_ARGUMENT_EXTRACT
			}

			operator bool() const {
				return mInvoker != NULL;
			}

			bool empty() const {
				return mInvoker != NULL;
			}

			self_type& operator=(const self_type &rhs) {
				assign(rhs);
				return *this;
			}

			template<typename F>
			bool operator==(F f) {
				typedef typename get_function_tag<F>::type tag;
				return equalTo(f, tag());
			}

			template<typename F>
			bool equalTo(F f, function_ptr_tag tag) {
				return mPtr.func_ptr == f;
			}

			template<typename F>
			bool equalTo(F f, function_obj_tag tag) {
				return *((F*)mPtr.obj_ptr) == f;
			}

			template<typename F>
			bool equalTo(F f, member_ptr_tag tag) {
				return this->equalTo(MakeMemFunc(f), function_obj_tag());
			}

		private:
			void assign(const self_type &rhs) {
				if(this != &rhs) {
					this->mInvoker = rhs.mInvoker;
					this->mReleaser = rhs.mReleaser;
					this->mRefCount = rhs.mRefCount;
					this->mPtr = rhs.mPtr;
					this->mArg = rhs.mArg;

					if(mRefCount)
						++*mRefCount;
				}
			}

			template<typename Functor>
			void assign(Functor f, function_ptr_tag tag) {
				if(f) {
					typedef typename BASE_GET_INVOKER<Functor, R BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_ARGS>::type invoker_type;

					mInvoker = &invoker_type::invoke;
					mPtr = make_any_pointer((void (*)())f);
				}
			}

			template<typename Functor>
			void assign(Functor f, function_obj_tag tag) {
				typedef typename BASE_GET_OBJ_INVOKER<Functor, R BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_ARGS>::type invoker_type;
				typedef typename BASE_GET_FUNCTOR_RELEASE<Functor, R BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_ARGS>::type releaser_type;
					
				mInvoker = &invoker_type::invoke;
				mReleaser = &releaser_type::release;

				mPtr = make_any_pointer(new Functor(f));
				mRefCount = new int;
				*mRefCount = 1;
			}

			template<typename Functor>
			void assign(Functor f, member_ptr_tag tag) {
				BASE_THROW_EXCEPTION("Can not use a single member function to create function, since you can't invoke it");
				//assign(MakeMemFunc(f), function_obj_tag());
			}

			template<typename Functor>
			void assign(Functor f, function_obj_ref_tag tag) {
				// FuncObj& will tag to obj_tag, what's the reason?
			}

		protected:
			any_ptr mPtr;
			AnyArray mArg;
			invoker_type mInvoker;
			releaser_type mReleaser;
			int* mRefCount;
		};

		//**********************
		// Bind
		//**********************
		template<typename obj, typename R BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_PARAM>
		class Binded<obj, R(BASE_FUNCTION_TEMPLATE_ARGS)>
		{
			typedef obj ObjType;
			typedef MemFunc<R(BASE_FUNCTION_TEMPLATE_ARGS), obj> MemFuncType;
			typedef typename MemFuncType::MemFunction MemFuncPtr;
			typedef Binded<obj, R(BASE_FUNCTION_TEMPLATE_ARGS)> self_type;
		public:
			Binded(ObjType *object, MemFuncPtr memfunc):
			  mObj(object),mMemFunc(memfunc) {}

			operator bool () {
				return mObj && mMemFunc;
			}

			bool operator == (const self_type &rhs) {
				return mObj==rhs.mObj&&mMemFunc==rhs.mMemFunc;
			}

			inline R operator()(BASE_FUNCTION_PARAMS) {
				return mMemFunc.invoke(mObj BASE_FUNCTION_COMMA BASE_FUNCTION_ARGS);
			}
		private:
			ObjType *mObj;
			MemFuncType mMemFunc;
		};

		template<typename obj, typename R BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_PARAM>
		Binded<obj, R(BASE_FUNCTION_TEMPLATE_ARGS)> Bind(obj *object, R (obj::*mem_func)(BASE_FUNCTION_PARAMS)) {
			return Binded<obj, R(BASE_FUNCTION_TEMPLATE_ARGS)>(object, mem_func);
		}

		template<typename R BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_PARAM>
		class Function<R(BASE_FUNCTION_TEMPLATE_ARGS)> : public BASE_FUNCTION<R BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_ARGS>
		{
			typedef typename BASE_FUNCTION<R BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_ARGS> base_type;
			typedef typename Function<R(BASE_FUNCTION_TEMPLATE_ARGS)> self_type;
		public:
			Function():
				base_type() {}

			Function(const self_type &rhs) {
				*this = rhs;
			}

			template<typename Functor>
			Function(Functor f):
				base_type(f) {}

			template<typename Functor, typename obj>
			Function(Functor f, obj *object) {
				assign(Base::function_detial::Bind(object, f));
			}

			self_type& operator=(const self_type &rhs) {
				base_type::operator=(rhs);
				return *this;
			}

			bool operator==(const self_type &rhs) {
				return mPtr.value==rhs.mPtr.value;
			}
		};

		template<typename R BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_PARAM>
		Function<R(BASE_FUNCTION_TEMPLATE_ARGS)> MakeFunction(R (*func)(BASE_FUNCTION_PARAMS)) {
			return Function<R(BASE_FUNCTION_TEMPLATE_ARGS)>(func);
		}
		template<typename R BASE_FUNCTION_COMMA BASE_FUNCTION_TEMPLATE_PARAM,
				typename obj>
		Function<R(BASE_FUNCTION_TEMPLATE_ARGS)> MakeFunction(R (obj::*mem_func)(BASE_FUNCTION_PARAMS), obj *object) {
			return Function<R(BASE_FUNCTION_TEMPLATE_ARGS)>(mem_func,object);
		}

#undef BASE_FUNCTION
#undef BASE_INVOKER
#undef BASE_OBJ_INVOKER
#undef BASE_GET_INVOKER
#undef BASE_GET_OBJ_INVOKER
#undef BASE_FUNCTOR_RELEASE
#undef BASE_GET_FUNCTOR_RELEASE
#undef BASE_FUNCTION_COMMA
	}

#endif
}