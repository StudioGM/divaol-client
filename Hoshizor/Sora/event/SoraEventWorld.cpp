//
//  SoraEventWorld.cpp
//  Sora
//
//  Created by Robert Bu on 8/5/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraEventWorld.h"
#include "SoraEventWorldImpl.h"
#include "SoraSingletonHolder.h"
#include "SoraException.h"
#include "SoraSystemEvents.h"
#include "SoraEventFactory.h"
#include "SoraEventWorldHolder.h"
#include "SoraCore.h"

namespace sora {
    
    SoraEventWorld::SoraEventWorld(const Parameter& param) {
        if(param.isMultiThread()) {
#ifdef SORA_ENABLE_MULTI_THREAD
            mImpl.reset(new SoraEventWorldImpl());
#else 
            mImpl.reset(new SoraEventWorldImpl());
            THROW_SORA_EXCEPTION(RuntimeException, "You must enable SORA_ENALBE_MULTI_THREAD to use multi threaded EventWorld");
#endif
        } else {
            mImpl.reset(new SoraEventWorldImpl());
        }
        mFixedStep = param.isFixedStep();
        
        if(!init()) 
            THROW_SORA_EXCEPTION(RuntimeException, "Error initializing SoraEventWorld");
        
        SoraEventWorldHolder::Instance()->registerEventWorld(this);
    }
    
    SoraEventWorld::~SoraEventWorld() {
        SoraEventWorldHolder::Instance()->removeEventWorld(this);
    }
    
    void SoraEventWorld::update(float dt) {
        if(mFixedStep) {
            uint32 step = static_cast<uint32>(dt / SoraCore::Instance()->getDelta());
            for(uint32 i=0; i<step; ++i)
                mImpl->update(dt);
        } else {
            mImpl->update(dt);
        }
    }
    
    void SoraEventWorld::enter(EventHandlerPtr handler) {
        handler->enable(this);
        handler->onEnable();
        mImpl->enter(handler);
    }
    
    void SoraEventWorld::leave(EventHandlerPtr handler) {
        if(!handler->isInWorld(this)) {
            return;
        }
        
        handler->disable();
        handler->onDisable();
        mImpl->leave(handler);
    }
    
    bool SoraEventWorld::init() {
        return mImpl->init();
    }
    
    void SoraEventWorld::broadcast(SoraEvent* evt) {
        if(IsSystemEvent(evt)) {
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Cannot broadcasting a system event");
        }
        mImpl->broadcasting(evt);
    }
    
    void SoraEventWorld::broadcastDirect(SoraEvent* evt) {
        if(IsSystemEvent(evt)) {
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Cannot broadcasting a system event");
        }
        mImpl->broadcastingDirect(evt);
    }
    
    void SoraEventWorld::broadcast(const std::string& event) {
        SoraEvent* evt = CreateEvent(event);
        if(evt != 0)
            broadcast(evt);
    }
   
    void SoraEventWorld::broadcastDirect(const std::string& event) {
        SoraEvent* evt = CreateEvent(event);
        if(evt != 0)
            broadcastDirect(evt);
    }
    
    namespace {
        static SoraSingletonHolder<SoraEventWorld, SoraEventWorld::Parameter> world;
    }
    
    SoraEventWorld& SoraEventWorld::defaultWorld(const Parameter& param) {
        return *world.get(param);
    }
} // namespace sora
