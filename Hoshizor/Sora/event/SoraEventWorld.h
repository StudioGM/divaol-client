//
//  SoraEventWorld.h
//  Sora
//
//  Created by Robert Bu on 8/5/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraEventWorld_h
#define Sora_SoraEventWorld_h

#include "SoraPlatform.h"
#include "SoraScopedPtr.h"
#include "SoraEvent.h"
#include "SoraUncopyable.h"

namespace sora {
    
    class SoraEventWorldImpl;
    class SoraEventWorld;
    
    /**
     * Notice: Event go through SoraEventWorld must be constructed by SoraEventFactory
     **/
    
    class SORA_API SoraEventWorld: SoraUncopyable {
    public:
        typedef SoraEventHandler* EventHandlerPtr;
        
        struct Parameter {
            Parameter():
            mMultitrhead(false),
            mFixedStep(true) {
                
            }
            
            Parameter(bool multithread, bool fixedstep):
            mMultitrhead(multithread),
            mFixedStep(fixedstep) {
                
            }
            
            inline Parameter& multiThread(bool multiThread=true) {
                mMultitrhead = multiThread;
                return *this;
            }
            
            inline Parameter& fixedStep(bool fixedStep=true) {
                mFixedStep = fixedStep;
                return *this;
            }
            
            bool isMultiThread() const {
                return mMultitrhead;
            }
            
            bool isFixedStep() const {
                return mFixedStep;
            }
            
        private:
            bool mMultitrhead;
            bool mFixedStep;
        };
        
    public:
        SoraEventWorld(const Parameter& param = Parameter());
        ~SoraEventWorld();
        
        void update(float dt);
        
        void enter(EventHandlerPtr eh);
        void leave(EventHandlerPtr eh);
        
        bool init();
        
        void shutdown();
        
        /**
         * Event maybe temporary queued in the world
         * And be destroyed after published
         * So DO NOT use temporary variables as event
         * and DO USE SoraEventFactory to create the event
         **/
        void broadcast(SoraEvent* evet);
        
        /**
         * Publish the event imnediately
         * No queue implemented
         * And temporary event is acceptable
         * But it's unefficient if you broadcast directly many times in a frame
         **/
        void broadcastDirect(SoraEvent* event);
        
        /**
         * Create a event from EventFactory and broadcast
         **/
        void broadcast(const std::string& event);
        void broadcastDirect(const std::string& event);
        
        static SoraEventWorld& defaultWorld(const Parameter& param = Parameter());
        
    private:
        SoraScopedPtr<SoraEventWorldImpl> mImpl;
        bool mFixedStep;
    };
    
    static void EnterDefaultEventWorld(SoraEventHandler* handler) {
        SoraEventWorld::defaultWorld().enter(handler);
    }
    
    static void LeaveDefaultEventWorld(SoraEventHandler* handler) {
        SoraEventWorld::defaultWorld().leave(handler);
    }
    
} // namespace sora 

#endif
