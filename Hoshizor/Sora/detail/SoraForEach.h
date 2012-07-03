//
//  SoraForEach.h
//  Sora
//
//  Created by Ruiwei Bu on 10/27/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraForEach_h
#define Sora_SoraForEach_h

namespace sora {
    
    namespace for_each {
        
        // any type holder
        struct auto_any_base { 
            operator bool() const {
                return false;
            }
        };
        
        template<typename T>
        struct auto_any: public auto_any_base {
            auto_any(const T& any): item(any) { }
            
            mutable T item;
        };
        
        
        // detect and hold type of an expr without eval it
        template<typename T> struct type2type {
            typedef typename T::iterator iterator;
            typedef typename T::const_iterator const_iterator;
            
            typedef typename T::reference reference;
            typedef typename T::const_reference const_reference;
        };
        
        template<typename T>
        type2type<T> encode_type(const T& t) {
            return type2type<T>();
        }         
        struct any_type {
            template<typename T>
            operator type2type<T>() const {
                return type2type<T>();
            }
        };
        
        // ? operator type trick
        // the macro actually returns type2type<TYPE_OF_CONTAINER>
        // since any_type can be cast to type2type
#define _for_each_EncodeTypeOf(container) (true ? sora::for_each::any_type(): sora::for_each::encode_type(container))
        
        // simple value holder for holding a container
        // with the _for_each_EvalRValue macro, the struct evaluates t is a rvalue or not
        // 
        //      (true ? sora::for_each::make_holder(expr, is_rvalue) : expr)
        // if T is a rvalue, then the type of expr is T, operator T() would be called and is_rvalue would be set to true
        // otherwise the type of expr is T&, operator T& is called
        template<typename T>
        struct value_holder {
            value_holder(T&t, bool& b):
            value(t),
            is_rvalue(b) { }
            
            T& value;
            bool& is_rvalue;
            
            operator T() {
                this->is_rvalue = true;
                return this->value;
            }
            
            operator T&() const {
                return this->value;
            }
        };
        
        // simple ref counted any holder
        struct simple_any {
            simple_any() {
                parent = 0;
                refCount = new int(1);
            }
            template<typename T>
            simple_any(const T& t) {
                parent = new holder<T>(t);
                refCount = new int(1);
            }
            
            simple_any(const simple_any& rhs) {
                parent = 0;
                refCount = new int(1);
                if(this != &rhs) {
                    release();
                    
                    refCount = rhs.refCount;
                    ++*refCount;
                    this->parent = rhs.parent;
                }
            }
            
            void release() {
                (*refCount)--;
                if(*refCount == 0) {
                    if(parent)
                        delete parent; 
                    delete refCount;
                    
                    parent = 0;
                    refCount = 0;
                }
            }
            
            ~simple_any() {
                release();
            }
            
            template<typename T>
            T& get() const {
                holder<T>* child = static_cast<holder<T>* >(parent);
                return child->value;
            }
            
            struct holder_parent {};
            
            template<typename T>
            struct holder: holder_parent {
                holder(const T& t):
                value(t) {
                    
                }
                
                T value;
            };
            
            holder_parent* parent;
            int* refCount;
        };
        
        
        template<typename Container>
        value_holder<Container> make_holder(Container& t, bool& is_rvalue) {
            return value_holder<Container>(t, is_rvalue);
        }
        
        template<typename Container>
        value_holder<Container> make_holder(const Container& t, bool& is_rvalue) {
            return value_holder<Container>(const_cast<Container&>(t), is_rvalue);
        }
        
        // ? operator trick 
        // determines the expr returns a rvalue or not without actually evaluate it
#define _for_each_EvalRValue(expr, is_rvalue) (true ? sora::for_each::make_holder(expr, is_rvalue) : expr)
        
        
        template<typename Container>
        auto_any<simple_any> make_container(const Container& t, bool& is_rvalue) {
            // if container expr is a rvalue (evaluated by _for_each_EvalRValue macro)
            // copy and store it in simple_any
            // otherwise store it as a pointer in simple_any
            return is_rvalue ? simple_any(t) : simple_any(&t);
        }
        
        template<typename Container>
        auto_any<simple_any> make_container(Container& t, bool& is_rvalue) {
            return is_rvalue ? simple_any(t) : simple_any(&t);
        }
        
        
        // cast auto_any_baes to auto_any<TYPE>
        template<typename T>
        T& auto_any_cast(const auto_any_base& base) {
            const auto_any<T>& value = static_cast<const auto_any<T>&>(base);
            return value.item;
        }
        
        // begin iterator for any container
        template<typename Container>
        auto_any<typename type2type<Container>::iterator> any_cont_begin(const auto_any_base& holder, bool is_rvalue, type2type<Container>) {
            const simple_any& val = auto_any_cast<simple_any>(holder);
            
            // if the container expr returns a rvalue, what we stored is a copy of it
            // otherwise we stored a pointer to it
            Container& cont = is_rvalue ? val.get<Container>() : *val.get<Container*>();
            return cont.begin();
        }
        
        // end iterator for any container
        template<typename Container>
        auto_any<typename type2type<Container>::iterator> any_cont_end(const auto_any_base& holder, bool is_rvalue, type2type<Container>) {
            const simple_any& val = auto_any_cast<simple_any>(holder);
            
            // if the container expr returns a rvalue, what we stored is a copy of it
            // otherwise we stored a pointer to it
            Container& cont = is_rvalue ? val.get<Container>() : *val.get<Container*>();
            return cont.end();
        }
        
        // increment the iterator
        template<typename Container>
        void any_cont_next(const auto_any_base& iter, type2type<Container>) {
            typedef typename type2type<Container>::iterator iter_type;
            ++auto_any_cast<iter_type>(iter);
        }
        
        // see if the iteration is ended
        template<typename Container>
        bool any_cont_done(const auto_any_base& iter, const auto_any_base& end, type2type<Container>) {
            typedef typename type2type<Container>::iterator iter_type;
            return auto_any_cast<iter_type>(iter) == auto_any_cast<iter_type>(end);
        }
        
        // dereference the iterator to get the real value
        template<typename Container>
        typename type2type<Container>::reference auto_any_deref(const auto_any_base& iter, type2type<Container>) {
            typedef typename type2type<Container>::iterator iter_type;
            return *auto_any_cast<iter_type>(iter);
        }
        
#define SORA_FOR_EACH(item, container) \
    if(bool is_rvalue = false) {} else \
    if(const sora::for_each::auto_any_base& c = sora::for_each::make_container(_for_each_EvalRValue(container, is_rvalue), is_rvalue)) {} else \
    if(const sora::for_each::auto_any_base& b = sora::for_each::any_cont_begin(c, is_rvalue, _for_each_EncodeTypeOf(container))) {} else \
    if(const sora::for_each::auto_any_base& e = sora::for_each::any_cont_end(c, is_rvalue, _for_each_EncodeTypeOf(container))) {} else \
    for(bool more=true; more && !sora::for_each::any_cont_done(b, e, _for_each_EncodeTypeOf(container)); sora::for_each::any_cont_next(b, _for_each_EncodeTypeOf(container))) \
    if((more = false)) {} else \
        for(item=sora::for_each::auto_any_deref(b, _for_each_EncodeTypeOf(container)); !more; more=true)
        
    }
    
} // namespace sora


#endif
