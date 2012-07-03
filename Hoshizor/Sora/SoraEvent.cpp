#include "SoraEvent.h"
#include "SoraSystemEvents.h"
#include "SoraPreDeclare.h"

#include "event/SoraEventWorld.h"
#include "message/SoraMessageEvent.h"

namespace sora {

    SoraEventHandler::~SoraEventHandler() {
        Handlers::iterator it = _handlers.begin();
        while(it != _handlers.end()) {
            delete it->second;
            ++it;
        }
        _handlers.clear();
        
        if(this->isInWorld())
            mWorld->leave(this);
    }
    
    void SoraEventHandler::handleEvent(SoraEvent* event) {
        sora_assert(event);
        
        if(event->getEventIdentifier() == SoraMessageEvent::GetEventIdentifier()) {
            onMessage(static_cast<SoraMessageEvent*>(event));
        }
        
#ifndef SORA_EVENT_GENERIC
        
#ifdef SORA_USE_RTTI
        Handlers::iterator it = _handlers.find(SoraTypeInfo(typeid(*event)));
#else
        Handlers::iterator it = _handlers.find(SoraTypeInfo(event->getEventIdentifier()));
#endif
        if(it != _handlers.end()) {
            it->second->exec(event);
        }
        
#else
        this->onEvent(event);
#endif
    }
    
    void SoraEventHandler::handleSystemEvent(SoraSystemEvent* evt) {
        if(mEnableUpdate && evt->getType() == SYS_EVT_UPDATE) {
            onUpdate(evt->getDelta());
            if(mUpdateReceiveEvent)
                handleEvent(evt);
        } else if(evt->getType() == SYT_EVT_ON_DEBUG_RENDER) {
            onDebugRender();
        }
    }
    
    void SoraEventHandler::handleWorldEvent(SoraEvent* evt) {
        if(isEnabled()) {
            handleEvent(evt);
        }
    }
    
    SoraEventHandler::SoraEventHandler():
    mEnabled(false),
    mWorld(NULL),
    mEnableUpdate(false),
    mUpdateReceiveEvent(false) {
    }
                
    bool SoraEventHandler::isEnabled() {
        return mEnabled;
    }
    
    bool SoraEventHandler::isEnableUpdate() const {
        return mEnableUpdate;
    }
    
    void SoraEventHandler::enableUpdate(bool receiveEvent) {
        mUpdateReceiveEvent = receiveEvent;
        mEnableUpdate = true;
    }
    
    void SoraEventHandler::disableUpdate(void) {
        mEnableUpdate = false;
    }
    
    void SoraEventHandler::enable(SoraEventWorld* world) {
        mWorld = world;
        mEnabled = true;
        
        onEnable();
    }
    
    void SoraEventHandler::disable() {
        mEnabled = false;
        onDisable();
        mWorld = NULL;
    }
    
    bool SoraEventHandler::isInWorld(SoraEventWorld* world) const {
        if(mWorld) {
            return mWorld == world;
        }
        
        return false;
    }

    bool SoraEventHandler::isInWorld() const {
        return mWorld != NULL;
    }
    
    SoraEventWorld* SoraEventHandler::getWorld() {
        return mWorld;
    }
    
    void SoraEventHandler::onEnable() {
        
    }
    
    void SoraEventHandler::onDisable() {
        
    }
    
    void SoraEventHandler::onUpdate(float dt) {
        
    }
    
    void SoraEventHandler::onDebugRender() {
        
    }
    
    void SoraEventHandler::onMessage(SoraMessageEvent* message) {
        
    }
    
#ifdef SORA_EVENT_GENERIC
    void SoraEventHandler::onEvent(SoraEvent* event) {
        
    }
#endif
    
} // namespace sora