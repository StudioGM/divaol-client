//
//  SoraComponentHolder.cpp
//  Sora
//
//  Created by Robert Bu on 8/21/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraComponentHolder.h"
#include "SoraException.h"
#include "SoraStringTokenlizer.h"
#include "SoraComponent.h"

namespace sora {
    
    void SoraComponentHolder::addComponent(SoraComponent* cop) {
        printf("%s\n", cop->getName().c_str());
        if(getComponent(cop->getName()) != NULL)
            THROW_SORA_EXCEPTION(ExistsException, "Component already exists!");
        mComponents.insert(std::make_pair(cop->getName(), cop));
        
        ComponentMap::iterator it = mComponents.begin();
        while(it != mComponents.end()) {
            // inform heavy weight components that a new component have been added
            if(it->second->isHeavyWeight()) {
                it->second->onComponentAdd(cop);
            }
            
            // inform the new component of all existing components
            cop->onComponentAdd(it->second);
            
            ++it;
        }
        
        cop->onInit();
    }
    
    SoraComponent* SoraComponentHolder::getComponent(const SoraString& tag) const {
        ComponentMap::const_iterator it = mComponents.find(tag);
        if(it != mComponents.end())
            return it->second;
        return 0;
    }
    
    SoraComponent* SoraComponentHolder::removeComponent(const SoraString& tag) {
        ComponentMap::iterator it = mComponents.find(tag);
        if(it != mComponents.end()) {
            SoraComponent* co = it->second;
            mComponents.erase(it);
            
            ComponentMap::iterator itComp = mComponents.begin();
            while(itComp != mComponents.end()) {
                // inform heavy weight components that a component have been removed
                if(itComp->second->isHeavyWeight()) {
                    itComp->second->onComponentRemove(co);
                }
                ++itComp;
            }
            
            co->onRemove();
            return co;
        }
        return 0;
    }
    
    SoraComponent* SoraComponentHolder::removeComponent(const SoraComponent* co) {
        ComponentMap::iterator it = mComponents.begin();
        ComponentMap::iterator end = mComponents.end();
        for(; it != end; ++it) {
            if(it->second == co) {
                SoraComponent* co2 = it->second;
                mComponents.erase(it);
                
                ComponentMap::iterator itComp = mComponents.begin();
                while(itComp != mComponents.end()) {
                    // inform heavy weight components that a component have been removed
                    if(itComp->second->isHeavyWeight()) {
                        itComp->second->onComponentRemove(co2);
                    }
                    ++itComp;
                }
                
                co2->onRemove();
                return co2;
            }
        }
        return 0;
    }
    
    void SoraComponentHolder::sendMessage(SoraMessageEvent* message) {
        ComponentMap::const_iterator it = mComponents.begin();
        while(it != mComponents.end()) {
            it->second->onMessage(message);
            if(message->isConsumed()) {
                break;
            }
            ++it;
        }
    }
    
    void SoraComponentHolder::sendMessageTo(const SoraString& to, SoraMessageEvent* message) {
        ComponentMap::const_iterator it = mComponents.find(to);
        if(it != mComponents.end()) {
            it->second->onMessage(message);
        }
    }
 /*   
    bool SoraComponentHolder::hasProperty(const DynRttiClassKeyType& pid) {
        SoraStringTokenlizer token(pid);
        if(token.size() == 2) {
            SoraComponent* comp = getComponent(token.first());
            if(comp)
                return comp->getProperty(token.last()) != 0;
        }
        return false;
    }
    
    void SoraComponentHolder::addProperty(const DynRttiClassKeyType& pid, SoraPropertyBase* prop) {
        SoraStringTokenlizer token(pid);
        if(token.size() == 2) {
            SoraComponent* comp = getComponent(token.front());
            if(comp)
                comp->addProperty(MakeProperty(token.last(), prop));
        }
    }
    
    SoraPropertyBase* SoraComponentHolder::getProperty(const DynRttiClassKeyType& pid) const {
        SoraStringTokenlizer token(pid);
        if(token.size() == 2) {
            SoraComponent* comp = getComponent(token.front());
            if(comp)
                return comp->getProperty(token.back());
        }
        return 0;
    }*/
    
    void SoraComponentHolder::onUpdate(float dt) {
        ComponentMap::const_iterator it = mComponents.begin();
        while(it != mComponents.end()) {
            it->second->onUpdate(dt);
            ++it;
        }
    }
    
    void SoraComponentHolder::onRender() {
        ComponentMap::const_iterator it = mComponents.begin();
        while(it != mComponents.end()) {
            it->second->onRender();
            ++it;
        }
    }
    
    bool SoraComponentHolder::hasComponent(const SoraString& tag) const {
        ComponentMap::const_iterator it = mComponents.begin();
        return it != mComponents.end();
    }
} // namespace sora
