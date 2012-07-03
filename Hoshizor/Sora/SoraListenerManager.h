//
//  SoraListenerManager.h
//  Sora
//
//  Created by Robert Bu on 8/18/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraListenerManager_h
#define Sora_SoraListenerManager_h

#include "SoraPlatform.h"
#include <string>
#include <map>
#include <deque>

namespace sora {
    
    template<typename T>
    struct SoraListenerManager {  
        typedef T ListenerType;

        typedef std::map<std::string, ListenerType*> ListenerMap;
        typedef std::deque<std::pair<ListenerType*, bool> > ListenerList;

        virtual ~SoraListenerManager() {
            typename ListenerList::iterator it = mListenerList.begin();
            while(it != mListenerList.end()) {
                if(it->second) {
                    delete it->first;
                }
                ++it;
            }
        }
        
        void attachListener(ListenerType*, const std::string& tag = std::string(), bool autoRelease=false);
        
        ListenerType* getListener(const std::string& tag) const;
        
    protected:
        ListenerMap mListenerMap;
        
        ListenerList mListenerList;
    };
    
    template<typename T>
    void SoraListenerManager<T>::attachListener(ListenerType* listener, const std::string& tag, bool autoRelease) {
        sora_assert(listener);
        if(!listener)
            return;
        
        mListenerList.push_back(typename ListenerList::value_type(listener, autoRelease));
        if(!tag.empty()) {
            typename ListenerMap::const_iterator it = mListenerMap.find(tag);
            if(it != mListenerMap.end()) {
                mListenerMap.insert(std::make_pair(tag, listener));
            }
        }
    }
    
    template<typename T>
    T* SoraListenerManager<T>::getListener(const std::string& tag) const {
        typename ListenerMap::const_iterator it = mListenerMap.find(tag);
        if(it != mListenerMap.end()) 
            return it->second;
        return NULL;
    }
                    
} // namespace sora


#endif
