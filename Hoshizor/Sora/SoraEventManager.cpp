#include "SoraEventManager.h"

#include "util/SoraHash.h"

//#include "SoraLuaStateManager.h"

namespace sora {
    
    SoraEventManager* SoraEventManager::Instance() {
        static SoraEventManager instance;
        return &instance;
    }

	SoraEventManager::SoraEventManager(): mFileChangeEventPublisher(NULL) {
	}
	
	SoraEventManager::~SoraEventManager() {
		if(mFileChangeEventPublisher)
			delete mFileChangeEventPublisher;
	}
	
	void SoraEventManager::registerEvent(const SoraString& eventName, SoraEventHandler* handler, SoraEvent* ev) {
		EVENT_ID eid = BKDRHash(eventName.c_str());
		evMap[eid].push_back(SoraEventInfo(eventName, handler, ev));
	}

	void SoraEventManager::sendMessage(const SoraString& eventName, const SoraString& params, SoraHandle hreceiver) {
		SoraEventHandler* receiver = (SoraEventHandler*)hreceiver;
		EVENT_ID eid = BKDRHash(eventName.c_str());
		EVENT_MAP::iterator itEvent = evMap.find(eid);
		if(itEvent == evMap.end()) {
		//	SoraLuaObjectManager::Instance()->sendMessage(eventName, params, hreceiver);
			return;
		}

		EVENT_LIST elist = itEvent->second;
		if(elist.size() != 0) {
			EVENT_LIST::iterator itEvent = elist.begin();
			while(itEvent != elist.end()) {
				if(itEvent->handler != NULL && itEvent->ev != NULL) {
					if(receiver == NULL) {
						itEvent->ev->setSource(itEvent->handler);
						itEvent->handler->handleEvent(itEvent->ev);
					}
					else {
						if(itEvent->handler == receiver) {
							itEvent->ev->setSource(itEvent->handler);
							itEvent->handler->handleEvent(itEvent->ev);
						}
					}
				} else {
					if(itEvent->handler == NULL) {
						itEvent = elist.erase(itEvent);
						continue;
					}
				}
				++itEvent;
			}
		}

	//	SoraLuaObjectManager::Instance()->sendMessage(eventName, params, hreceiver);
	}
	
	void SoraEventManager::sendMessage(const SoraString& eventName, SoraEvent* ev, SoraHandle hreceiver) {
		SoraEventHandler* receiver = (SoraEventHandler*)hreceiver;
		EVENT_ID eid = BKDRHash(eventName.c_str());
		EVENT_MAP::iterator itEvent = evMap.find(eid);
		if(itEvent == evMap.end()) {
			return;
		}
		
		EVENT_LIST elist = itEvent->second;
		if(elist.size() != 0) {
			EVENT_LIST::iterator itEvent = elist.begin();
			while(itEvent != elist.end()) {
				if(itEvent->handler != NULL) {
					if(receiver == NULL) {
						ev->setSource(itEvent->handler);
						itEvent->handler->handleEvent(ev);
					}
					else
						if(itEvent->handler == receiver) {
							ev->setSource(itEvent->handler);
							itEvent->handler->handleEvent(ev);
						}
				} else {
					if(itEvent->handler == NULL) {
						itEvent = elist.erase(itEvent);
						continue;
					}
				}
				++itEvent;
			}
		}
	}
	
	void SoraEventManager::unregisterEvent(const SoraString& eventName) {
		EVENT_ID eid = BKDRHash(eventName.c_str());
		EVENT_MAP::iterator itEvent = evMap.find(eid);
		if(itEvent != evMap.end())
			evMap.erase(itEvent);
	}
	
	void SoraEventManager::unregisterHandlerFromEvent(const SoraString& eventName, SoraEventHandler* handler) {
		EVENT_ID eid = BKDRHash(eventName.c_str());
		EVENT_MAP::iterator itEvent = evMap.find(eid);
		if(itEvent != evMap.end()) {
			EVENT_LIST elist = itEvent->second;

			EVENT_LIST::iterator itEvent = elist.begin();
			while(itEvent != elist.end()) {
				if(itEvent->handler == handler) {
					elist.erase(itEvent);
				}
				++itEvent;
			}
		}
	}
	
