//
//  SoraFSM.h
//  Sora
//
//  Created by Robert Bu on 7/17/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraFSM_h
#define Sora_SoraFSM_h

/**
 *  A Simple FSM(Finite State Machine) Implemention
 *  No advanced FSM features such as HSM supported here
 **/

#include "SoraPlatform.h"
#include "SoraStringId.h"
#include <map>

namespace sora {
    
    class SoraFSMManager;
        
    class SORA_API SoraFSMState {
    public:
        friend class SoraFSMManager;
        
        SoraFSMState(): mInitiated(false), mManager(NULL) {}
        
        virtual void onInitiate() {}
        virtual void onDestroy() {
            mManager = 0;
            delete this;
        }
        
        virtual void onUpdate(float dt) {}
        virtual void onRender() {}
        
        virtual void onEnter() {}
        virtual void onLeave() {}
        
        SoraFSMManager* getManager() const {
            return mManager;
        }
                
    private:
        bool mInitiated;
        SoraFSMManager* mManager;
    };
    
    class SORA_API SoraFSMManager {
    public:
        typedef std::string EventType;
        
        SoraFSMManager();
        ~SoraFSMManager();
        
        SoraFSMManager& defState(SoraFSMState* state, const SoraString& name);
        void delState(const SoraString& name);
        
        void defTrans(const SoraString& state1, const EventType& event, const SoraString& state2);
        void delTrans(const SoraString& state, const EventType& event);
        
        void postEvent(const EventType& event);
        
        void switchToState(const SoraString& name);
        void setGlobalState(SoraFSMState* state);
        
        SoraFSMState* getCurrentState() const;
        SoraFSMState* getPreviousState() const;
        SoraFSMState* getGlobalState() const;
        
        void onUpdate(float dt);
        void onRender();
        
        bool returnToPreviousState();
        
        static SoraFSMManager& defaultFSMManager();
        
        SoraFSMState* operator[](const SoraString& name) const;
        
    private:
        void switchToState(SoraFSMState* state);

        typedef std::map<SoraStringId, SoraFSMState*> FSMStateMap;
        FSMStateMap mStates;
        
        typedef std::map<EventType, SoraFSMState*> StateEventMap;
        struct EventTrans {
            StateEventMap EventMap;
        };
        typedef std::map<SoraFSMState*, EventTrans> StateTransitionMap;
        StateTransitionMap mTransitions;
        
        SoraFSMState* mGlobalState;
        SoraFSMState* mCurrentState;
        SoraFSMState* mPreviousState;
    };
} // namespace sora



#endif
