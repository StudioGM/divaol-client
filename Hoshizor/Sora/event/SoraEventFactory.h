//
//  SoraEventFactory.h
//  Sora
//
//  Created by Robert Bu on 8/6/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraEventFactory_h
#define Sora_SoraEventFactory_h

#include "SoraPlatform.h"
#include "factory/SoraDynamicFactory.h"
#include "SoraEvent.h"

namespace sora {

    /**
     * Event factory class
     * You must register before using this
     **/
    
    class SoraEventFactory: public SoraDynamicFactory<SoraEvent> {
    public:
        typedef SoraDynamicFactory<SoraEvent>::AbstractFactory InstantiatorType;
        
        void destroyEvent(SoraEvent* evt);
        
        /**
         * If name is ignored, would create a SoraSystemEvent
         * Otherwise if name is not recognized throw NotFoundException
         **/
        SoraEvent* createEvent(const std::string& t=std::string());
        
        /**
         *  same as DynamicFactory::registerClass
         *  Would throw ExistsException if the event name alreay exists
         **/
        template<typename T>
        void registerEvent(const std::string& name);
        
        template<typename T>
        void registerEvent(const std::string& name, InstantiatorType* instantiator);
        
        static SoraEventFactory* Instance();
        static SoraEventFactory& RefInstance();
        
    private:
        SoraEventFactory();
        ~SoraEventFactory();
    };
    
    template<typename T>
    inline void SoraEventFactory::registerEvent(const std::string& name) {
        SoraDynamicFactory<SoraEvent>::registerClass<T>(name);
    }
    
    template<typename T>
    inline void SoraEventFactory::registerEvent(const std::string& name, InstantiatorType* instantiator) {
        SoraDynamicFactory<SoraEvent>::registerClass<T>(name, instantiator);
    }
    
    inline SoraEvent* CreateEvent(const std::string& name) {
        return SoraEventFactory::Instance()->createEvent(name);
    }
    
    inline void DestroyEvent(SoraEvent* evt) {
        return SoraEventFactory::Instance()->destroyEvent(evt);
    }
    
    template<typename T>
    inline void RegisterEvent(const std::string& name) {
        SoraEventFactory::Instance()->registerEvent<T>(name);
    }
} // namespace sora

#endif
