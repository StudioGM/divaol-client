
//
//  SoraFSM.cpp
//  Sora
//
//  Created by Robert Bu on 7/17/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraFSM.h"
#include "SoraSingletonHolder.h"
#include "SoraException.h"

namespace sora {
    
    SoraFSMManager::SoraFSMManager(): 
    mGlobalState(NULL),
    mCurrentState(NULL),
    mPreviousState(NULL) {
        
    }
    
    SoraFSMManager::~SoraFSMManager() {
        FSMStateMap::iterator itState = mStates.begin();
        while(itState != mStates.end()) {
            itState->second->onDestroy();
            itState->second->mManager = NULL;
            ++itState;
        }
    }
    
    SoraFSMManager& SoraFSMManager::defState(SoraFSMState* state, const SoraString& name) {
        SoraStringId uniqueId = GetUniqueStringId(name);
        mStates.insert(std::make_pair(uniqueId, state));
        state->onInitiate();
        state->mManager = this;
        return *this;
    }
    
    void SoraFSMManager::delState(const SoraString& name) {
        SoraStringId uniqueId = GetUniqueStringId(name);
        FSMStateMap::iterator itState = mStates.find(uniqueId);
        if(itState != mStates.end()) {
            itState->second->onDestroy();
            itState->second->mManager = NULL;
            mStates.erase(itState);
        }
    }
    
    void SoraFSMManager::switchToState(const SoraString& name) {
        SoraStringId uniqueId = GetUniqueStringId(name);
        FSMStateMap::iterator itState = mStates.find(uniqueId);
        if(itState != mStates.end()) {
            switchToState(itState->second);
        }
    }
    
    void SoraFSMManager::switchToState(SoraFSMState* state) {
        assert(state);
        
        if(mCurrentState != NULL) {
            mCurrentState->onLeave();
            mPreviousState = mCurrentState;
        }
        mCurrentState = state;
        if(mCurrentState)
            mCurrentState->onEnter();
    }
    
    SoraFSMState* SoraFSMManager::getCurrentState() const {
        return mCurrentState;
    }
    
    SoraFSMState* SoraFSMManager::getPreviousState() const {
        return mPreviousState;
    }
    
    SoraFSMState* SoraFSMManager::getGlobalState() const {
        return mGlobalState;
    }
    
    void SoraFSMManager::onUpdate(float dt) {
        if(mCurrentState)
            mCurrentState->onUpdate(dt);
        if(mGlobalState)
            mGlobalState->onUpdate(dt);
    }
   
    void SoraFSMManager::onRender() {
        if(mCurrentState)
            mCurrentState->onRender();
        if(mGlobalState)
            mGlobalState->onRender();
    }
    
    bool SoraFSMManager::returnToPreviousState() {
        if(mPreviousState != NULL) {
            mCurrentState->onLeave();
            mCurrentState = mPreviousState;
            mCurrentState->onEnter();
            return true;
        }
        return false;
    }
    
    void SoraFSMManager::setGlobalState(SoraFSMState* state) {
        mGlobalState = state;
    }
    
    SoraFSMState* SoraFSMManager::operator[](const SoraString& name) const {
        SoraStringId uniqueId = GetUniqueStringId(name);
        FSMStateMap::const_iterator itState = mStates.find(uniqueId);
        if(itState != mStates.end()) {
            return itState->second;
        }
        return NULL;
    }
    
    namespace {
        static SoraSingletonHolder<SoraFSMManager, void> fsm;
    }
    
    SoraFSMManager& SoraFSMManager::defaultFSMManager() {
        return *fsm.get();
    }
    
    void SoraFSMManager::defTrans(const SoraString& state1, const EventType& event, const SoraString& state2) {
        SoraStringId sid1 = GetUniqueStringId(state1);
        SoraStringId sid2 = GetUniqueStringId(state2);
        
        FSMStateMap::iterator itState1 = mStates.find(sid1);
        FSMStateMap::iterator itState2 = mStates.find(sid2);
        if(itState1 != mStates.end() &&
           itState2 != mStates.end()) {
            mTransitions[itState1->second].EventMap.insert(std::make_pair(event, itState2->second));
        } else 
            THROW_SORA_EXCEPTION(NotFoundException, "State name not exist");
    }
    
    void SoraFSMManager::delTrans(const SoraString& state, const EventType& event) {
        SoraStringId sid = GetUniqueStringId(state);
        
        FSMStateMap::iterator itState = mStates.find(sid);
        if(itState != mStates.end()) {
            StateTransitionMap::iterator itTrans = mTransitions.find(itState->second);
            if(itTrans != mTransitions.end()) {
                StateEventMap::iterator itEvent = itTrans->second.EventMap.find(event);
                if(itEvent != itTrans->second.EventMap.end()) {
                    itTrans->second.EventMap.erase(itEvent);
                }
            }
        }
    }
    
    void SoraFSMManager::postEvent(const EventType& event) {
        StateTransitionMap::iterator itTrans = mTransitions.find(mCurrentState);
        if(itTrans != mTransitions.end()) {
            StateEventMap::iterator itEvent = itTrans->second.EventMap.find(event);
            if(itEvent != itTrans->second.EventMap.end()) {
                switchToState(itEvent->second);
            }
        }
    }
    
} // namespace sora