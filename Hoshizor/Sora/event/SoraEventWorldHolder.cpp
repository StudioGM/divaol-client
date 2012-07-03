//
//  SoraEventWorldHolder.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraEventWorldHolder.h"
#include "SoraEventFactory.h"
#include "SoraEventWorld.h"

namespace sora {
    
    static SoraEventWorldHolder* g_instance = 0;
    
    SoraEventWorldHolder* SoraEventWorldHolder::Instance() {
        if(!g_instance)
            g_instance = new SoraEventWorldHolder;
        return g_instance;
    }
    
    void SoraEventWorldHolder::registerEventWorld(SoraEventWorld* world) {
        mEventWorlds.push_back(world);
    }
    
    void SoraEventWorldHolder::removeEventWorld(SoraEventWorld* world) {
        mEventWorlds.remove(world);
        
        EventWorldMap::iterator it = mEventWorldMap.begin();
        EventWorldMap::iterator end = mEventWorldMap.end();
        for (; it != end; ++it) {
            if(it->second == world) {
                mEventWorldMap.erase(it);
                break;
            }
        }
    }
    
    void SoraEventWorldHolder::registerEventWorld(const std::string& name, SoraEventWorld* world) {
        EventWorldMap::const_iterator it = mEventWorldMap.find(name);
        if(it == mEventWorldMap.end())
            mEventWorldMap.insert(std::make_pair(name, world));
    }
    
    void SoraEventWorldHolder::removeEventWorld(const std::string& name) {
        EventWorldMap::iterator it = mEventWorldMap.find(name);
        if(it != mEventWorldMap.end())
            mEventWorldMap.erase(it);
    }
    
    void SoraEventWorldHolder::broadcast(const std::string& evtName, const std::string& to) {
        SoraEvent* evt = CreateEvent(evtName);
        if(evt != 0)
            broadcast(evt, to);
    }
    
    void SoraEventWorldHolder::broadcast(SoraEvent* evt, const std::string& to) {
        if(!to.empty()) {
            EventWorldMap::iterator it = mEventWorldMap.find(to);
            if(it != mEventWorldMap.end())
                it->second->broadcast(evt);
        } else {
            EventWorldArray::iterator it = mEventWorlds.begin();
            EventWorldArray::iterator end = mEventWorlds.end();
            for(; it != end; ++it)
                (*it)->broadcast(evt);
        }
    }
    
    void SoraEventWorldHolder::broadcastDirect(const std::string& evtName, const std::string& to) {
        SoraEvent* evt = CreateEvent(evtName);
        if(evt != 0)
            broadcastDirect(evt, to);
    }
    
    void SoraEventWorldHolder::broadcastDirect(SoraEvent* evt, const std::string& to) {
        if(!to.empty()) {
            EventWorldMap::iterator it = mEventWorldMap.find(to);
            if(it != mEventWorldMap.end())
                it->second->broadcastDirect(evt);
        } else {
            EventWorldArray::iterator it = mEventWorlds.begin();
            EventWorldArray::iterator end = mEventWorlds.end();
            for(; it != end; ++it)
                (*it)->broadcastDirect(evt);
        }
    }
    
    SoraEventWorld* SoraEventWorldHolder::getEventWorld(const std::string& name) {
        EventWorldMap::iterator it = mEventWorldMap.find(name);
        if(it != mEventWorldMap.end())
            return it->second;
        return 0;
    }
    
    
} // naemspace sora