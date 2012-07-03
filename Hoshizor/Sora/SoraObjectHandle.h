//
//  SoraObjectHandle.h
//  Sora
//
//  Created by Robert Bu on 7/13/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraObjectHandle_h
#define Sora_SoraObjectHandle_h

#include "SoraObject.h"

namespace sora {
    
    class SORA_API SoraObjectHandle {
    public:
        enum { MaxObjectSize = 65525 };
        
        explicit SoraObjectHandle(SoraObject* obj);
        ~SoraObjectHandle();
        
        SoraObject* toObject();
        
        SoraObjectHandle& operator =(const SoraObjectHandle& rhs);
        
        bool operator==(const SoraObjectHandle& rhs) const;
        
        operator SoraObject*();
        
        SoraHandle getHandleId() const;
        SoraUniqueId getUniqueId() const;
        
        static void freeObjectHandle(SoraHandle handle);
        static uint32 getGlobalObjectSize();
        
    private:
        SoraObjectHandle(SoraHandle handle, SoraUniqueId uid);
        SoraObjectHandle();

        SoraHandle m_HandleId;
        SoraUniqueId m_UniqueId;
    }; 
    
    static SoraObjectHandle toObjectHandle(SoraObject* obj) {
        return SoraObjectHandle(obj);
    }
    
} // namespace sora



#endif
