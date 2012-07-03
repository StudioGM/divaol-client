//
//  SoraFullscreenBufferHandler.h
//  Sora
//
//  Created by Robert Bu on 7/15/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraFullscreenBufferHandler_h
#define Sora_SoraFullscreenBufferHandler_h

#include "SoraPlatform.h"
#include "SoraSingleton.h"
#include "function/SoraFunction.h"

namespace sora {
    
    class SoraSprite;
    
    class SoraFullscreenBufferHandler: public SoraSingleton<SoraFullscreenBufferHandler> {
    protected:  
        friend class SoraSingleton<SoraFullscreenBufferHandler>;
        
        SoraFullscreenBufferHandler();
        ~SoraFullscreenBufferHandler();
        
    public:
        void onBufferRender(SoraTextureHandle buffer);
        
        typedef SoraFunction<void(SoraTextureHandle)> DelegateFuncType;
        void registerDelegate(const DelegateFuncType& delegate);
        void unregisterDelegate();
        
    private:
        SoraSprite* mBufferSprite;
        SoraHandle mBufferTarget;
        
        DelegateFuncType mDelegate;
    };
    
} // namespace sora


#endif
