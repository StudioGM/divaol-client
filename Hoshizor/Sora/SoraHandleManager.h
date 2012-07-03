//
//  SoraHandleManager.h
//  Sora
//
//  Created by Robert Bu on 7/13/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraHandleManager_h
#define Sora_SoraHandleManager_h

#include "SoraPlatform.h"
#include "SoraSingleton.h"

#include <deque>
#include <map>

namespace sora {
    
    class SORA_API SoraHandleManager {
    protected:
        friend class SoraSingleton<SoraHandleManager>;
        SoraHandleManager();
        
    public:
        static SoraHandleManager& GetInstance();
        
        SoraHandle getUniqueHandle();
        SoraUniqueId getNextUniqieId();
        
        void freeHandle(SoraHandle handle);
                
    private:
        SoraHandle m_NextUniqueId;
        SoraUniqueId m_NextUniqueHandle;
        
        typedef std::deque<SoraUniqueId> FreeUniqieHandles;
        FreeUniqieHandles m_FreeHandles;
    };
    
    static SoraUniqueId GetNextUniqueId() {
        return SoraHandleManager::GetInstance().getNextUniqieId();
    }
    
    static SoraHandle FindFreeHandleSlot() {
        return SoraHandleManager::GetInstance().getUniqueHandle();
    }
    
    static void FreeHandleSlot(SoraHandle handle) {
        SoraHandleManager::GetInstance().freeHandle(handle);
    }
    
} // namespace sora


#endif