	void SoraEventManager::createTimerEvent(SoraEventHandler* handler, float time, bool repeat) {
		tevList.push_back(new SoraTimerEventInfo(handler, time, 0.f, repeat)); 
	}
	
	void SoraEventManager::registerTimerEvent(SoraEventHandler* handler, SoraTimerEvent* ev, float time, bool repeat) {
		tevList.push_back(new SoraTimerEventInfo(handler, ev, time, 0.f, repeat)); 
	}
	
	void SoraEventManager::freeTimerEvent(TIMER_EVENT_LIST::iterator ittev) {
		delete (*ittev);
		(*ittev) = 0;
	}
	
	void SoraEventManager::update(float dt) {
		if(tevList.size() != 0) {
			TIMER_EVENT_LIST::iterator ittev = tevList.begin();
			while(ittev != tevList.end()) {
				(*ittev)->update(dt);
				if((*ittev)->handler == NULL) {
					ittev = tevList.erase(ittev);
					continue;
				}
				++ittev;
			}
		}
	}
	
	void SoraEventManager::unregisterTimerEvent(SoraEventHandler* handler) {
		TIMER_EVENT_LIST::iterator ittev = tevList.begin();
		while(ittev != tevList.end()) {
			if((*ittev)) {
				tevList.erase(ittev);
			} else {
				ittev = tevList.erase(ittev);
				continue;
			}
			++ittev;
		}
	}
	
	void SoraEventManager::publishInputEvent(SoraKeyEvent* kev) {
		INPUT_EVENT_HANDLER_LIST::iterator itHandler = iehList.begin();
		while(itHandler != iehList.end()) {
			if(itHandler->second) {
				itHandler->second->handleEvent(kev);
				++itHandler;
				
				if(kev->isConsumed())
					break;
			}
		}
	}
	
	void SoraEventManager::registerInputEventHandler(SoraEventHandler* handler, int32 priority) {
		iehList.insert(std::make_pair(priority, handler));
	}
	
	void SoraEventManager::unregisterInputEventHandler(SoraEventHandler* handler) {
		INPUT_EVENT_HANDLER_LIST::iterator itHandler = iehList.begin();
		while(itHandler != iehList.end()) {
            if(itHandler->second == handler) {
                iehList.erase(itHandler);
                break;
            }
            ++itHandler;
        }
	}
	
	void SoraEventManager::SoraTimerEventInfo::update(float dt) {
		float timepass = currTime + dt;
        unsigned int step = static_cast<unsigned int>(timepass / dt);
        
        totalTime += dt;

        if(step > 0) {
            ev->setTime(currTime);
			ev->setTotalTime(totalTime);
            
            sora_assert(handler);
            handler->handleEvent(ev);
            
            if(!repeat) 
                handler = NULL;
            
            currTime -= step * time;
        }
	}
	
	void SORACALL SoraEventManager::registerFileChangeEventHandler(const SoraWString& file, SoraEventHandler* handler) {
		if(mFileChangeEventPublisher == NULL)
			mFileChangeEventPublisher = new SoraFileChangeEventPublisher;
		
		mFileChangeEventPublisher->addEventHandler(file, handler);
	}
	
	void SORACALL SoraEventManager::unregisterFileChangeEventHandler(SoraEventHandler* handler) {
		if(!mFileChangeEventPublisher)
			return;
		
		mFileChangeEventPublisher->delEventHandler(handler);
	}
	
	void SORACALL SoraEventManager::setFileChangeDetectionInterval(float interval) {
		if(!mFileChangeEventPublisher)
			return;
		
		mFileChangeEventPublisher->setInterval(interval);
	}
	
} // namespace sora