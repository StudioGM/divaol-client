//
//  SoraAutoUpdate.h
//  Sora
//
//  Created by Robert Bu on 8/21/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraAutoUpdate_h
#define Sora_SoraAutoUpdate_h

/**
 * Classes that will be updated each frame
 **/
#include "SoraPlatform.h"
#include <list>

namespace sora {

    class SORA_API SoraAutoUpdate {
    public:
        SoraAutoUpdate() {
            GetObjectList().push_back(this);
        }
        
        virtual ~SoraAutoUpdate() {
            GetObjectList().remove(this);
        }
        
        virtual void onUpdate(float dt) = 0;
        
    public:
        static void UpdateList(float dt);
        
        typedef std::list<SoraAutoUpdate*> UpdateObjectList;

        static UpdateObjectList& GetObjectList() {
            static UpdateObjectList static_objects;
            return static_objects;
        }
        
    private:
    };

} // namespace sora



#endif
