//
//  SoraExternalRenderObject.h
//  Sora
//
//  Created by Robert Bu on 7/19/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraExternalRenderObject_h
#define Sora_SoraExternalRenderObject_h

#include "SoraPlatform.h"
#include "SoraObject.h"
#include "SoraDelegate.h"
#include "function/SoraFunction.h"

namespace sora {
    
    /**
     *  A special object that adapts it's render function to a 
     *  external render delegate
     *  For use with subobjects such as SoraScene etc
     **/
    
    class SORA_API SoraExternalRenderObject: public SoraObject {
    public:
        typedef SoraFunction<void(SoraObject*)> RenderDelegate;
        
        SoraExternalRenderObject();
        template<typename F>
        SoraExternalRenderObject(const F& delegate);
 
        template<typename F>
        void setDelegate(const F& delegate);
 
        virtual void render();
        
    private:
        RenderDelegate mDelegate;
    };
    
    template<typename F>
    SoraExternalRenderObject::SoraExternalRenderObject(const F& delegate) {
        mDelegate = delegate;
    }
    
    template<typename F>
    void SoraExternalRenderObject::setDelegate(const F& delegate) {
        mDelegate = delegate;
    }
    
    void SoraExternalRenderObject::render() {
        if(mDelegate && this->getParent()) {
            mDelegate(this->getParent());
        }
    }
    
}

#endif
