/*
 *  RefCounted.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/16/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_REF_COUNTED_H_
#define SORA_REF_COUNTED_H_

#include "SoraUncopyable.h"

namespace sora {
	
	class SORA_API SoraRefCounted: SoraUncopyable {
    public:
        SoraRefCounted(): counter(1) {}
        
        void duplicate() const;
        void release() const;
        int refCount() const;
        
    protected:
        virtual ~SoraRefCounted() {}
        
    private:
        mutable int counter;
    };
    
    inline int SoraRefCounted::refCount() const {
        return counter;
    }
    
    inline void SoraRefCounted::duplicate() const {
        ++counter;
    }
    
    inline void SoraRefCounted::release() const {
        if(--counter == 0)
            delete this;
    }
    
	
} // namespace sora


#endif // SORA_REF_COUNTED_H_