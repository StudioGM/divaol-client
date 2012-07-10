//
//  factory.hpp
//  Guichan_Sora
//
//  Created by Robert Bu(darkfall) 8/26/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef GCN_FACTORY_HPP
#define GCN_FACTORY_HPP

#include <string>
#include <map>

#include "guichan/exception.hpp"

namespace gcn {
    
    /**
     * Abstract template class for all kinds of factories
     *
     * @author Robert Bu(darkfall)
     * @since Guichan for Hoshizora
     **/
    
    template<typename base, typename product>
    base* AbstractFactoryCtorDelegate0() {
        return new product();
    }
    
    template<typename BaseProduct>
    class AbstractFactory {
    public:
        typedef BaseProduct* ProductType;
        typedef ProductType (*CreatorFn)();
        typedef std::map<std::string, CreatorFn> CreatorFnMap;
        
        typedef AbstractFactory<BaseProduct> SelfType;
        
        static SelfType& Instance() {
            static SelfType instance;
            return instance;
        }
        
        /**
         * Create a product depend on it's name
         * Product must be registered
         * May throw a exception if product name not found
         **/
        ProductType create(const std::string& name) {
            typename CreatorFnMap::const_iterator it = mCreatorMap.find(name);
            if(it != mCreatorMap.end()) {
                return it->second();
            } else
                throw GCN_EXCEPTION("No prodcut with name" + name + "found in factory");
            return NULL;
        }
        
        /**
         * Register a custom product creator function
         * @param product name
         * @param creator function
         **/
        void reg(const std::string& name, CreatorFn fn) {
            mCreatorMap[name] = fn;
        }
        
        /**
         * Register a custom product using default ctor(new product())
         * Eg AbstractFactory::Instance().reg<My_Product>("myproduct");
         * @param product name
         **/
        template<typename Product>
        void reg(const std::string& name) {
            mCreatorMap[name] = AbstractFactoryCtorDelegate0<BaseProduct, Product>;
        }
        
    private:
        CreatorFnMap mCreatorMap;
    };
    
    
} // namespace gcn



#endif
