//
//  SoraGameState.h
//  Sora
//
//  Created by Ruiwei Bu on 9/17/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraGameState_h
#define Sora_SoraGameState_h

#include "SoraPlatform.h"
#include "SoraFSM.h"
#include "SoraInputListeners.h"

namespace sora {
    
    class SoraGameApp;
    
    class SORA_API SoraGameState: public SoraFSMState, public SoraInputListener {
    public:
        SoraGameApp* getGameApp() const {
            return mGameApp;
        }
        
        
    private:
        friend class SoraGameApp;
        void setGameApp(SoraGameApp* app) {
            mGameApp = app;
        }
        
        SoraGameApp* mGameApp;
    };
    
} // namespace sora



#endif
