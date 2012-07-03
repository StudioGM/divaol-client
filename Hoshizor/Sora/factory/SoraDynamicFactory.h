//
//  SoraDynamicFactory.h
//  Sora
//
//  Created by Robert Bu on 8/2/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraDynamicFactory_h
#define Sora_SoraDynamicFactory_h

#include "SoraPlatform.h"
#include "SoraException.h"
#include "SoraCommon.h"
#include "SoraUncopyable.h"
#include "SoraLogger.h"

#include <map>

namespace sora {
    
    template<class BASE>
    class SORA_API SoraAbstractInstantiator: SoraUncopyable {
    public:
        virtual ~SoraAbstractInstantiator() {}
        
        virtual BASE* createInstance() const = 0;
    };
    
    template<class BASE, class C>
    class SORA_API SoraInstantiator: public SoraAbstractInstantiator<BASE> {
    public:
        BASE* createInstance() const {
            return new C;
        }
    };
    
    /**
     * A helper that manages allow users to create a class instance by it's name
     * Users must register class before instantiate it
     * And the Class must have a default ctor to be constructed
     * Unlike SoraRTTI, no any constructor support
     **/    
    template<class BASE>
    class SORA_API SoraDynamicFactory: SoraUncopyable {
    public:
        typedef SoraAbstractInstantiator<BASE> AbstractFactory;
        
        ~SoraDynamicFactory() {
            for_each(mFactory.begin(), mFactory.end(), DeleteSTLPairPtr());
        }
        
        template<class C>
        void registerClass(const std::string& name) {
            registerClass<C>(name, new SoraInstantiator<BASE, C>());
        }
        
        template<class C>
        void registerClass(const std::string& name, AbstractFactory* instantiator) {
            typename FactoryMap::iterator itFactory = mFactory.find(name);
            
            if(itFactory == mFactory.end()) {
                mFactory[name] = instantiator;
            } else 
                THROW_SORA_EXCEPTION(ExistsException, vamssg("Class %s already exists", name.c_str()));
        }
        
        void unregisterClass(const std::string& name) {
            typename FactoryMap::iterator itFactory = mFactory.find(name);
        
            if(itFactory != mFactory.end()) {
                delete itFactory->second;
                mFactory.erase(itFactory);
            }
        }
        
        bool isClass(const std::string& name) {
            typename FactoryMap::iterator itFactory = mFactory.find(name);
            return itFactory != mFactory.end();
        }
        
        BASE* createInstance(const std::string& name) {
            typename FactoryMap::iterator itFactory = mFactory.find(name);
            if(itFactory != mFactory.end()) {
                return itFactory->second->createInstance();
            } else {
                std::ostringstream msg;
                msg << "Class with "<<name<<" Not found in Factory";
                THROW_SORA_EXCEPTION(NotFoundException, msg.str());
            }
            return NULL;
        }
        
    private:
        typedef std::map<std::string, AbstractFactory*> FactoryMap;
        FactoryMap mFactory;
    };
    
    
} // namespace sora



#endif
