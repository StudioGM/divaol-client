//
//  SoraFunctionTemplate.h
//  zeromq_client
//
//  Created by Ruiwei Bu on 8/16/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

// this is a template header replys on macro definitions 
// must not have multiple-inclusion protection

#include "SoraPreprocessor.h"
#include "SoraFunctionBase.h"
#include "SoraFunctionFoward.h"

#include <functional>
#include <cassert>

namespace sora {
    
#ifdef SORA_FUNCTION_NUM_ARGS

    namespace detail {        
        
#define SORA_FUNCTION_TEMPLATE_PARAM SORA_ENUM_PARAMS(SORA_FUNCTION_NUM_ARGS, typename T)
        
#define SORA_FUNCTION_TEMPLATE_ARGS SORA_ENUM_PARAMS(SORA_FUNCTION_NUM_ARGS, T)

#define SORA_FUNCTION_PARAM(n, d) SORA_CAT(T, n) SORA_CAT(a, n)
#define SORA_FUNCTION_PARAMS SORA_ENUM_N(SORA_FUNCTION_NUM_ARGS, SORA_FUNCTION_PARAM, SORA_EMPTY)
        
#define SORA_FUNCTION_ARGS SORA_ENUM_PARAMS(SORA_FUNCTION_NUM_ARGS, a)
    
#define SORA_INVOKER SORA_JOIN(invoker, SORA_FUNCTION_NUM_ARGS)
#define SORA_OBJ_INVOKER SORA_JOIN(obj_invoker, SORA_FUNCTION_NUM_ARGS)
        
#define SORA_GET_INVOKER SORA_JOIN(get_function_invoker, SORA_FUNCTION_NUM_ARGS)
#define SORA_GET_OBJ_INVOKER SORA_JOIN(get_function_obj_invoker, SORA_FUNCTION_NUM_ARGS)
        
#define SORA_FUNCTION SORA_JOIN(function, SORA_FUNCTION_NUM_ARGS)
        
#define SORA_FUNCTOR_RELEASE SORA_JOIN(functor_release, SORA_FUNCTION_NUM_ARGS)
#define SORA_GET_FUNCTOR_RELEASE SORA_JOIN(get_functor_release, SORA_FUNCTION_NUM_ARGS)
        
#if SORA_FUNCTION_NUM_ARGS == 0
#define SORA_FUNCTION_COMMA
#else
#define SORA_FUNCTION_COMMA ,
#endif
        
        template<typename FunctionPtr, typename R SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_PARAM>
        struct SORA_INVOKER {
            static R invoke(any_ptr ptr SORA_FUNCTION_COMMA SORA_FUNCTION_PARAMS) {
                FunctionPtr f = reinterpret_cast<FunctionPtr>(ptr.func_ptr);
                return f(SORA_FUNCTION_ARGS);
            }
        };
        
        template<typename FunctionObj, typename R SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_PARAM>
        struct SORA_FUNCTOR_RELEASE {
            static void release(any_ptr ptr) {
                FunctionObj* f = reinterpret_cast<FunctionObj*>(ptr.obj_ptr);
                delete f;
            }
        };
        
        template<typename FunctionObj, typename R SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_PARAM>
        struct SORA_OBJ_INVOKER {
            static R invoke(any_ptr ptr SORA_FUNCTION_COMMA SORA_FUNCTION_PARAMS) {
                FunctionObj* f = reinterpret_cast<FunctionObj*>(ptr.obj_ptr);
                return (*f)(SORA_FUNCTION_ARGS);
            }
        };
        
        template<typename FunctionPtr SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_PARAM>
        struct SORA_INVOKER<FunctionPtr, void SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_ARGS> {
            static void invoke(any_ptr ptr SORA_FUNCTION_COMMA SORA_FUNCTION_PARAMS) {
                FunctionPtr f = reinterpret_cast<FunctionPtr>(ptr.func_ptr);
                f(SORA_FUNCTION_ARGS);
            }
        };
        
