//
//  SoraAutoUpdate.cpp
//  Sora
//
//  Created by Robert Bu on 8/21/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraAutoUpdate.h"
#include "SoraIteratorHelper.h"

namespace sora {
        
    void SoraAutoUpdate::UpdateList(float dt) {
        ConstVectorIterator<UpdateObjectList> iterator(GetObjectList());
        while(iterator.hasMoreElements()) {
            iterator.getNext()->onUpdate(dt);
        }
    }
    
} // namespace sora
