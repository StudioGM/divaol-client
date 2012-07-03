//
//  SoraPropertyHolder.h
//  SoraProperty
//
//  Created by Ruiwei Bu on 8/27/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef SoraProperty_SoraPropertyHolder_h
#define SoraProperty_SoraPropertyHolder_h

#include "SoraProperty.h"

#include <map>

namespace sora {
    
    class SORA_API SoraPropertyHolder {
    public:
        SoraPropertyHolder() { }
        ~SoraPropertyHolder() { 
            removeAllProperties();
        }
        
        bool hasProperty(const std::string& name);
        
        template<typename T>
        SoraProperty<T>* addProperty(const std::string& name, const T& defaultValue);
        void addProperty(SoraPropertyBase* property);
        
        template<typename T>
        SoraProperty<T> getProperty(const std::string& name) const;
        
        template<typename T>
        SoraProperty<T>* getPropertyPtr(const std::string& name) const;
        
        template<typename T>
        void setProperty(const std::string& name, const T& val);
        
        template<typename T>
        T getProperty(const std::string& name, T defaultValue) const;
        
        SoraPropertyBase* getPropertyBase(const std::string& name) const;
        
        SoraPropertyBase* removeProperty(const std::string& name, bool release=false);
        void removeAllProperties();
        
        typedef sora_hash_map<std::string, SoraPropertyBase*> PropertyMap;
        const PropertyMap& getProperties() const;
        
        size_t getPropertySize() const;
        
    protected:
        PropertyMap mProperties;
    };
    
    inline bool SoraPropertyHolder::hasProperty(const std::string& name) {
        PropertyMap::iterator it = mProperties.find(name);
        return it != mProperties.end();
    }
    
    // cast using type id
    template<typename T>
    inline SoraProperty<T>* PropertyCast(SoraPropertyBase* ptr) {
        if(ptr->getTypeId() == SoraTypeSerializer::getTypeId<T>())
            return static_cast<SoraProperty<T>*>(ptr);
        return NULL;
    }
    
    template<typename T>
    inline SoraProperty<T>* SoraPropertyHolder::addProperty(const std::string& name, const T& defaultValue) {
        PropertyMap::iterator it = mProperties.find(name);
        if(it != mProperties.end()) {
            SoraProperty<T>* prop = PropertyCast<T>(it->second);
            if(prop)
                return prop;
        }
        
        SoraProperty<T>* prop = new SoraProperty<T>(name);
        prop->set(defaultValue);
        mProperties.insert(std::make_pair(name, prop));
        return prop;
    }
    
    inline void SoraPropertyHolder::addProperty(SoraPropertyBase* prop) {
        PropertyMap::iterator it = mProperties.find(prop->getName());
        if(it == mProperties.end()) {
            mProperties.insert(std::make_pair(prop->getName(), prop));
        }
    }
    
    template<typename T>
    inline SoraProperty<T> SoraPropertyHolder::getProperty(const std::string& name) const {
        PropertyMap::const_iterator it = mProperties.find(name);
        if(it != mProperties.end()) {
            SoraProperty<T>* prop = PropertyCast<T>(it->second);
            if(prop) {
                return *prop;
            } else {
                SoraProperty<T> tprop(name);
                tprop.setFromString(it->second->toString());
                return tprop;
            }
        }
        return SoraProperty<T>();
    }
    
    template<typename T>
    T SoraPropertyHolder::getProperty(const std::string& name, T defaultValue) const {
        PropertyMap::const_iterator it = mProperties.find(name);
        if(it != mProperties.end()) {
            SoraProperty<T>* prop = PropertyCast<T>(it->second);
            if(prop) {
                return prop->get();
            } 
        }
        return defaultValue;
    }
    
    inline SoraPropertyBase* SoraPropertyHolder::getPropertyBase(const std::string& name) const {
        PropertyMap::const_iterator it = mProperties.find(name);
        if(it != mProperties.end()) {
            return it->second;
        }
        return NULL;
    }
    
    inline SoraPropertyBase* SoraPropertyHolder::removeProperty(const std::string& name, bool release) {
        PropertyMap::iterator it = mProperties.find(name);
        if(it != mProperties.end()) {
            SoraPropertyBase* prop = it->second;
            mProperties.erase(it);
            
            if(release) {
                delete prop;
                prop = 0;
            }
            return prop;
        }
        return 0;
    }
    
    template<typename T>
    SoraProperty<T>* SoraPropertyHolder::getPropertyPtr(const std::string& name) const {
        PropertyMap::const_iterator it = mProperties.find(name);
        if(it != mProperties.end()) {
            SoraProperty<T>* prop = PropertyCast<T>(it->second);
            if(prop)
                return prop;
        }
        return NULL;
    }
    
    template<typename T>
    void SoraPropertyHolder::setProperty(const std::string& name, const T& val) {
        PropertyMap::iterator it = mProperties.find(name);
        if(it != mProperties.end()) {
            SoraProperty<T>* prop = PropertyCast<T>(it->second);
            if(prop)
                prop->set(val);
            else {
                // force set
                // should we allow this?
                it->second->setFromString(SoraTypeSerializer::toString(val));
            }
        }
    }
    
    inline void SoraPropertyHolder::removeAllProperties() {
        PropertyMap::iterator it = mProperties.begin();
        for(; it != mProperties.end(); ++it) {
            delete it->second;
        }
        mProperties.clear();
    }
    
    inline const SoraPropertyHolder::PropertyMap& SoraPropertyHolder::getProperties() const {
        return mProperties;
    }
    
    inline size_t SoraPropertyHolder::getPropertySize() const {
        return mProperties.size();
    }
} // namespace sora


#endif