        template<typename FunctionObj SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_PARAM>
        struct SORA_OBJ_INVOKER<FunctionObj, void SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_ARGS> {
            static void invoke(any_ptr ptr SORA_FUNCTION_COMMA SORA_FUNCTION_PARAMS) {
                FunctionObj* f = reinterpret_cast<FunctionObj*>(ptr.obj_ptr);
                (*f)(SORA_FUNCTION_ARGS);
            }
        };
        
        template<typename FunctionPtr, typename R SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_PARAM>
        struct SORA_GET_INVOKER {
            typedef SORA_INVOKER<FunctionPtr, R SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_ARGS> type;
        };
        
        template<typename FunctionPtr, typename R SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_PARAM>
        struct SORA_GET_OBJ_INVOKER {
            typedef SORA_OBJ_INVOKER<FunctionPtr, R SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_ARGS> type;
        };
        
        template<typename FunctionObj, typename R SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_PARAM>
        struct SORA_GET_FUNCTOR_RELEASE {
            typedef SORA_FUNCTOR_RELEASE<FunctionObj, R SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_ARGS> type;
        };
        
        template<typename R SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_PARAM>
        class SORA_FUNCTION {
        public:
            typedef SORA_FUNCTION<R SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_ARGS> self_type;

            template<typename Functor>
            SORA_FUNCTION(const Functor& functor):
            mInvoker(0),
            mReleaser(0),
            mRefCount(NULL) {
                this->assign_to(functor);
            }
            
            SORA_FUNCTION():
            mInvoker(0),
            mReleaser(0),
            mRefCount(NULL) {
                
            }
            
            virtual ~SORA_FUNCTION() {
                clear();
            }
            
            void clear() {
                if(mInvoker && mRefCount) {
                    --*mRefCount;
                    if(*mRefCount <= 0) {
                        delete mRefCount;
                        
                        if(mReleaser)
                            mReleaser(mPtr);
                    }
                    mRefCount = NULL;
                    mReleaser = NULL;
                    mInvoker = NULL;
                }
            }
            
            template<typename Functor>
            void assign_to(Functor f) {
                clear();
                
                typedef typename get_function_tag<Functor>::type tag;
                this->assign_to(f, tag());
            }
            
            inline R operator()(SORA_FUNCTION_PARAMS) {
                assert(mInvoker);
                
                return mInvoker(mPtr SORA_FUNCTION_COMMA SORA_FUNCTION_ARGS);
            }
            
            operator bool() {
                return mInvoker != NULL;
            }
            
            typedef R (*invoker_type)(any_ptr SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_ARGS);
            typedef void (*releaser_type)(any_ptr);
      
        private:
            
            template<typename Functor>
            void assign_to(Functor f, function_ptr_tag tag) {
                if(f) {
                    typedef typename SORA_GET_INVOKER<Functor, R SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_ARGS>::type invoker_type;
                    
                    mInvoker = &invoker_type::invoke;
                    mPtr = make_any_pointer((void (*)())f);
                }
            }
            
            template<typename Functor>
            void assign_to(Functor f, function_obj_tag tag) {
                typedef typename SORA_GET_OBJ_INVOKER<Functor, R SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_ARGS>::type invoker_type;
                typedef typename SORA_GET_FUNCTOR_RELEASE<Functor, R SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_ARGS>::type releaser_type;

                
                mInvoker = &invoker_type::invoke;
                mReleaser = &releaser_type::release;
                
                mPtr = make_any_pointer(new Functor(f));
                mRefCount = new int;
                *mRefCount = 1;
            }
            
            template<typename Functor>
            void assign_to(Functor f, member_ptr_tag tag) {
                this->assign_to(MemFun(f)); 
            }
            
            template<typename Functor>
            void assign_to(Functor f, function_obj_ref_tag tag) {
                // ?
            }
            
        protected:
            any_ptr mPtr;
            invoker_type mInvoker;
            releaser_type mReleaser;
            int* mRefCount;
        };

