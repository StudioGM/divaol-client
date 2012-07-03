//
//  SoraMovable.h
//  Sora
//
//  Created by Robert Bu on 8/18/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraMovable_h
#define Sora_SoraMovable_h

#include "SoraMath.h"
#include "math/SoraTransform.h"

namespace sora {
    
    // base class for all movable things
    
    class SORA_API SoraMovable {
    protected:
        SoraMovable() {};
        
    public:
        virtual ~SoraMovable() {} 
        
        virtual void setPosition(const SoraVector3& pos) {
            mTransform.setPosition(pos);
        }
        
        virtual SoraVector3 getPosition() const {
            return mTransform.getPosition();
        }
        
        void setTransform(const SoraTransform& transform) {
            mTransform = transform;
        }
        
        const SoraTransform& getTransform() const {
            return mTransform;
        }
        
        inline void translate(const SoraVector3& d) {
            setPosition(getPosition() + d);
        }
        
    protected:
        SoraTransform mTransform;
    };
    
    
} // namespace sora



#endif
