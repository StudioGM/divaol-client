//
//  SoraSimpleStateMachine.h
//  Sora
//
//  Created by Robert Bu on 7/31/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraSimpleStateMachine_h
#define Sora_SoraSimpleStateMachine_h

#include "SoraPlatform.h"
#include "SoraLogger.h"
#include <map>
#include <string>

namespace sora {
    
    /**
     *  A Simple FSM Based on a user defined compile-time state_type
     **/
    
    template<typename t>
    struct simple_fsm_do_nothing_one {
        void operator()(t) {
        }
    };
    
    template<typename t1, typename t2>
    struct simple_fsm_do_nothing_two {
        void operator()(t1, t2) {
            
        }
    };
    
    /**
     *  Get Null state for initial state
     *  You can disable null state by disable SORA_FSM_USE_NULL
     **/
    template<typename state_type>
    const state_type& simple_fsm_get_null_state();
    
    template<>
    inline const unsigned char& simple_fsm_get_null_state() {
        static unsigned char null = 0xff;
        return null;
    }
    
    template<>
    inline const int& simple_fsm_get_null_state() {
        static int null = 0xffffffff;
        return null;
    }
    
    template<>
    inline const std::string& simple_fsm_get_null_state() {
        static std::string null("null");
        return null;
    }
    
    template<typename state_type,
             typename event_type>
    struct SoraSimpleFSM {   
        typedef state_type      StateType;
        typedef event_type      EventType;
        typedef SoraSimpleFSM   MachineType;
        typedef simple_fsm_do_nothing_one<state_type> DoNothingOp1;
        typedef simple_fsm_do_nothing_two<MachineType, state_type> DoNothingOp2;
        typedef SoraSimpleFSM<state_type, event_type> SelfType;
        
#ifdef SORA_FSM_USE_NULL
        SoraSimpleFSM():
        mCurrState(simple_fsm_get_null_state<state_type>()) {
        }
#else
        SoraSimpleFSM() {
        }
#endif
        
        SelfType& add(state_type state) {
            mStateMap.insert(std::make_pair(state, state_trans()));
            return *this;
        }
        SelfType& del(state_type state) {
            typename StateMap::iterator itState = mStateMap.find(mCurrState);
            if(itState != mStateMap.end()) {
                mStateMap.erase(itState);
            }
            return *this;
        }
        
        void setState(state_type state) {
            typename StateMap::iterator itState = mStateMap.find(mCurrState);
            if(itState == mStateMap.end()) {
                log_error("SoraSimpleFSM::setState: unregistered state");
                return;
            }
            mCurrState = state;
        }
        
        void clear() {
#ifdef SORA_FSM_USE_NULL
            mCurrState = simple_fsm_get_null_state<state_type>();
#else 
            mCurrState = state_type();
#endif 
            mStateMap.clear();
        }
        
        SelfType& defTrans(state_type state, event_type event, state_type stateto) {
            typename StateMap::iterator itState = mStateMap.find(mCurrState);
            if(itState == mStateMap.end()) {
                // should we allow this?
                state_trans trans;
                trans.mTransitions.insert(std::make_pair(event, stateto));
                mStateMap.insert(std::make_pair(state, trans));
            } else {
                itState->second.mTransitions.insert(std::make_pair(event, stateto));
            }
            return *this;
        }
        
        SelfType& delTrans(state_type state, event_type event) {
            typename StateMap::iterator itState = mStateMap.find(mCurrState);
            if(itState != mStateMap.end()) {
                typename StateEventMap::iterator itEvent = itState->second.mTransitions.find(event);
                if(itEvent != itState->second.mTransitions.end()) {
                    itState->second.mTransitions.erase(itEvent);
                }
            }
            return *this;
        }
        
        SelfType& procEvent(event_type event) {
            DoNothingOp2 donothing;
            procEvent(event, donothing, donothing);
            return *this;
        }
        
        template<typename event_enter_op, typename event_exit_op>
        void procEvent(event_type event, event_enter_op enterOp, event_exit_op exitOp) {
            typename StateMap::iterator itState = mStateMap.find(mCurrState);
            if(itState != mStateMap.end()) {
                typename StateEventMap::iterator itEvent = itState->second.mTransitions.find(event);
                if(itEvent != itState->second.mTransitions.end()) {
                    exitOp(*this, mCurrState);
                    mCurrState = itEvent->second;
                    enterOp(*this, mCurrState);
                }
            }
        }
        
        size_t numStates() const {
            return mStateMap.size();
        }
        size_t numTransitions(const state_type& state) const {
            typename StateMap::const_iterator itState = mStateMap.find(state);
            if(itState != mStateMap.end()) {
                return itState->second.mTransitions.size();
            }
        }
        
        const state_type& curr() const {
            return mCurrState;
        }
        
    private:
        typedef std::map<event_type, state_type> StateEventMap;
        struct state_trans {
            StateEventMap mTransitions;
        };
        typedef std::map<state_type, state_trans> StateMap;
        StateMap mStateMap;
        
        state_type mCurrState;
    };
    
    
} // namespace sora


#endif