        template<SORA_FUNCTION_TEMPLATE_PARAM>
        class SORA_FUNCTION<void SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_ARGS> {
        public:
            typedef SORA_FUNCTION<void SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_ARGS> self_type;
            
            template<typename Functor>
            SORA_FUNCTION(const Functor& functor):
            mInvoker(0),
            mReleaser(0),
            mRefCount(NULL) {
                this->assign_to(functor);
            }
            
            SORA_FUNCTION():
            mInvoker(0),
            mReleaser(0),
            mRefCount(NULL) {
                
            }
            
            virtual ~SORA_FUNCTION() {
                clear();
            }
            
            void clear() {
                if(mInvoker && mRefCount) {
                    --*mRefCount;
                    if(*mRefCount <= 0) {
                        delete mRefCount;
                        
                        if(mReleaser)
                            mReleaser(mPtr);
                    }
                    mRefCount = NULL;
                    mReleaser = NULL;
                    mInvoker = NULL;
                }
            }
            
            template<typename Functor>
            void assign_to(Functor f) {
                clear();
                
                typedef typename get_function_tag<Functor>::type tag;
                this->assign_to(f, tag());
            }
            
            inline void operator()(SORA_FUNCTION_PARAMS) {
                assert(mInvoker);
                
                mInvoker(mPtr SORA_FUNCTION_COMMA SORA_FUNCTION_ARGS);
            }
            
            operator bool() {
                return mInvoker != NULL;
            }
            
            typedef void (*invoker_type)(any_ptr SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_ARGS);
            typedef void (*releaser_type)(any_ptr);
            
        private:
            
            template<typename Functor>
            void assign_to(Functor f, function_ptr_tag tag) {
                if(f) {
                    typedef typename SORA_GET_INVOKER<Functor, void SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_ARGS>::type invoker_type;
                    
                    mInvoker = &invoker_type::invoke;
                    mPtr = make_any_pointer((void (*)())f);
                }
            }
            
            template<typename Functor>
            void assign_to(Functor f, function_obj_tag tag) {
                typedef typename SORA_GET_OBJ_INVOKER<Functor, void SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_ARGS>::type invoker_type;
                typedef typename SORA_GET_FUNCTOR_RELEASE<Functor, void SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_ARGS>::type releaser_type;
                
                
                mInvoker = &invoker_type::invoke;
                mReleaser = &releaser_type::release;
                
                mPtr = make_any_pointer(new Functor(f));
                mRefCount = new int(1);
            }
            
            template<typename Functor>
            void assign_to(Functor f, member_ptr_tag tag) {
                this->assign_to(MemFun(f)); 
            }
            
            template<typename Functor>
            void assign_to(Functor f, function_obj_ref_tag tag) {
                // ?
            }
            
        protected:
            any_ptr mPtr;
            invoker_type mInvoker;
            releaser_type mReleaser;
            int* mRefCount;
        };
        
        
    } // namespace detail
    
    
    template<typename R SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_PARAM>
    class SoraFunction<R(SORA_FUNCTION_TEMPLATE_ARGS)>: public detail::SORA_FUNCTION<R SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_ARGS>
#if SORA_FUNCTION_NUM_ARGS == 1
    , public std::unary_function<T0, R>
#elif SORA_FUNCTOR_NUM_ARGS == 2
    , public std::binary_function<T0, T1, R> 
#endif
    {
    public:
        typedef detail::SORA_FUNCTION<R SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_ARGS> base_type;
        typedef SoraFunction<R(SORA_FUNCTION_TEMPLATE_ARGS)> self_type;
#if SORA_FUNCTION_NUM_ARGS == 1
        typedef T0 argument_type;
#elif SORA_FUNCTOR_NUM_ARGS == 2
        typedef T0 first_argument_type;
        typedef T1 second_argument_type;
#endif
        
        template<typename Functor>
        SoraFunction(Functor f):
        base_type(f) {
            
        }
        
        SoraFunction():
        base_type() {
            
        }
        
        template<typename Functor>
        SoraFunction<R(SORA_FUNCTION_TEMPLATE_ARGS)>& operator=(Functor f) {
            base_type::assign_to(f);
            return *this;
        }
        
        
        self_type& operator=(const self_type& rhs) {
            if(this != &rhs) {
                base_type::clear();
                
                this->mPtr = rhs.mPtr;
                this->mInvoker = rhs.mInvoker;
                this->mReleaser = rhs.mReleaser;
                this->mRefCount = rhs.mRefCount;
                
                if(this->mRefCount != NULL) {
                    ++*this->mRefCount;
                }
            }
            return *this;
        }
        
        SoraFunction(const self_type& rhs) {
            if(this != &rhs) {
                base_type::clear();
                
                this->mPtr = rhs.mPtr;
                this->mInvoker = rhs.mInvoker;
                this->mReleaser = rhs.mReleaser;
                this->mRefCount = rhs.mRefCount;
                
                if(this->mRefCount != NULL) {
                    ++*this->mRefCount;
                }
            }
        }
    };
    
