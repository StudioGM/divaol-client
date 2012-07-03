//
//  SoraObjectHandle.cpp
//  Sora
//
//  Created by Robert Bu on 7/13/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraObjectHandle.h"

namespace sora {
    
    static SoraObject* g_ObjectMap[SoraObjectHandle::MaxObjectSize];
    static uint32 g_ObjectSize = 0;
    
    SoraObjectHandle::SoraObjectHandle(SoraObject* obj):
    m_HandleId(obj->getHandleId()),
    m_UniqueId(obj->getUniqueId()) {
        sora_assert(m_HandleId < SoraObjectHandle::MaxObjectSize);
        g_ObjectMap[m_HandleId] = obj;
        ++g_ObjectSize;
    }
    
    SoraObjectHandle::~SoraObjectHandle() {
        --g_ObjectSize;
    }
    
    SoraObjectHandle::SoraObjectHandle(SoraHandle handle, SoraUniqueId uid):
    m_HandleId(handle),
    m_UniqueId(uid) {
        
    }
        
    SoraObject* SoraObjectHandle::toObject() {
        assert(m_HandleId < SoraObjectHandle::MaxObjectSize);
        SoraObject* object = g_ObjectMap[m_HandleId];
        if(object != NULL &&
           object->getUniqueId() == m_UniqueId)
            return object;
        return NULL;
    }
    
    SoraObjectHandle& SoraObjectHandle::operator =(const SoraObjectHandle& rhs) {
        if(&rhs != this) {
            m_HandleId = rhs.m_HandleId;
            m_UniqueId = rhs.m_UniqueId;
        }
		return *this;
    }
    
    bool SoraObjectHandle::operator==(const SoraObjectHandle& rhs) const {
        return m_HandleId == rhs.m_HandleId && m_UniqueId == rhs.m_UniqueId;
    }
    
    SoraObjectHandle::operator SoraObject*() {
        return toObject();
    }
    
    SoraHandle SoraObjectHandle::getHandleId() const {
        return m_HandleId;
    }
    
    SoraUniqueId SoraObjectHandle::getUniqueId() const {
        return m_UniqueId;
    }

    void SoraObjectHandle::freeObjectHandle(SoraHandle handle) {
        assert(handle < SoraObjectHandle::MaxObjectSize);
        g_ObjectMap[handle] = NULL;
    }
    
    uint32 SoraObjectHandle::getGlobalObjectSize() {
        return g_ObjectSize;
    }
} // namespace sora