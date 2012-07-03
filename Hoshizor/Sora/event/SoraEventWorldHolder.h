//
//  SoraEventWorldHolder.h
//  Sora
//
//  Created by Ruiwei Bu on 9/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraEventWorldHolder_h
#define Sora_SoraEventWorldHolder_h

#include "SoraPlatform.h"

#include <list>
#include <map>

namespace sora {
    
    /**
     * This class holds every EventWorld
     * Newly allocated EventWorld would register itself to this class
     * You can also register by yourself with a tag
     * So feel free to broadcast event to all existing eventworld
     **/
    
    class SoraEvent;
    class SoraEventWorld;
    
    class SORA_API SoraEventWorldHolder {
        SoraEventWorldHolder() {}
        
    public:
        static SoraEventWorldHolder* Instance();
        
        void registerEventWorld(SoraEventWorld* world);
        void removeEventWorld(SoraEventWorld* world);
        
        void registerEventWorld(const std::string& name, SoraEventWorld* world);
        void removeEventWorld(const std::string& name);
        
        void broadcast(const std::string& evtName, const std::string& to=std::string());
        void broadcast(SoraEvent* evt, const std::string& to=std::string());
        
        void broadcastDirect(const std::string& evtName, const std::string& to=std::string());
        void broadcastDirect(SoraEvent* evt, const std::string& to=std::string());
        
        SoraEventWorld* getEventWorld(const std::string& name);
        
    private:
        typedef std::list<SoraEventWorld*> EventWorldArray;
        EventWorldArray mEventWorlds;
        
        typedef std::map<std::string, SoraEventWorld*> EventWorldMap;
        EventWorldMap mEventWorldMap;
    };
    
} // namespace sora



#endif
