//
//  animationlistener.h
//  Guichan_Sora
//
//  Created by Robert Bu on 8/28/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef gcn_animationlistener_h
#define gcn_animationlistener_h

namespace gcn {
    
    class Animation;
    
    class AnimationListener {
    public:
        virtual void animationBegan(Animation* animation) { }
        
        virtual void animationEnded(Animation* animation) { }
    };
    
} // namespace gcn



#endif
