//
//  SoraPlaybackEvent.h
//  Sora
//
//  Created by Griffin Bu on 4/19/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef SORA_PLAYBACK_EVENT_H_
#define SORA_PLAYBACK_EVENT_H_

#include "SoraEvent.h"
#include "SoraConstantStrings.h"

namespace sora {
    
    /* 
        Base event class for playback events, such as a music, a soundeffect or a movie
     */
    
    class SORA_API SoraPlaybackEvent: public SoraEvent {
    public:
        enum SoraPlayBackEventType {
            Null,
            Started,
            Ended,
            Paused,
            Resumed,
            Stopped,
        };
        
        SoraPlaybackEvent(): eventType(SoraPlaybackEvent::Null) {}
        SoraPlaybackEvent(SoraPlayBackEventType event): eventType(event) {
            _setEventName(event);
        }
        
        SoraPlayBackEventType getEventType() const { 
            return eventType;
        }
        void setEventType(SoraPlayBackEventType event) { 
            eventType = event;
            _setEventName(event);
        }
		
#ifndef SORA_USE_RTTI
		SORA_EVENT_IDENTIFIER(18446744069908103960ULL);
#endif
		
    private:
        void _setEventName(SoraPlayBackEventType event) {
            switch(event) {
                case SoraPlaybackEvent::Started: setName(STR_ID_PLAY_STARTED); break;
                case SoraPlaybackEvent::Ended: setName(STR_ID_PLAY_ENDED); break;
                case SoraPlaybackEvent::Paused: setName(STR_ID_PLAY_PAUSED); break;
                case SoraPlaybackEvent::Resumed: setName(STR_ID_PLAY_RESUMED); break;
                case SoraPlaybackEvent::Stopped: setName(STR_ID_PLAY_STOPPED); break;
                default:
                    break;
            }
        }
        SoraPlayBackEventType eventType;
    };
    
    class SORA_API SoraPlaybackEventHandler: public SoraEventHandler {
    public:
        SoraPlaybackEventHandler(): pEventHandler(NULL), mEventPublish(false) {
			event = new SoraPlaybackEvent;
		}
		virtual ~SoraPlaybackEventHandler() {
			delete event;
			event = 0;
		}
        
        void registerEventHandler(SoraEventHandler* pHandler) {
            pEventHandler = pHandler;
        }
		
		/*
		 enable or disable event publishing
		 */
		void enableEventPublish(bool flag) { mEventPublish = flag; }
		bool isEventPublishEnabled() const { return mEventPublish; }
        
        void publishEvent(SoraPlaybackEvent::SoraPlayBackEventType eventType) {
            event->setEventType(eventType);
			event->setSource(this);
            if(pEventHandler)
                pEventHandler->handleEvent(event);
            this->handleEvent(event);
        }
        
        // default does nothing
        virtual void onSoundEvent(SoraPlaybackEvent* pEvent) {}
        
    protected:
        SoraEventHandler* pEventHandler;
        
        SoraPlaybackEvent* event;
		bool mEventPublish;
    };
} // namespace sora

#endif