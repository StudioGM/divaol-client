//
//  SoraObjectListener.h
//  Sora
//
//  Created by Robert Bu on 8/18/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraObjectListener_h
#define Sora_SoraObjectListener_h

#include "prerequisites.h"

namespace sora {
    
    class SoraEntity;
    
    typedef SoraSimpleFSM<SoraString, SoraString> EntityFsmType;

    class SORA_API SoraEntityListener {
    public:
        virtual ~SoraEntityListener() { }
         
        virtual void onFsmEventHandled(SoraEntity& ent, const EntityFsmType& fsm, const EntityFsmType::StateType& state) {}
        virtual void onFsmEnterState(SoraEntity& ent, const EntityFsmType& fsm, const EntityFsmType::StateType& state) {}
        virtual void onFsmExitState(SoraEntity& ent, const EntityFsmType& fsm, const EntityFsmType::StateType& state) {}
        
        virtual void onInit(SoraEntity& ent) {}
        virtual void onUpdate(SoraEntity& ent) {}
    };
    
    class SORA_API SoraEntityListenerManager: public SoraListenerManager<SoraEntityListener> {
    protected:
        typedef ConstVectorIterator<ListenerList> IteratorType;
        
        void listeners_init(SoraEntity& entity) {
            IteratorType it(mListenerList);
            while(it.hasMoreElements()) {
                it.getNextRef().first->onInit(entity);
            }
        }
        
        void listeners_update(SoraEntity& entity) {
            IteratorType it(mListenerList);
            while(it.hasMoreElements()) {
                it.getNextRef().first->onUpdate(entity);
            }
        }
        
        void listeners_fsmEventHandled(SoraEntity& entity, const EntityFsmType& fsm, const EntityFsmType::StateType& state) {
            IteratorType it(mListenerList);
            while(it.hasMoreElements()) {
                it.getNextRef().first->onFsmEventHandled(entity, fsm, state);
            }
        }
        
        void listeners_fsmEnterState(SoraEntity& entity, const EntityFsmType& fsm, const EntityFsmType::StateType& state) {
            IteratorType it(mListenerList);
            while(it.hasMoreElements()) {
                it.getNextRef().first->onFsmEnterState(entity, fsm, state);
            }
        }
        
        void listeners_fsmExitState(SoraEntity& entity, const EntityFsmType& fsm, const EntityFsmType::StateType& state) {
            IteratorType it(mListenerList);
            while(it.hasMoreElements()) {
                it.getNextRef().first->onFsmExitState(entity, fsm, state);
            }
        }
        
    };
    
} // namespace sora

#endif
