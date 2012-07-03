//
//  SoraFactory.h
//  Sora
//
//  Created by Robert Bu on 7/31/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraFactory_h
#define Sora_SoraFactory_h

#include "SoraPlatform.h"
#include "SoraPreDeclare.h"
#include "SoraAutoPtr.h"
#include "SoraStringConv.h"
#include "function/SoraFunction.h"

#include "SoraUncopyable.h"


namespace sora {
    
    template<typename base, typename product>
    base* SoraFactoryCtorDelegate0() {
        return new product();
    }
    
    template<typename base, typename product, typename A0>
    base* SoraFactoryCtorDelegate1(A0 a0) {
        return new product(a0);
    }
    
    template<typename base, typename product, typename A0, typename A1>
    base* SoraFactoryCtorDelegate2(A0 a0, A1 a1) {
        return new product(a0, a1);
    }
    
    template<typename T, typename SIG>
    struct SoraAbstractFactory;
    
    template<typename T>
    struct SoraAbstractFactory<T, T*()> {
    public:
        typedef SoraFunction<T*()> CreatorFn;
        typedef std::map<std::string, CreatorFn> CreatorFnMap;
        
        typedef SoraAbstractFactory<T, T*()> SelfType;
        static SelfType* Instance() {
            static SelfType instance;
            return &instance;
        }
        
        T* createInstance(const std::string& name) {
            typename CreatorFnMap::iterator it = mCreators.find(name);
            if(it != mCreators.end()) {
                return it->second();
            } else
                THROW_SORA_EXCEPTION(NotFoundException, "No required product founded");
        }
        
        template<typename FN>
        void reg(const std::string& name, FN fn) {
            mCreators[name] = fn;
        }
        
        template<typename product>
        void reg_ctor(const std::string& name) {
            mCreators[name] = CreatorFn(SoraFactoryCtorDelegate0<T, product>);
        }
        
    protected:
        CreatorFnMap mCreators;
    };
    
    template<typename T, typename A0>
    class SoraAbstractFactory<T, T*(A0)> {
    public:
        typedef SoraFunction<T*(A0)> CreatorFn;
        typedef std::map<std::string, CreatorFn> CreatorFnMap;
        
        typedef SoraAbstractFactory<T, T*(A0)> SelfType;
        static SelfType* Instance() {
            static SelfType instance;
            return &instance;
        }
        
        T* createInstance(const std::string& name, A0 a0) {
            typename CreatorFnMap::iterator it = mCreators.find(name);
            if(it != mCreators.end()) {
                return it->second(a0);
            } else
                THROW_SORA_EXCEPTION(NotFoundException, "No required product founded");
        }
        
        template<typename FN>
        void reg(const std::string& name, FN fn) {
            mCreators[name] = fn;
        }
        
        template<typename product>
        void reg_ctor(const std::string& name) {
            mCreators[name] = CreatorFn(SoraFactoryCtorDelegate1<T, product, A0>);
        }
        
    protected:
        CreatorFnMap mCreators;
    };
    
    template<typename T, typename A0, typename A1>
    class SoraAbstractFactory<T, T*(A0, A1)> {
    public:
        typedef SoraFunction<T*(A0, A1)> CreatorFn;
        typedef std::map<std::string, CreatorFn> CreatorFnMap;
        
        typedef SoraAbstractFactory<T, T*(A0, A1)> SelfType;
        static SelfType* Instance() {
            static SelfType instance;
            return &instance;
        }
        
        T* createInstance(const std::string& name, A0 a0, A1 a1) {
            typename CreatorFnMap::iterator it = mCreators.find(name);
            if(it != mCreators.end()) {
                return it->second(a0, a1);
            } else
                THROW_SORA_EXCEPTION(NotFoundException, "No required product founded");
        }
        
        template<typename FN>
        void reg(const std::string& name, FN fn) {
            mCreators[name] = fn;
        }
        
        template<typename product>
        void reg_ctor(const std::string& name) {
            mCreators[name] = CreatorFn(SoraFactoryCtorDelegate2<T, product, A0, A1>);
        }
        
    protected:
        CreatorFnMap mCreators;
    };

} // namespace sora



#endif