    template<SORA_FUNCTION_TEMPLATE_PARAM>
    class SoraFunction<void(SORA_FUNCTION_TEMPLATE_ARGS)>: public detail::SORA_FUNCTION<void SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_ARGS>
#if SORA_FUNCTION_NUM_ARGS == 1
    , public std::unary_function<T0, void>
#elif SORA_FUNCTOR_NUM_ARGS == 2
    , public std::binary_function<T0, T1, void> 
#endif
    {
    public:
        typedef detail::SORA_FUNCTION<void SORA_FUNCTION_COMMA SORA_FUNCTION_TEMPLATE_ARGS> base_type;
        typedef SoraFunction<void(SORA_FUNCTION_TEMPLATE_ARGS)> self_type;
#if SORA_FUNCTION_NUM_ARGS == 1
        typedef T0 argument_type;
#elif SORA_FUNCTOR_NUM_ARGS == 2
        typedef T0 first_argument_type;
        typedef T1 second_argument_type;
#endif
        
        template<typename Functor>
        SoraFunction(Functor f):
        base_type(f) {
            
        }
        
        SoraFunction():
        base_type() {
            
        }
        
        template<typename Functor>
        SoraFunction<void(SORA_FUNCTION_TEMPLATE_ARGS)>& operator=(Functor f) {
            base_type::assign_to(f);
            return *this;
        }
        
        
        self_type& operator=(const self_type& rhs) {
            if(this != &rhs) {
                base_type::clear();
                
                this->mPtr = rhs.mPtr;
                this->mInvoker = rhs.mInvoker;
                this->mReleaser = rhs.mReleaser;
                this->mRefCount = rhs.mRefCount;
                
                if(this->mRefCount != NULL) {
                    ++*this->mRefCount;
                }
            }
            return *this;
        }
        
        SoraFunction(const self_type& rhs) {
            if(this != &rhs) {
                base_type::clear();
                
                this->mPtr = rhs.mPtr;
                this->mInvoker = rhs.mInvoker;
                this->mReleaser = rhs.mReleaser;
                this->mRefCount = rhs.mRefCount;
                
                if(this->mRefCount != NULL) {
                    ++*this->mRefCount;
                }
            }
        }
    };
    
#undef SORA_FUNCTION_TEMPLATE_PARAM
#undef SORA_FUNCTION_TEMPLATE_ARGS
#undef SORA_FUNCTION_PARAMS
#undef SORA_FUNCTION_PARAM
#undef SORA_FUNCTION_ARGS
#undef SORA_INVOKER
#undef SORA_OBJ_INVOKER
#undef SORA_GET_INVOKER
#undef SORA_GET_OBJ_INVOKER
#undef SORA_FUNCTION_COMMA
#undef SORA_FUNCTION
#undef SORA_GET_FUNCTOR_RELEASE
#undef SORA_FUNCTOR_RELEASE
    
#endif // SORA_FUNCTION_NUM_ARGS

    
} // namespace sora 