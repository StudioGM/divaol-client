//
//  SoraComponentHolder.h
//  Sora
//
//  Created by Robert Bu on 8/21/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraComponentHolder_h
#define Sora_SoraComponentHolder_h

#include "SoraPlatform.h"
#include "SoraUncopyable.h"
#include "message/SoraMessageEvent.h"
#include "property/SoraProperty.h"

namespace sora {
    
    class SoraComponent;
    
    struct SORA_API SoraComponentHolder: SoraUncopyable {
        SoraComponentHolder() {}
        
        /**
         * Add a component, may throw a ExistsException
         **/
        void addComponent(SoraComponent* cop);
        
        SoraComponent* removeComponent(const SoraString& tag);
        SoraComponent* removeComponent(const SoraComponent*);
        
        SoraComponent* getComponent(const SoraString& tag) const;
     
        bool hasComponent(const SoraString& tag) const;
        
        void sendMessage(SoraMessageEvent* message);
        void sendMessageTo(const SoraString& to, SoraMessageEvent* message);
        
        void onUpdate(float dt);
        void onRender();
        
    private:
        typedef sora_hash_map<SoraString, SoraComponent*> ComponentMap;
        ComponentMap mComponents;
    };
    
} // namespace sora


#endif
